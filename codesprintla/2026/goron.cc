/*

    C — Goron City (user AC)

    We can use a binary search to find the minimum number of days required to complete the tasks.

    We can use a greedy algorithm to assign the tasks to the Gorons.
    The idea is to assign the tasks from the smallest to the largest strength,
    greedily assigning the houses first, then the rocks.

    To prove on why it works:

    * If we sort the rocks in increasing strength and the houses in increasing strength,
    then in an optimal solution we will assign the rocks and the houses as consecutive segments
    to each Goron.

    * For the rocks, if we are not assigning to some Goron, we can assign it to some stronger
    Goron, provided they have time to do so.

    * If we can assign t rocks to a Goron, we can exchange the t rocks to one house to assign to
    the Goron, provided the Goron has enough strength to build the house.
    Because from the previous point, we can assign the rocks to some stronger Goron.

    * Therefore, the optimal solution is to assign the houses to the Goron first, then
    assign the rocks until we run out of time.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 1000005;
int n, m, g, t;

int rocks[k_max_n], house[k_max_n];
int goron[k_max_n];

bool ok(int mid) {
  int ix = 0, iy = 0;
  for (int i = 0; i < g; i++) {
    int remaining = mid;
    while (iy < m && house[iy] <= goron[i] && remaining >= t) {
      remaining -= t;
      iy++;
    }
    while (ix < n && rocks[ix] <= goron[i] && remaining > 0) {
      remaining--;
      ix++;
    }
  }
  return (ix >= n && iy >= m);
}

int solve() {
  if (n > 0 && rocks[n - 1] > goron[g - 1]) {
    return -1;
  }

  if (m > 0 && house[m - 1] > goron[g - 1]) {
    return -1;
  }

  if (n == 0 && m == 0) {
    return 0;
  }

  int low = 1, high = n + m * t, ans = high;
  while (low <= high) {
    int mid = (low + high) / 2;
    if (ok(mid)) {
      ans = mid;
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }

  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m >> g >> t;
  for (int i = 0; i < n; i++) {
    cin >> rocks[i];
  }
  for (int i = 0; i < m; i++) {
    cin >> house[i];
  }
  for (int i = 0; i < g; i++) {
    cin >> goron[i];
  }

  sort(rocks, rocks + n);
  sort(house, house + m);
  sort(goron, goron + g);

  cout << solve() << endl;

  return 0;
}

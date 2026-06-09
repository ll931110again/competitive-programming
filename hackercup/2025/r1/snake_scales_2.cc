
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 500005;
int T;
int n, a[k_max_n];
bool vis[k_max_n];

bool ok(int mid) {
  for (int i = 1; i <= n; i++) {
    vis[i] = false;
  }

  queue<int> q;
  for (int i = 1; i <= n; i++) {
    if (a[i] <= mid) {
      q.push(i);
      vis[i] = true;
    }
  }

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    if (u > 0 && !vis[u - 1] && abs(a[u] - a[u - 1]) <= mid) {
      vis[u - 1] = true;
      q.push(u - 1);
    }

    if (u < n && !vis[u + 1] && abs(a[u] - a[u + 1]) <= mid) {
      vis[u + 1] = true;
      q.push(u + 1);
    }
  }

  for (int i = 1; i <= n; i++) {
    if (!vis[i]) {
      return false;
    }
  }

  return true;
}

int solve() {
  int low = 1, high = 1e9, ans = high;
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
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  for (int it = 1; it <= T; it++) {
    cerr << it << endl;
    cin >> n;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }

    cout << "Case #" << it << ": " << solve() << endl;
  }

  return 0;
}

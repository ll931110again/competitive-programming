// Codeforces 1810 (CodeTON Round 4 (Div. 1 + Div. 2, Rated, Prizes!)) — C. Make It Permutation
// Submission: https://codeforces.com/contest/1810/submission/201603958

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 100005;
int T, n, delete_cost, insert_cost;
int a[k_max_n];

i64 solve() {
  scanf("%d %d %d", &n, &delete_cost, &insert_cost);
  for (int i = 0; i < n; i++) {
    scanf("%d", &a[i]);
  }
  sort(a, a + n);

  // first case corresponds to delete everything and insert 1.
  i64 min_cost = 1LL * n * delete_cost + insert_cost;
  unordered_set<int> reps;
  for (int i = 0; i < n; i++) {
    reps.insert(a[i]);
    min_cost =
        min((unsigned long long)min_cost, 1LL * (i + 1 - reps.size() + (n - 1 - i)) * delete_cost +
                                              1LL * (a[i] - reps.size()) * insert_cost);
  }

  return min_cost;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cout << solve(<< '\n');
  }
  return 0;
}

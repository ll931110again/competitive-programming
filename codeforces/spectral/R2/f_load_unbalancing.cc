// Codeforces Spectral::Cup 2026 Round 2 — Load Unbalancing
// https://codeforces.com/contest/2229/problem/F
//
// Sketch
// ------
// It is optimal to place max(a) last. After that the last assignment adds
// max(a) to a current minimum bin, so maximizing the final max(b) is equivalent
// to maximizing the minimum bin value v among the other n-1 items, then adding
// max(a).
//
// Binary-search v. A mask DP records the maximum (number of groups, leftover)
// obtainable from a subset: appending an element either grows the leftover or
// completes a new group once the leftover reaches v. Feasible iff the full
// set produces at least k groups.
//
// Complexity: O(n 2^n log (n A)) per test, and sum 2^n <= 2^18.

#include <bits/stdc++.h>
using namespace std;

namespace {

int64_t max_unbalance(vector<int64_t> values, int bin_count) {
  sort(values.begin(), values.end());
  const int64_t extra = values.back();
  values.pop_back();
  const int m = static_cast<int>(values.size());

  int64_t lo = 0;
  int64_t hi = extra * max(m, 1);
  while (lo < hi) {
    const int64_t mid = lo + (hi - lo + 1) / 2;
    vector<pair<int, int64_t>> dp(1 << m, {0, 0});
    for (int mask = 1; mask < (1 << m); mask++) {
      for (int i = 0; i < m; i++) {
        if ((mask & (1 << i)) == 0) {
          continue;
        }
        pair<int, int64_t> cur = dp[mask ^ (1 << i)];
        cur.second += values[i];
        if (cur.second >= mid) {
          cur.first++;
          cur.second = 0;
        }
        dp[mask] = max(dp[mask], cur);
      }
    }
    if (dp[(1 << m) - 1].first >= bin_count) {
      lo = mid;
    } else {
      hi = mid - 1;
    }
  }
  return lo + extra;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int test_count;
  cin >> test_count;
  while (test_count--) {
    int n, k;
    cin >> n >> k;
    vector<int64_t> values(n);
    for (int64_t& x : values) {
      cin >> x;
    }
    cout << max_unbalance(std::move(values), k) << '\n';
  }
  return 0;
}

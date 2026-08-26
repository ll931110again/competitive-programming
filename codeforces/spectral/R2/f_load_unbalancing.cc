// Codeforces Spectral::Cup 2026 Round 2 — Load Unbalancing
// https://codeforces.com/contest/2229/problem/F
//
// Sketch
// ------
// Binary search the load threshold and SOS/subset DP over n <= 18 items
// to test whether extra capacity can form enough groups. O(2^n n log ANS).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 18;
int T, n, k;
vector<int> a;
pair<int, int64_t> dp[1 << k_max_n];

int64_t solve() {
  int extra = a.back();
  a.pop_back();
  n--;

  int64_t low = 0, high = 1LL * extra * n, ans = low;
  while (low <= high) {
    int64_t mid = (low + high) / 2;
    dp[0] = {0, 0};
    for (int mask = 1; mask < (1 << n); mask++) {
      dp[mask] = {0, 0};
      for (int i = 0; i < n; i++)
        if (mask & (1 << i)) {
          auto cur = dp[mask ^ (1 << i)];
          cur.second += a[i];
          if (cur.second >= mid) {
            cur.first++;
            cur.second = 0;
          }
          dp[mask] = max(dp[mask], cur);
        }
    }

    if (dp[(1 << n) - 1].first >= k) {
      ans = mid;
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return (ans + extra);
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> k;
    a.resize(n);
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    sort(a.begin(), a.end());
    cout << solve() << endl;
  }

  return 0;
}

// Codeforces 1842 (CodeTON Round 5 (Div. 1 + Div. 2, Rated, Prizes!)) — C. Tenzing and Balls
// Submission: https://codeforces.com/contest/1842/submission/334342594

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int T, n;
int a[k_max_n];

int dp[k_max_n], min_val[k_max_n], min_pos[k_max_n];

int solve() {
  for (int i = 1; i <= n; i++) {
    dp[i] = n + 1;
    min_val[i] = n + 1;
  }

  for (int i = 1; i <= n; i++) {
    dp[i] = min(dp[i - 1] + 1, min_val[a[i]]);
    min_val[a[i]] = min(min_val[a[i]], dp[i - 1]);
  }

  return n - dp[n];
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }
    cout << solve() << endl;
  }
  return 0;
}

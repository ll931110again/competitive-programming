
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 505;
#define mod 998244353

int T;
int n, k;
int a[2][k_max_n];

i64 dp[2][k_max_n][k_max_n];
i64 delta[2 * k_max_n];

i64 solve() {
  memset(dp, 0, sizeof dp);
  dp[0][0][k] = 1;

  for (int reset = 0; reset < 2; reset++) {
    for (int i = 0; i + 1 < n; i++) {
      // calculate the difference a[i + 1] - b[i]
      memset(delta, 0, sizeof delta);
      for (int u = 1; u <= k; u++)
        if (a[0][i + 1] == -1 || a[0][i + 1] == u) {
          for (int v = 1; v <= k; v++)
            if (a[1][i] == -1 || a[1][i] == v) {
              int d = u - v;
              delta[d + k]++;
            }
        }

      for (int j = 0; j <= k; j++)
        if (dp[i][j]) {
          for (int d = -k; d <= k; d++)
            if (delta[d + k]) {
              int _reset = reset || (d < 0);
              int _j = j + d;

              if (d < 0) {
                _j = d + k;
              }

              if (!_reset) {
                _j = k;
              }

              if (reset && _j > k) {
                break;
              }

              dp[_reset][i + 1][_j] += dp[reset][i][j] * delta[d + k];
              dp[_reset][i + 1][_j] %= mod;
            }
        }
    }
  }

  i64 ans = 0;
  for (int reset = 0; reset < 2; reset++) {
    for (int j = 0; j <= k; j++) {
      ans += dp[reset][n - 1][j];
      ans %= mod;
    }
  }

  i64 first_choice = 0, last_choice = 0;
  for (int u = 1; u <= k; u++)
    if (a[0][0] == -1 || a[0][0] == u) {
      first_choice++;
    }
  for (int u = 1; u <= k; u++)
    if (a[1][n - 1] == -1 || a[1][n - 1] == u) {
      last_choice++;
    }

  ans *= first_choice;
  ans *= last_choice;
  ans %= mod;
  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> k;
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < n; j++) {
        cin >> a[i][j];
      }
    }
    cout << solve() << endl;
  }
  return 0;
}


#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 22;
#define maxl 10000

int T;
double p;

int N = 20;
double dp[maxl][k_max_n];
double pin[k_max_n], pout[k_max_n];
i64 binom[k_max_n][k_max_n];

double solve(double p) {
  pin[0] = 1;
  pout[0] = 1;
  for (int i = 1; i <= N; i++) {
    pin[i] = pin[i - 1] * p;
    pout[i] = pout[i - 1] * (1 - p);
  }

  memset(dp, 0, sizeof dp);
  dp[0][0] = 1;
  for (int i = 1; i < maxl; i++) {
    for (int j = 0; j <= N; j++) {
      for (int k = 0; k <= j; k++) {
        dp[i][j] += dp[i - 1][k] * pin[j - k] * pout[N - j] * binom[N - k][j - k];
      }
    }
  }

  double ans = 0;
  for (int i = 1; i < maxl; i++) {
    ans += i * (dp[i][N] - dp[i - 1][N]);
  }

  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  binom[0][0] = 1;
  for (int i = 1; i <= N; i++) {
    for (int j = 0; j <= i; j++) {
      binom[i][j] = binom[i - 1][j];
      if (j) {
        binom[i][j] += binom[i - 1][j - 1];
      }
    }
  }

  cin >> T;
  for (int it = 1; it <= T; it++) {
    cin >> p;
    cout << fixed << setprecision(9) << "Case #" << it << ": " << solve(p) << '\n';
  }

  return 0;
}

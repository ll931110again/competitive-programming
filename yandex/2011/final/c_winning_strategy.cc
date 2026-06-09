// Codeforces 97 (Yandex.Algorithm 2011: Finals) — C. Winning Strategy
// Submission: https://codeforces.com/contest/97/submission/263665030

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 305;
double dp[k_max_n + 5][k_max_n * k_max_n + 5];
int n;
double a[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 0; i <= n; i++) {
    cin >> a[i];
  }

  for (int i = 0; i < k_max_n; i++) {
    for (int j = 0; j < k_max_n * k_max_n; j++) {
      dp[i][j] = -1;
    }
  }
  dp[0][0] = 0;

  for (int i = 0; i < k_max_n; i++) {
    for (int j = 0; j < k_max_n * k_max_n; j++)
      if (dp[i][j] >= 0) {
        for (int k = 0; k <= n; k++) {
          dp[i + 1][j + k] = max(dp[i + 1][j + k], dp[i][j] + a[k]);
        }
      }
  }

  double ret = 0;
  for (int i = 1; i < k_max_n; i++) {
    ret = max(ret, dp[i][(i * n) / 2] / i);
  }
  cout << ret << endl;
  return 0;
}

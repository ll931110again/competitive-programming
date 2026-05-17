// Codeforces 97 (Yandex.Algorithm 2011: Finals) — C. Winning Strategy
// Submission: https://codeforces.com/contest/97/submission/263665030

#include <bits/stdc++.h>
#define maxn 305
using namespace std;

double dp[maxn + 5][maxn * maxn + 5];
int n;
double a[maxn];

int main() {
  cin >> n;
  for (int i = 0; i <= n; i++) {
    cin >> a[i];
  }

  for (int i = 0; i < maxn; i++) {
    for (int j = 0; j < maxn * maxn; j++) {
      dp[i][j] = -1;
    }
  }
  dp[0][0] = 0;

  for (int i = 0; i < maxn; i++) {
    for (int j = 0; j < maxn * maxn; j++)
      if (dp[i][j] >= 0) {
        for (int k = 0; k <= n; k++) {
          dp[i + 1][j + k] = max(dp[i + 1][j + k], dp[i][j] + a[k]);
        }
      }
  }

  double ret = 0;
  for (int i = 1; i < maxn; i++) {
    ret = max(ret, dp[i][(i * n) / 2] / i);
  }
  cout << ret << endl;
  return 0;
}

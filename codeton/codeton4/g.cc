#include <bits/stdc++.h>
using namespace std;

static constexpr int maxn = 5005;
static constexpr unsigned MOD = 1'000'000'007;
#include "../../lib/modint.h"
using Mint = ModInt<MOD>;

int T, n;
int x[maxn], y[maxn], h[maxn];

Mint p[maxn];
Mint dp[maxn][maxn];

void solve() {
  for (int i = 1; i <= n; i++) {
    p[i] = x[i];
    p[i] /= y[i];
  }

  for (int i = 0; i <= n; i++) {
    dp[0][i] = h[i];
  }

  for (int i = 1; i <= n; i++) {
    for (int j = 0; j <= n; j++) {
      dp[i][j] = p[i] * dp[i - 1][j + 1];
      dp[i][j] += (1 - p[i]) * dp[i - 1][max(j - 1, 0)];
    }
  }

  for (int i = 1; i <= n; i++) {
    cout << dp[i][0] << ' ';
  }
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 1; i <= n; i++) {
      cin >> x[i] >> y[i];
    }
    for (int i = 0; i <= n; i++) {
      cin >> h[i];
    }
    solve();
  }
  return 0;
}

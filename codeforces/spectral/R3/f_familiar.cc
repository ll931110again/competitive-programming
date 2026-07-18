
#include <bits/stdc++.h>
#define maxn 505
using namespace std;

int T, n;
int a[maxn];

int mod = 998'244'353;
int binom[maxn][maxn];

int f[maxn][maxn][maxn];
int g[maxn][maxn];
int w[maxn][maxn];

long long solve() {
  int sa = 0;
  for (int i = 0; i < n; i++)
    if (a[i] >= 0) {
      sa += a[i];
    }
  if (sa >= n) {
    return 0;
  }

  for (int i = 1; i <= n; i++) {
    f[i][i - 1][0] = 0;
  }

  for (int l = 1; l <= n; l++) {
    for (int i = 1; i + l - 1 <= n; i++) {
      int j = i + l - 1;

      for (int k = i; k <= j; k++) {
        long long value = binom[j - i][k - i] * w[i][k] % mod;
        value = value * g[k + 1][j] % mod;
        g[i][j] = (g[i][j] + value) % mod;
      }

      w[i][j] = (a[j] >= 0) ? f[i][j - 1][a[j]] : g[i][j - 1];
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  binom[0][0] = 1;
  for (int i = 1; i < maxn; i++) {
    for (int j = 0; j <= i; j++) {
      binom[i][j] = (binom[i - 1][j] + (j ? binom[i - 1][j - 1] : 0)) % mod;
    }
  }

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

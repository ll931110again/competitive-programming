#include <bits/stdc++.h>
using namespace std;

static constexpr int maxn = 20;
static constexpr unsigned MOD = 998'244'353;
#include "../../lib/modint.h"
using Mint = ModInt<MOD>;

int n, m;
int constraints[maxn][2];
Mint f[1 << maxn];

int main() {
  cin >> n >> m;
  for (int i = 0; i < m; i++) {
    int t, u, v;
    cin >> t >> u >> v;
    u--;
    v--;
    constraints[u][t] |= (1 << v);
    constraints[v][t] |= (1 << u);
  }

  f[0] = 1;
  for (int mask = 0; mask < (1 << n); mask++) {
    for (int i = 0; i < n; i++)
      if ((mask & (1 << i)) == 0) {
        if ((constraints[i][0] | mask) == mask) {
          f[mask | (1 << i)] += f[mask];
        }
        if ((constraints[i][1] | mask) == mask) {
          f[mask | (1 << i)] += f[mask];
        }
      }
  }

  Mint ways = Mint(2).pow(n);
  for (int i = 2; i <= n; i++) {
    ways *= i;
  }

  Mint ans = f[(1 << n) - 1] / ways;
  cout << ans << endl;

  return 0;
}
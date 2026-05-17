#include <bits/stdc++.h>
#define maxn 205
#define maxk 30
using namespace std;

int n, m, Q;

static constexpr unsigned MOD = 1'000'000'007;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

Mint both[maxn][maxn][maxk];
Mint ones[maxn][maxk];

int main() {
  cin >> n >> m >> Q;
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    both[u][v][0]++;
    both[v][u][0]++;
  }

  for (int k = 1; k < maxk; k++) {
    for (int u = 1; u <= n; u++) {
      for (int v = 1; v <= n; v++) {
        for (int z = 1; z <= n; z++) {
          both[u][v][k] += both[u][z][k - 1] * both[z][v][k - 1];
        }
      }
    }
  }

  for (int k = 0; k < maxk; k++) {
    for (int u = 1; u <= n; u++) {
      for (int v = 1; v <= n; v++) {
        ones[u][k] += both[u][v][k];
      }
    }
  }

  while (Q--) {
    int u, k;
    cin >> u >> k;

    Mint cur[maxn], nxt[maxn];
    memset(cur, 0, sizeof cur);
    cur[u] = 1;

    for (int i = maxk - 1; i >= 0; i--)
      if (k & (1 << i)) {
        memset(nxt, 0, sizeof nxt);
        for (int u = 1; u <= n; u++)
          if (cur[u]) {
            for (int v = 1; v <= n; v++) {
              nxt[v] += cur[u] * both[u][v][i];
            }
          }
        for (int u = 1; u <= n; u++) {
          cur[u] = nxt[u];
        }
      }

    Mint ret = 0;
    for (int u = 1; u <= n; u++) {
      ret += cur[u];
    }

    cout << ret << endl;
  }
  return 0;
}
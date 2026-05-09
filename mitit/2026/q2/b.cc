#include <bits/stdc++.h>
using namespace std;

static constexpr int maxn = 100005;
static constexpr unsigned MOD = 1'000'000'007;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

int T, n;

vector<int> adj[maxn];
Mint dp[maxn];
int sz[maxn];
Mint fact[maxn], inv[maxn], p2[maxn];

Mint binom(int x, int y) { return fact[x] * inv[y] * inv[x - y]; }

Mint rec(int u, int p) {
  sz[u] = 1;
  dp[u] = 1;

  int odd_count = 0, even_count = 0;
  for (auto v : adj[u])
    if (v != p) {
      rec(v, u);
      dp[u] *= dp[v];

      sz[u] += sz[v];

      if (sz[v] & 1) {
        odd_count++;
      } else {
        even_count++;
      }
    }

  if (odd_count > 0) {
    dp[u] *= p2[even_count] * binom(odd_count, odd_count / 2);
  }

  return dp[u];
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  fact[0] = 1;
  for (int i = 1; i < maxn; i++) {
    fact[i] = fact[i - 1] * i;
  }
  inv[maxn - 1] = Mint(1) / fact[maxn - 1];
  for (int i = maxn - 2; i >= 0; i--) {
    inv[i] = inv[i + 1] * (i + 1);
  }

  p2[0] = 1;
  for (int i = 1; i < maxn; i++) {
    p2[i] = p2[i - 1] * 2;
  }

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 1; i <= n; i++) {
      adj[i].clear();
    }
    for (int i = 1; i < n; i++) {
      int u, v;
      cin >> u >> v;
      adj[u].push_back(v);
      adj[v].push_back(u);
    }

    for (int i = 1; i <= n; i++) {
      dp[i] = -1;
    }
    cout << rec(1, -1) << endl;
  }

  return 0;
}
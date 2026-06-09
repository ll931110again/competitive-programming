#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 100005;
constexpr unsigned MOD = 1'000'000'007;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

int T, n;

vector<int> adj[k_max_n];
Mint dp[k_max_n];
int sz[k_max_n];
Mint fact[k_max_n], inv[k_max_n], p2[k_max_n];

Mint binom(int x, int y) {
  return fact[x] * inv[y] * inv[x - y];
}

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

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  fact[0] = 1;
  for (int i = 1; i < k_max_n; i++) {
    fact[i] = fact[i - 1] * i;
  }
  inv[k_max_n - 1] = Mint(1) / fact[k_max_n - 1];
  for (int i = k_max_n - 2; i >= 0; i--) {
    inv[i] = inv[i + 1] * (i + 1);
  }

  p2[0] = 1;
  for (int i = 1; i < k_max_n; i++) {
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

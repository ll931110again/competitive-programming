#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>
#define maxn 200005
using namespace std;

static constexpr unsigned MOD = 1'000'000'007;
#include "../../lib/modint.h"
using Mint = ModInt<MOD>;

Mint fact[maxn], inv[maxn];

int T, n, m;
vector<int> adj[maxn];

Mint solve() {
  if (m >= n) {
    return 0;
  }

  if (n == 2) {
    return 2;
  }

  int nonleaf = 0;
  vector<int> leaf_deg(n + 1), nonleaf_deg(n + 1);

  for (int i = 1; i <= n; i++) {
    if (adj[i].size() == 1) {
      for (auto j : adj[i]) {
        leaf_deg[j]++;
      }
    } else {
      nonleaf++;
      for (auto j : adj[i]) {
        nonleaf_deg[j]++;
      }
    }
  }

  for (int i = 1; i <= n; i++)
    if (nonleaf_deg[i] >= 3) {
      return 0;
    }

  Mint ans = 2;
  if (nonleaf >= 2) {
    ans *= 2;
  }
  for (auto x : leaf_deg) {
    ans *= fact[x];
  }

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  fact[0] = 1;
  for (int i = 1; i < maxn; i++) {
    fact[i] = fact[i - 1] * i;
  }

  cin >> T;
  while (T--) {
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
      adj[i].clear();
    }
    for (int i = 0; i < m; i++) {
      int u, v;
      cin >> u >> v;
      adj[u].push_back(v);
      adj[v].push_back(u);
    }
    cout << solve() << endl;
  }
  return 0;
}
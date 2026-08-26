// Codeforces Spectral::Cup 2026 Round 2 — Deconstruction Tree
// https://codeforces.com/contest/2229/problem/E
//
// Sketch
// ------
// DP on the path from the rightmost leaf toward n. Prefix sums of reachable
// ways, modulo 998244353. O(n).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
constexpr int kMod = 998'244'353;
int T, n;
vector<int> adj[k_max_n];
int max_child[k_max_n];

int64_t dp[k_max_n], prefix[k_max_n];
bool reachable[k_max_n];

void dfs(int u, int p) {
  max_child[u] = -1;
  for (auto v : adj[u])
    if (v != p) {
      dfs(v, u);
      max_child[u] = max({max_child[u], max_child[v], v});
    }
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int it = 0;

  cin >> T;
  while (T--) {
    it++;
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

    dfs(n, -1);
    for (int i = 0; i <= n; i++) {
      dp[i] = prefix[i] = 0;
    }

    int initial_leaf = 0;
    for (int i = n; i > 0; i--)
      if (adj[i].size() == 1) {
        initial_leaf = i;
        break;
      }

    if (initial_leaf == n) {
      cout << 1 << endl;
      continue;
    }

    dp[initial_leaf] = 1;
    prefix[initial_leaf] = 1;

    for (int i = initial_leaf + 1; i < n; i++) {
      dp[i] = 0;
      if (max_child[i] < i) {
        dp[i] = prefix[i - 1];
        if (max_child[i] >= 0) {
          dp[i] -= prefix[max_child[i]];
          if (dp[i] < 0) {
            dp[i] += kMod;
          }
        }
      }
      prefix[i] = (prefix[i - 1] + dp[i]) % kMod;
    }

    set<int, greater<int>> s;
    vector<int> cur;

    auto dfs2 = [&](auto self, int u, int p) -> void {
      s.erase(u);
      cur.push_back(u);
      for (auto v : adj[u])
        if (v != p) {
          self(self, v, u);
        }
    };

    for (int i = 1; i < n; i++) {
      reachable[i] = false;
      s.insert(i);
    }

    for (auto x : adj[n]) {
      dfs2(dfs2, x, n);

      for (auto z : cur) {
        if (z > (*s.begin())) {
          reachable[z] = true;
        }
      }

      for (auto z : cur) {
        s.insert(z);
      }
      cur.clear();
    }

    int64_t ans = 0;
    for (int i = 1; i < n; i++)
      if (reachable[i]) {
        ans = (ans + dp[i]) % kMod;
      }

    cout << ans << endl;
  }

  return 0;
}

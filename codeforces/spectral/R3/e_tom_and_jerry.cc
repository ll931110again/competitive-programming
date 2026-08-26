// Codeforces Spectral::Cup 2026 Round 3 — Tom and Jerry
// https://codeforces.com/contest/2245/problem/E
//
// Sketch
// ------
// On the tree, DFS through even-degree components and charge odd-degree
// neighbors. The total is the number of winning first paths. O(n).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int kMaxN = 200005;

int T, n;
vector<int> adj[kMaxN];

int64_t solve() {
  int64_t ans = 0;
  int c = 0;

  vector<bool> vis(n);
  auto dfs = [&](auto self, int u) -> void {
    vis[u] = true;
    for (auto v : adj[u]) {
      if (adj[v].size() & 1) {
        ans += c++;
      } else if (!vis[v]) {
        self(self, v);
      }
    }
  };

  for (int u = 0; u < n; u++)
    if (~adj[u].size() & 1 && !vis[u]) {
      c = 0;
      dfs(dfs, u);
    }

  for (int u = 0; u < n; u++) {
    for (auto v : adj[u])
      if (v > u) {
        ans += adj[u].size() & adj[v].size() & 1;
      }
  }

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      adj[i].clear();
    }
    for (int i = 0; i + 1 < n; i++) {
      int u, v;
      cin >> u >> v;
      u--;
      v--;

      adj[u].emplace_back(v);
      adj[v].emplace_back(u);
    }

    cout << solve() << endl;
  }

  return 0;
}

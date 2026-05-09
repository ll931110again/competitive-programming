// D – Chemtech Contagion
// https://codesprintla25.kattis.com/problems/chemtechcontagion
//
// Close every vent that joins a room on the tree path between a and b with a
// room off that path — otherwise gas reaches that off-path room via the vent.
// Each query is independent (pretend other transports / closures don't exist).

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M, Q;
  if (!(cin >> N >> M >> Q))
    return 0;
  vector<vector<int>> adj(N + 1);
  for (int i = 0; i < N - 1; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  const int LOG = 20;
  vector<int> dep(N + 1), tin(N + 1), tout(N + 1), par(N + 1);
  vector<vector<int>> up(N + 1, vector<int>(LOG));
  int timer = 0;
  function<void(int, int)> dfs = [&](int u, int p) {
    tin[u] = ++timer;
    par[u] = p;
    up[u][0] = p;
    for (int k = 1; k < LOG; ++k)
      up[u][k] = up[up[u][k - 1]][k - 1];
    for (int v : adj[u]) {
      if (v == p)
        continue;
      dep[v] = dep[u] + 1;
      dfs(v, u);
    }
    tout[u] = timer;
  };
  dfs(1, 1);

  auto anc = [&](int u, int v) -> bool {
    return tin[u] <= tin[v] && tin[v] <= tout[u];
  };

  auto lca = [&](int u, int v) -> int {
    if (anc(u, v))
      return u;
    if (anc(v, u))
      return v;
    for (int k = LOG - 1; k >= 0; --k) {
      if (!anc(up[u][k], v))
        u = up[u][k];
    }
    return par[u];
  };

  auto dist = [&](int u, int v) -> int {
    int w = lca(u, v);
    return dep[u] + dep[v] - 2 * dep[w];
  };

  auto walk_on_path = [&](int a, int b, int target) -> bool {
    // Is target on simple path a–b?
    int w = lca(a, b);
    return dist(a, target) + dist(target, b) == dist(a, b);
  };

  vector<pair<int, int>> vents(M);
  for (int i = 0; i < M; ++i) {
    int u, v;
    cin >> u >> v;
    vents[i] = {u, v};
  }

  while (Q--) {
    int a, b;
    cin >> a >> b;
    int ans = 0;
    for (auto [u, v] : vents) {
      bool ua = walk_on_path(a, b, u);
      bool va = walk_on_path(a, b, v);
      if (ua ^ va)
        ++ans;
    }
    cout << ans << '\n';
  }
  return 0;
}

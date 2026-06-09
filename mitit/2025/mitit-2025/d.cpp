
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int T, n, m, k;
int vertices[k_max_n];
int dist[k_max_n];
vector<int> adj[k_max_n];

void BFS(int source) {
  for (int i = 1; i <= n; i++) {
    dist[i] = (i == source) ? 0 : -1;
  }

  queue<int> q;
  q.push(source);

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    for (auto v : adj[u]) {
      if (dist[v] == -1) {
        dist[v] = 1 + dist[u];
        q.push(v);
      }
    }
  }
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  int T;
  cin >> T;
  while (T--) {
    cin >> n >> m >> k;
    for (int i = 0; i < k; i++) {
      cin >> vertices[i];
    }

    for (int i = 1; i <= n; i++) {
      adj[i].clear();
    }

    for (int i = 0; i < m; i++) {
      int u, v;
      cin >> u >> v;
      adj[u].push_back(v);
      adj[v].push_back(u);
    }

    vector<pair<int, int>> edges;
    if (vertices[0] != 1) {
      edges.push_back({1, vertices[0]});
    }
    for (int i = 1; i < k; i++) {
      edges.push_back({vertices[i - 1], vertices[i]});
    }

    for (auto edge : edges) {
      adj[edge.first].push_back(edge.second);
      adj[edge.second].push_back(edge.first);
    }

    BFS(1);

    bool ok = true;
    for (int i = 1; i < k; i++) {
      if (dist[vertices[i]] <= dist[vertices[i - 1]]) {
        ok = false;
        break;
      }
    }

    if (!ok) {
      cout << -1 << endl;
      continue;
    }

    cout << edges.size() << endl;
    for (auto edge : edges) {
      cout << edge.first << ' ' << edge.second << endl;
    }
  }
  return 0;
}

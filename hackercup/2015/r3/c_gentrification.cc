#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 505
using namespace std;

int T, n, m;
vector<int> edges[maxn];

bool vis[maxn];
bool reachable[maxn][maxn];

struct BipartiteGraph {
  int N;
  vector< vector<int> > adj;
  int left_index[maxn], right_index[maxn];
  int trace[maxn];

  int maximum_matching() {
    memset(left_index, -1, sizeof left_index);
    memset(right_index, -1, sizeof right_index);

    memset(trace, -1, sizeof trace);
    int ans = 0;

    while (true) {
      bool proceed = false;
      memset(trace, -1, sizeof trace);
      for (int i = 0; i < N; i++) if (left_index[i] < 0) {
        int d = augment(i);
        if (d >= 0) {
          ans++;
          proceed = true;
          break;
        }
      }

      if (!proceed) {
        break;
      }
    }

    return ans;
  }

  int augment(int u) {
    for (auto v : adj[u]) if (trace[v] < 0) {
      trace[v] = u;
      if (right_index[v] < 0) {
        left_index[u] = v;
        right_index[v] = u;
        return v;
      }
      
      int d = augment(right_index[v]);
      if (d >= 0) {
        left_index[u] = v;
        right_index[v] = u;
        return d;
      }
    }

    return -1;
  }
};

void DFS(int u, int root) {
  for (auto v : edges[u]) if (!reachable[root][v]) {
    reachable[root][v] = true;
    DFS(v, root);
  }
}

BipartiteGraph build_bipartite_graph() {
  BipartiteGraph graph;

  graph.N = n;
  graph.adj.resize(graph.N);

  memset(reachable, false, sizeof reachable);
  for (int i = 0; i < n; i++) {
    DFS(i, i);
  }

  for (int u = 0; u < n; u++) {
    for (int v = 0; v < n; v++) if (reachable[u][v] && !reachable[v][u]) {
      graph.adj[u].push_back(v);
    }
  }

  return graph;
}

struct SCCGraph {
  vector<int> scc_size;
  vector< set<int> > adj;

  BipartiteGraph build_bipartite_graph() {
    BipartiteGraph graph;

    graph.N = 0;
    for (auto x : scc_size) {
      graph.N += x;
    }
    graph.adj.resize(graph.N);

    int ranges[maxn];
    ranges[0] = 0;
    for (int i = 0; i < scc_size.size(); i++) {
      ranges[i + 1] = ranges[i] + scc_size[i];
    }

    for (int u = 0; u < scc_size.size(); u++) {
      for (auto v : adj[u]) {
        for (int iu = ranges[u]; iu < ranges[u + 1]; iu++) {
          for (int iv = ranges[v]; iv < ranges[v + 1]; iv++) {
            graph.adj[iu].push_back(iv);
          }
        }
      }
    }

    return graph;
  }
};

int solve() {
  auto bipartite_graph = build_bipartite_graph();
  auto matching = bipartite_graph.maximum_matching();
  return n - matching;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  
  cin >> T;
  for (int it = 1; it <= T; it++) {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
      edges[i].clear();
    }
    for (int i = 0; i < m; i++) {
      int u, v;
      cin >> u >> v;
      edges[u].push_back(v);
    }

    cout << "Case #" << it << ": " << solve() << endl;
  }

  return 0;
}
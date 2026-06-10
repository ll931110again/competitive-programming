// Dinic max flow (reference implementation for tests).
#pragma once

#include <algorithm>
#include <limits>
#include <queue>
#include <vector>

namespace maxflow {

struct Dinic {
  struct Edge {
    int to = 0;
    int rev = 0;
    int cap = 0;
  };

  int n = 0;
  std::vector<std::vector<Edge>> g;
  std::vector<int> level;
  std::vector<int> it;

  explicit Dinic(int n_) : n(n_), g(n_) {}

  void add_edge(int u, int v, int cap) {
    Edge fwd{v, static_cast<int>(g[v].size()), cap};
    Edge rev{u, static_cast<int>(g[u].size()), 0};
    g[u].push_back(fwd);
    g[v].push_back(rev);
  }

  bool bfs(int s, int t) {
    level.assign(n, -1);
    std::queue<int> q;
    level[s] = 0;
    q.push(s);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (const Edge& e : g[u]) {
        if (e.cap > 0 && level[e.to] < 0) {
          level[e.to] = level[u] + 1;
          q.push(e.to);
        }
      }
    }
    return level[t] >= 0;
  }

  int dfs(int u, int t, int f) {
    if (u == t) {
      return f;
    }
    for (int& i = it[u]; i < static_cast<int>(g[u].size()); ++i) {
      Edge& e = g[u][i];
      if (e.cap > 0 && level[e.to] == level[u] + 1) {
        int pushed = dfs(e.to, t, std::min(f, e.cap));
        if (pushed > 0) {
          e.cap -= pushed;
          g[e.to][e.rev].cap += pushed;
          return pushed;
        }
      }
    }
    return 0;
  }

  int max_flow(int s, int t) {
    const int k_inf = std::numeric_limits<int>::max() / 4;
    int flow = 0;
    while (bfs(s, t)) {
      it.assign(n, 0);
      while (true) {
        int pushed = dfs(s, t, k_inf);
        if (pushed == 0) {
          break;
        }
        flow += pushed;
      }
    }
    return flow;
  }
};

} // namespace maxflow

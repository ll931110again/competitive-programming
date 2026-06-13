// Nagamochi-Ibaraki maximum adjacency search and sparse connectivity certificates.
//
// Used for Benczur-Karger edge strength estimation (BK96, arXiv:cs/0207078).
#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <tuple>
#include <vector>

namespace nagamochi_ibaraki {

struct WeightedGraph {
  int n = 0;
  struct Edge {
    int u = 0;
    int v = 0;
    double cap = 0.0;
    int id = 0;
  };
  std::vector<Edge> edges;
  std::vector<std::vector<std::pair<int, int>>> adj; // (neighbor, edge_id)

  void init(int n_) {
    n = n_;
    edges.clear();
    adj.assign(n, {});
  }

  int add_edge(int u, int v, double cap) {
    const int id = static_cast<int>(edges.size());
    edges.push_back({u, v, cap, id});
    adj[u].push_back({v, id});
    adj[v].push_back({u, id});
    return id;
  }

  int m() const {
    return static_cast<int>(edges.size());
  }
};

// Maximum adjacency ordering. Returns order and last[v] = weighted degree to
// previously ordered vertices when v was added.
inline std::pair<std::vector<int>, std::vector<double>>
maximum_adjacency_ordering(const WeightedGraph& graph) {
  std::vector<int> order;
  order.reserve(graph.n);
  std::vector<double> last(graph.n, 0.0);
  std::vector<char> used(graph.n, 0);

  for (int step = 0; step < graph.n; ++step) {
    int best = -1;
    double best_w = -1.0;
    for (int v = 0; v < graph.n; ++v) {
      if (used[v]) {
        continue;
      }
      if (best < 0 || last[v] > best_w) {
        best = v;
        best_w = last[v];
      }
    }
    order.push_back(best);
    used[best] = 1;
    for (const auto& [to, eid] : graph.adj[best]) {
      if (!used[to]) {
        last[to] += graph.edges[eid].cap;
      }
    }
  }
  return {order, last};
}

// Edges with NI connectivity >= k (standard edge connectivity lower bound).
inline std::vector<int> k_certificate_edges(const WeightedGraph& graph, double k) {
  const auto [order, last] = maximum_adjacency_ordering(graph);
  std::vector<int> pos(graph.n);
  for (int i = 0; i < graph.n; ++i) {
    pos[order[i]] = i;
  }
  std::vector<int> cert;
  cert.reserve(graph.m());
  for (const auto& e : graph.edges) {
    const int pu = pos[e.u];
    const int pv = pos[e.v];
    const int later = pu > pv ? e.u : e.v;
    const int earlier = pu > pv ? e.v : e.u;
    if (last[later] >= k - 1e-12) {
      cert.push_back(e.id);
      (void)earlier;
    }
  }
  return cert;
}

// Tight strength bounds: k_e <= true strong connectivity, sum 1/k_e = O(n).
// Benczur-Karger Estimation (Section 4.4), simplified doubling schedule.
inline std::vector<double> estimate_strong_connectivities(const WeightedGraph& graph) {
  std::vector<double> strength(graph.m(), 1.0);
  std::vector<char> assigned(graph.m(), 0);

  WeightedGraph active = graph;
  std::vector<int> active_id(active.m());
  for (int i = 0; i < active.m(); ++i) {
    active_id[i] = i;
  }

  for (double k = 1.0; k <= static_cast<double>(active.m()) + graph.n; k *= 2.0) {
    if (active.m() == 0) {
      break;
    }
    const std::vector<int> cert = k_certificate_edges(active, 2.0 * k);
    std::vector<char> in_cert(active.m(), 0);
    for (int eid : cert) {
      in_cert[eid] = 1;
    }

    std::vector<char> remove(active.m(), 0);
    for (int i = 0; i < active.m(); ++i) {
      if (!in_cert[i] && !assigned[active_id[i]]) {
        strength[active_id[i]] = std::max(1.0, k / 2.0);
        assigned[active_id[i]] = 1;
        remove[i] = 1;
      }
    }

    if (std::none_of(remove.begin(), remove.end(), [](char x) { return x != 0; })) {
      break;
    }

    WeightedGraph next;
    next.init(active.n);
    std::vector<int> next_id;
    for (int i = 0; i < active.m(); ++i) {
      if (!remove[i]) {
        const auto& e = active.edges[i];
        const int nid = next.add_edge(e.u, e.v, e.cap);
        next_id.push_back(active_id[i]);
        (void)nid;
      }
    }
    active = std::move(next);
    active_id = std::move(next_id);
  }

  for (int i = 0; i < graph.m(); ++i) {
    if (!assigned[i]) {
      strength[i] = static_cast<double>(graph.m());
    }
  }
  return strength;
}

} // namespace nagamochi_ibaraki

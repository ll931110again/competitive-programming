// Benczur-Karger cut sparsifier (STOC 1996, arXiv:cs/0207078 Section 2).
//
// Samples edges with probability proportional to 1/strong_connectivity, rescales
// capacities so every cut is preserved within (1 +/- epsilon) w.h.p.
#pragma once

#include "lib/nagamochi_ibaraki.hh"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

namespace graph_sparsifier {

struct SparsifierOptions {
  double epsilon = 0.1;
  uint64_t seed = 1;
};

struct SparsifyResult {
  nagamochi_ibaraki::WeightedGraph graph;
  std::vector<int> origin_edge; // sparsifier edge -> original edge id
  bool ok = false;
};

inline uint64_t splitmix64(uint64_t& state) {
  state += 0x9e3779b97f4a7c15ULL;
  uint64_t z = state;
  z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
  z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
  return z ^ (z >> 31);
}

inline double unit_double(uint64_t& state) {
  return (splitmix64(state) >> 11) * (1.0 / 9007199254740992.0);
}

inline nagamochi_ibaraki::WeightedGraph to_weighted(int n, const std::vector<int>& u,
                                                    const std::vector<int>& v,
                                                    const std::vector<double>& cap) {
  nagamochi_ibaraki::WeightedGraph w;
  w.init(n);
  for (size_t i = 0; i < cap.size(); ++i) {
    w.add_edge(u[i], v[i], cap[i]);
  }
  return w;
}

// Build G[p_E] = (1/p_E) * G(p_E) from BK Section 2.1.
inline SparsifyResult sparsify(const nagamochi_ibaraki::WeightedGraph& graph,
                               const SparsifierOptions& options) {
  SparsifyResult out;
  if (graph.n <= 0) {
    return out;
  }
  const double eps = options.epsilon;
  const double rho = 4.0 * std::log(std::max(2.0, static_cast<double>(graph.n))) / (eps * eps);
  const std::vector<double> strength = nagamochi_ibaraki::estimate_strong_connectivities(graph);

  out.graph.init(graph.n);
  out.origin_edge.clear();
  uint64_t rng = options.seed;

  for (const auto& e : graph.edges) {
    const double k = std::max(1.0, strength[e.id]);
    const double p = std::min(1.0, rho / k);
    if (unit_double(rng) <= p) {
      const double cap = e.cap / p;
      out.graph.add_edge(e.u, e.v, cap);
      out.origin_edge.push_back(e.id);
    }
  }

  // Preserve connectivity: if sampling removed all edges, keep the heaviest one.
  if (out.graph.m() == 0 && graph.m() > 0) {
    int best = 0;
    for (int i = 1; i < graph.m(); ++i) {
      if (graph.edges[i].cap > graph.edges[best].cap) {
        best = i;
      }
    }
    const auto& e = graph.edges[best];
    out.graph.add_edge(e.u, e.v, e.cap);
    out.origin_edge.push_back(best);
  }

  out.ok = true;
  return out;
}

} // namespace graph_sparsifier

// Karger graph smoothing for approximate max-flow (BK98, arXiv:cs/0207078 Section 3).
//
// Makes a graph c-smooth (k_e >= c * u_e), partitions edges into 1/p random
// groups, and combines sub-flows per Lemma 3.5 / Theorem 4.5 of arXiv:1010.2921.
#pragma once

#include "lib/graph_sparsifier.hh"
#include "lib/nagamochi_ibaraki.hh"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

namespace graph_smoothing {

struct SmoothingOptions {
  double epsilon = 0.1;
  uint64_t seed = 1;
};

struct SmoothGraph {
  nagamochi_ibaraki::WeightedGraph graph;
  std::vector<int> origin_edge; // smooth edge -> input edge id
  double smoothness = 1.0;
};

// Lemma 3.3: split heavy edges so k_e >= c * u_e on each piece.
inline SmoothGraph make_smooth(const nagamochi_ibaraki::WeightedGraph& graph, double smoothness_c) {
  SmoothGraph out;
  out.graph.init(graph.n);
  out.smoothness = std::max(1.0, smoothness_c);
  const std::vector<double> strength = nagamochi_ibaraki::estimate_strong_connectivities(graph);

  for (const auto& e : graph.edges) {
    const double k = std::max(1.0, strength[e.id]);
    const int pieces = static_cast<int>(std::ceil(out.smoothness * e.cap / k));
    const double piece_cap = e.cap / pieces;
    for (int i = 0; i < pieces; ++i) {
      out.graph.add_edge(e.u, e.v, piece_cap);
      out.origin_edge.push_back(e.id);
    }
  }
  return out;
}

inline nagamochi_ibaraki::WeightedGraph
default_smooth(const nagamochi_ibaraki::WeightedGraph& graph) {
  const double c = graph.n > 0 ? static_cast<double>(graph.m()) / graph.n : 1.0;
  return make_smooth(graph, c).graph;
}

// Partition smoothed edges into num_groups disjoint groups (Lemma 3.5).
inline std::vector<std::pair<nagamochi_ibaraki::WeightedGraph, std::vector<int>>>
partition_with_origins(const SmoothGraph& smooth, int num_groups, uint64_t seed) {
  std::vector<nagamochi_ibaraki::WeightedGraph> groups(num_groups);
  std::vector<std::vector<int>> origins(num_groups);
  for (int i = 0; i < num_groups; ++i) {
    groups[i].init(smooth.graph.n);
  }
  for (int i = 0; i < smooth.graph.m(); ++i) {
    uint64_t rng = seed + static_cast<uint64_t>(i) * 0x9e3779b97f4a7c15ULL;
    const int g =
        static_cast<int>(graph_sparsifier::splitmix64(rng) % static_cast<uint64_t>(num_groups));
    const auto& e = smooth.graph.edges[i];
    groups[g].add_edge(e.u, e.v, e.cap);
    origins[g].push_back(smooth.origin_edge[i]);
  }
  std::vector<std::pair<nagamochi_ibaraki::WeightedGraph, std::vector<int>>> out;
  out.reserve(num_groups);
  for (int i = 0; i < num_groups; ++i) {
    out.push_back({groups[i], origins[i]});
  }
  return out;
}

inline std::vector<nagamochi_ibaraki::WeightedGraph>
partition_groups(const nagamochi_ibaraki::WeightedGraph& graph, int num_groups) {
  SmoothGraph wrapper;
  wrapper.graph = graph;
  wrapper.origin_edge.resize(graph.m());
  for (int i = 0; i < graph.m(); ++i) {
    wrapper.origin_edge[i] = i;
  }
  std::vector<nagamochi_ibaraki::WeightedGraph> groups;
  for (const auto& [g, origin] : partition_with_origins(wrapper, num_groups, 1)) {
    (void)origin;
    groups.push_back(g);
  }
  return groups;
}

inline int num_groups_for_smoothness(int m, int n, double epsilon) {
  if (n <= 0 || m <= 0) {
    return 1;
  }
  const double c = static_cast<double>(m) / n;
  const double rho = 4.0 * std::log(std::max(2.0, static_cast<double>(n))) / (epsilon * epsilon);
  const double p = std::min(1.0, rho / c);
  return std::max(1, static_cast<int>(std::ceil(1.0 / p)));
}

} // namespace graph_smoothing

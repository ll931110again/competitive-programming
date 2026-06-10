// Shared helpers for max-flow gtests (LCT vs Dinic).
#pragma once

#include "lib/maxflow_dinic.hh"
#include "lib/maxflow_lct.hh"

#include <gtest/gtest.h>

#include <cstdint>
#include <utility>
#include <vector>

namespace maxflow_test {

constexpr int kStressSeeds = 8;
constexpr int kStressMinN = 12;
constexpr int kStressMaxN = 36;

struct StressRng {
  uint64_t state;

  explicit StressRng(uint64_t seed) : state(seed + 1) {}

  uint64_t next_u64() {
    state = state * 6364136223846793005ULL + 1442695040888963407ULL;
    return state;
  }

  int rand_int(int lo, int hi) {
    if (hi <= lo) {
      return lo;
    }
    return lo + static_cast<int>(next_u64() % static_cast<uint64_t>(hi - lo + 1));
  }
};

struct FlowGraph {
  int n = 0;
  std::vector<std::tuple<int, int, int>> edges;

  void add_edge(int u, int v, int cap) {
    edges.emplace_back(u, v, cap);
  }
};

inline int dinic_max_flow(const FlowGraph& graph, int s, int t) {
  maxflow::Dinic dinic(graph.n);
  for (const auto& [u, v, cap] : graph.edges) {
    dinic.add_edge(u, v, cap);
  }
  return dinic.max_flow(s, t);
}

inline int lct_max_flow(const FlowGraph& graph, int s, int t) {
  maxflow::LctMaxFlow flow(graph.n);
  for (const auto& [u, v, cap] : graph.edges) {
    flow.add_edge(u, v, cap);
  }
  return flow.max_flow(s, t);
}

inline void expect_flow_algorithms_match(const FlowGraph& graph, int s, int t) {
  const int dinic = dinic_max_flow(graph, s, t);
  const int lct = lct_max_flow(graph, s, t);
  EXPECT_EQ(lct, dinic) << "LCT max flow disagrees with Dinic";
}

inline FlowGraph random_graph(StressRng& rng, int n, int m) {
  FlowGraph g;
  g.n = n;
  for (int i = 0; i < m; ++i) {
    int u = rng.rand_int(0, n - 2);
    int v = rng.rand_int(u + 1, n - 1);
    int cap = rng.rand_int(1, 1000);
    g.add_edge(u, v, cap);
  }
  // Guarantee a positive-flow candidate path 0 -> n-1.
  g.add_edge(0, n - 1, rng.rand_int(1, 500));
  for (int i = 0; i + 1 < n; ++i) {
    if (rng.rand_int(0, 1) == 0) {
      g.add_edge(i, i + 1, rng.rand_int(1, 500));
    }
  }
  return g;
}

} // namespace maxflow_test

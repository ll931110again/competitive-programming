// Tests for Christiano et al. electrical-flow approximate max flow / min cut.

#include "lib/maxflow_christiano.hh"
#include "lib/maxflow_dinic.hh"

#include "support/maxflow_test_helpers.hh"

#include <gtest/gtest.h>

#include <cmath>
#include <set>
#include <tuple>
#include <vector>

namespace {

maxflow::christiano::Graph to_christiano(const std::vector<std::tuple<int, int, int>>& edges,
                                         int n) {
  maxflow::christiano::Graph g;
  g.n = n;
  for (const auto& [u, v, cap] : edges) {
    g.add_edge(u, v, static_cast<double>(cap));
  }
  return g;
}

int dinic_value(const std::vector<std::tuple<int, int, int>>& edges, int n, int s, int t) {
  maxflow::Dinic dinic(n);
  for (const auto& [u, v, cap] : edges) {
    dinic.add_edge(u, v, cap);
  }
  return dinic.max_flow(s, t);
}

int undirected_dinic_value(const std::vector<std::tuple<int, int, int>>& edges, int n, int s,
                           int t) {
  maxflow::Dinic dinic(n);
  for (const auto& [u, v, cap] : edges) {
    dinic.add_edge(u, v, cap);
    dinic.add_edge(v, u, cap);
  }
  return dinic.max_flow(s, t);
}

void expect_approx_max_flow(const maxflow::christiano::Graph& g, int s, int t, int exact,
                            const maxflow::christiano::Config& config) {
  const auto result = maxflow::christiano::approximate_max_flow(g, s, t, config);
  ASSERT_TRUE(result.ok) << "algorithm failed to produce a flow";
  EXPECT_LE(result.value, exact + 1e-3);
  EXPECT_GE(result.value, (1.0 - 3.0 * config.epsilon) * exact - 1e-3)
      << "flow=" << result.value << " exact=" << exact;
  for (size_t i = 0; i < g.edges.size(); ++i) {
    const double cong = std::abs(result.edge_flow[i]) / g.edges[i].cap;
    EXPECT_LE(cong, 1.0 + 1e-5) << "edge " << i << " violates capacity";
  }
}

maxflow::christiano::Config fast_config() {
  maxflow::christiano::Config config;
  config.epsilon = 0.2;
  config.improved = true;
  config.max_mw_rounds = 8;
  return config;
}

maxflow::christiano::Config large_stress_config() {
  maxflow::christiano::Config config;
  config.epsilon = 0.2;
  config.improved = true;
  config.max_mw_rounds = 20;
  config.flow_delta = 0.05;
  return config;
}

maxflow_test::FlowGraph random_undirected_graph(maxflow_test::StressRng& rng, int n, int m) {
  maxflow_test::FlowGraph g;
  g.n = n;
  for (int i = 0; i < m; ++i) {
    const int u = rng.rand_int(0, n - 2);
    const int v = rng.rand_int(u + 1, n - 1);
    g.add_edge(u, v, rng.rand_int(1, 100));
  }
  g.add_edge(0, n - 1, rng.rand_int(10, 200));
  for (int i = 0; i + 1 < n; ++i) {
    if (rng.rand_int(0, 2) == 0) {
      g.add_edge(i, i + 1, rng.rand_int(5, 100));
    }
  }
  return g;
}

maxflow::christiano::Graph to_christiano(const maxflow_test::FlowGraph& g) {
  return to_christiano(g.edges, g.n);
}

void expect_christiano_vs_dinic(const maxflow_test::FlowGraph& graph, int s, int t,
                                const maxflow::christiano::Config& config) {
  const int exact = undirected_dinic_value(graph.edges, graph.n, s, t);
  const auto christiano_g = to_christiano(graph);
  const auto result = maxflow::christiano::approximate_max_flow(christiano_g, s, t, config);
  ASSERT_TRUE(result.ok) << "electrical flow failed";
  std::cout << "  dinic=" << exact << " electrical=" << result.value
            << " ratio=" << (exact > 0 ? result.value / exact : 0.0) << '\n';
  EXPECT_LE(result.value, exact + 1e-2);
  EXPECT_GE(result.value, (1.0 - 3.0 * config.epsilon) * exact - 1e-2)
      << "electrical=" << result.value << " dinic=" << exact;
  for (size_t i = 0; i < christiano_g.edges.size(); ++i) {
    EXPECT_LE(std::abs(result.edge_flow[i]) / christiano_g.edges[i].cap, 1.0 + 1e-4)
        << "edge " << i << " infeasible";
  }
}

} // namespace

// ---------------------------------------------------------------------------
// Large stress: electrical flow (Christiano) vs exact Dinic on undirected graphs
// ---------------------------------------------------------------------------

TEST(ChristianoLargeStress, RandomUndirectedGraphs) {
  constexpr int k_seeds = 16;
  constexpr int k_min_n = 32;
  constexpr int k_max_n = 56;
  const auto config = large_stress_config();
  for (int seed = 0; seed < k_seeds; ++seed) {
    maxflow_test::StressRng rng(1000 + seed);
    const int n = rng.rand_int(k_min_n, k_max_n);
    const int m = rng.rand_int(2 * n, 5 * n);
    const auto graph = random_undirected_graph(rng, n, m);
    SCOPED_TRACE("seed=" + std::to_string(seed) + " n=" + std::to_string(n) +
                 " m=" + std::to_string(graph.edges.size()));
    expect_christiano_vs_dinic(graph, 0, n - 1, config);
  }
}

TEST(ChristianoLargeStress, GridGraph) {
  // h x w grid, undirected unit-capacity edges.
  const int h = 12;
  const int w = 12;
  const int n = h * w;
  maxflow_test::FlowGraph graph;
  graph.n = n;
  auto id = [w](int r, int c) { return r * w + c; };
  for (int r = 0; r < h; ++r) {
    for (int c = 0; c < w; ++c) {
      if (r + 1 < h) {
        graph.add_edge(id(r, c), id(r + 1, c), 10);
      }
      if (c + 1 < w) {
        graph.add_edge(id(r, c), id(r, c + 1), 10);
      }
    }
  }
  expect_christiano_vs_dinic(graph, id(0, 0), id(h - 1, w - 1), large_stress_config());
}

TEST(ChristianoLargeStress, LayeredNetwork) {
  // 6 layers, 10 nodes each, dense connections between consecutive layers.
  const int layers = 6;
  const int width = 10;
  const int n = layers * width;
  maxflow_test::FlowGraph graph;
  graph.n = n;
  maxflow_test::StressRng rng(4242);
  for (int L = 0; L + 1 < layers; ++L) {
    for (int i = 0; i < width; ++i) {
      for (int j = 0; j < width; ++j) {
        if (rng.rand_int(0, 2) == 0) {
          graph.add_edge(L * width + i, (L + 1) * width + j, rng.rand_int(5, 50));
        }
      }
    }
  }
  for (int i = 0; i + 1 < width; ++i) {
    graph.add_edge(i, i + 1, rng.rand_int(20, 80));
    graph.add_edge(n - width + i, n - width + i + 1, rng.rand_int(20, 80));
  }
  expect_christiano_vs_dinic(graph, 0, n - 1, large_stress_config());
}

TEST(ChristianoLargeStress, Figure1Large) {
  // Paper Figure 1 at k=4 (k=6+ needs full MW iteration count for tight bounds).
  const int k = 4;
  const int n = k * (k - 1) + 2;
  maxflow_test::FlowGraph graph;
  graph.n = n;
  graph.add_edge(0, 1, k + 1);
  for (int i = 0; i < k; ++i) {
    int prev = 0;
    for (int j = 1; j < k; ++j) {
      const int node = 2 + i * (k - 1) + (j - 1);
      graph.add_edge(prev, node, k + 1);
      prev = node;
    }
    graph.add_edge(prev, 1, k + 1);
  }
  expect_christiano_vs_dinic(graph, 0, 1, large_stress_config());
}

TEST(ChristianoMaxFlow, EmptyPath) {
  const auto g = to_christiano({}, 2);
  expect_approx_max_flow(g, 0, 1, 0, fast_config());
}

TEST(ChristianoMaxFlow, SingleEdge) {
  const auto g = to_christiano({{0, 1, 7}}, 2);
  expect_approx_max_flow(g, 0, 1, 7, fast_config());
}

TEST(ChristianoMaxFlow, ParallelEdges) {
  const auto g = to_christiano({{0, 2, 3}, {0, 2, 5}, {0, 1, 4}, {1, 2, 6}}, 3);
  expect_approx_max_flow(g, 0, 2, 8, fast_config());
}

TEST(ChristianoMaxFlow, ClassicSmallNetwork) {
  // Undirected max-flow benchmark (each edge is one undirected link).
  std::vector<std::tuple<int, int, int>> edges = {
      {0, 1, 16}, {0, 2, 13}, {1, 2, 10}, {1, 3, 12}, {2, 4, 14},
      {3, 2, 9},  {3, 5, 20}, {4, 3, 8},  {4, 5, 4},
  };
  const int exact = undirected_dinic_value(edges, 6, 0, 5);
  const auto g = to_christiano(edges, 6);
  expect_approx_max_flow(g, 0, 5, exact, fast_config());
}

TEST(ChristianoMaxFlow, SimpleOracleVariant) {
  std::vector<std::tuple<int, int, int>> edges = {
      {0, 1, 10},
      {1, 2, 10},
      {0, 2, 5},
  };
  const int exact = dinic_value(edges, 3, 0, 2);
  auto config = fast_config();
  config.improved = false;
  const auto g = to_christiano(edges, 3);
  expect_approx_max_flow(g, 0, 2, exact, config);
}

TEST(ChristianoMaxFlow, Figure1StressGraph) {
  // Paper Figure 1: k parallel paths of length k plus a direct s-t edge (m = Theta(k^2)).
  const int k = 4;
  const int n = k * (k - 1) + 2;
  const int s = 0;
  const int t = 1;
  std::vector<std::tuple<int, int, int>> edges;
  edges.emplace_back(s, t, k + 1);
  for (int i = 0; i < k; ++i) {
    int prev = s;
    for (int j = 1; j < k; ++j) {
      const int node = 2 + i * (k - 1) + (j - 1);
      edges.emplace_back(prev, node, k + 1);
      prev = node;
    }
    edges.emplace_back(prev, t, k + 1);
  }
  const int exact = undirected_dinic_value(edges, n, s, t);
  const auto g = to_christiano(edges, n);
  expect_approx_max_flow(g, s, t, exact, fast_config());
}

TEST(ChristianoMinCut, MatchesMaxFlowOnSmallGraph) {
  std::vector<std::tuple<int, int, int>> edges = {
      {0, 1, 3},
      {0, 2, 2},
      {1, 3, 2},
      {2, 3, 3},
  };
  const int exact = dinic_value(edges, 4, 0, 3);
  const auto g = to_christiano(edges, 4);

  maxflow::christiano::Config config;
  config.epsilon = 0.05;
  config.max_mw_rounds = 12;

  const auto cut = maxflow::christiano::approximate_min_cut(g, 0, 3, exact, config);
  ASSERT_TRUE(cut.ok);
  EXPECT_LE(cut.capacity, (1.0 + 3.0 * config.epsilon) * exact + 1e-3);
  EXPECT_GE(cut.capacity, (1.0 - 3.0 * config.epsilon) * exact - 1e-3);
  EXPECT_EQ(cut.side[0], 1);
  EXPECT_EQ(cut.side[3], 0);
}

TEST(ChristianoElectrical, UnitFlowEnergyEqualsEffectiveResistance) {
  maxflow::christiano::Graph g;
  g.n = 3;
  g.add_edge(0, 1, 10.0);
  g.add_edge(1, 2, 10.0);
  std::vector<double> resistances(g.edges.size(), 1.0);
  const double r_eff = maxflow::christiano::effective_resistance(g, 0, 2, resistances);
  EXPECT_NEAR(r_eff, 2.0, 1e-6);
}

TEST(ChristianoMaxFlow, RandomUndirectedGraphs) {
  auto config = fast_config();
  config.max_mw_rounds = 6;
  for (int seed = 0; seed < 5; ++seed) {
    maxflow_test::StressRng local(100 + seed);
    const int n = local.rand_int(6, 12);
    maxflow::christiano::Graph g;
    g.n = n;
    std::vector<std::tuple<int, int, int>> edges;
    const int m = local.rand_int(n, 2 * n);
    for (int i = 0; i < m; ++i) {
      const int u = local.rand_int(0, n - 2);
      const int v = local.rand_int(u + 1, n - 1);
      const int cap = local.rand_int(1, 20);
      edges.emplace_back(u, v, cap);
      g.add_edge(u, v, static_cast<double>(cap));
    }
    const int st_cap = local.rand_int(1, 20);
    g.add_edge(0, n - 1, st_cap);
    edges.emplace_back(0, n - 1, st_cap);
    const int exact = undirected_dinic_value(edges, n, 0, n - 1);
    SCOPED_TRACE("seed=" + std::to_string(seed));
    expect_approx_max_flow(g, 0, n - 1, exact, config);
  }
}

TEST(ChristianoMaxFlow, SmoothedAlgorithm) {
  std::vector<std::tuple<int, int, int>> edges;
  for (int i = 0; i < 20; ++i) {
    edges.emplace_back(0, 1 + (i % 10), 3);
    edges.emplace_back(1 + (i % 10), 11, 3);
  }
  edges.emplace_back(0, 11, 5);
  const int exact = undirected_dinic_value(edges, 12, 0, 11);
  const auto g = to_christiano(edges, 12);
  maxflow::christiano::Config config;
  config.epsilon = 0.25;
  config.max_mw_rounds = 10;
  config.rng_seed = 7;
  const auto result = maxflow::christiano::approximate_max_flow_smoothed(g, 0, 11, config);
  ASSERT_TRUE(result.ok);
  EXPECT_GE(result.value, (1.0 - 4.0 * config.epsilon) * exact - 1e-3);
  for (size_t i = 0; i < g.edges.size(); ++i) {
    EXPECT_LE(std::abs(result.edge_flow[i]) / g.edges[i].cap, 1.0 + 1e-4);
  }
}

TEST(ChristianoMaxFlow, SparsifierFlowValue) {
  std::vector<std::tuple<int, int, int>> edges = {
      {0, 1, 5}, {1, 2, 5}, {0, 2, 3}, {2, 3, 4}, {1, 3, 2},
  };
  const int exact = undirected_dinic_value(edges, 4, 0, 3);
  const auto g = to_christiano(edges, 4);
  maxflow::christiano::Config config;
  config.epsilon = 0.15;
  config.max_mw_rounds = 8;
  config.rng_seed = 123;
  const auto value = maxflow::christiano::approximate_max_flow_value(g, 0, 3, config);
  ASSERT_TRUE(value.ok);
  EXPECT_GE(value.value, (1.0 - 3.0 * config.epsilon) * exact - 1e-3);
  EXPECT_LE(value.value, exact + 1e-3);
}

TEST(ChristianoMinCut, SparsifiedMinCut) {
  std::vector<std::tuple<int, int, int>> edges = {
      {0, 1, 3},
      {0, 2, 2},
      {1, 3, 2},
      {2, 3, 3},
  };
  const int exact = dinic_value(edges, 4, 0, 3);
  const auto g = to_christiano(edges, 4);
  maxflow::christiano::Config config;
  config.epsilon = 0.1;
  config.max_mw_rounds = 10;
  config.rng_seed = 99;
  const auto cut = maxflow::christiano::approximate_min_cut_sparsified(g, 0, 3, config);
  ASSERT_TRUE(cut.ok);
  EXPECT_LE(cut.capacity, (1.0 + 3.0 * config.epsilon) * exact + 1e-3);
  EXPECT_GE(cut.capacity, (1.0 - 3.0 * config.epsilon) * exact - 1e-3);
}

TEST(ChristianoElectrical, ApproximateSolverMatchesExact) {
  maxflow::christiano::Graph g;
  g.n = 4;
  g.add_edge(0, 1, 5.0);
  g.add_edge(1, 3, 5.0);
  g.add_edge(0, 2, 4.0);
  g.add_edge(2, 3, 4.0);
  std::vector<double> r(g.edges.size(), 1.0);
  maxflow::christiano::Config approx_cfg;
  approx_cfg.approximate_solver = true;
  approx_cfg.flow_delta = 0.01;
  maxflow::christiano::Config exact_cfg;
  exact_cfg.approximate_solver = false;
  const std::set<int> forbidden;
  const auto approx =
      maxflow::christiano::detail::electrical_flow(g, 0, 3, 3.0, r, forbidden, approx_cfg);
  const auto exact =
      maxflow::christiano::detail::electrical_flow(g, 0, 3, 3.0, r, forbidden, exact_cfg);
  ASSERT_TRUE(approx.ok);
  ASSERT_TRUE(exact.ok);
  EXPECT_NEAR(approx.edge_flow[0], exact.edge_flow[0], 0.5);
  EXPECT_NEAR(maxflow::christiano::detail::flow_value(g, 0, 3, approx.edge_flow), 3.0, 0.1);
}

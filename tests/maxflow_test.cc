// Google Test: link-cut tree blocking-flow max flow vs Dinic.

#include "support/maxflow_test_helpers.hh"

#include <gtest/gtest.h>

using namespace maxflow_test;

TEST(MaxflowUnit, EmptyPath) {
  FlowGraph g;
  g.n = 2;
  expect_flow_algorithms_match(g, 0, 1);
}

TEST(MaxflowUnit, SingleEdge) {
  FlowGraph g;
  g.n = 2;
  g.add_edge(0, 1, 7);
  expect_flow_algorithms_match(g, 0, 1);
}

TEST(MaxflowUnit, ParallelEdges) {
  FlowGraph g;
  g.n = 3;
  g.add_edge(0, 2, 3);
  g.add_edge(0, 2, 5);
  g.add_edge(0, 1, 4);
  g.add_edge(1, 2, 6);
  expect_flow_algorithms_match(g, 0, 2);
}

TEST(MaxflowUnit, ClassicSmallNetwork) {
  // CLRS-style toy network (0 source, 5 sink).
  FlowGraph g;
  g.n = 6;
  g.add_edge(0, 1, 16);
  g.add_edge(0, 2, 13);
  g.add_edge(1, 2, 10);
  g.add_edge(1, 3, 12);
  g.add_edge(2, 1, 4);
  g.add_edge(2, 4, 14);
  g.add_edge(3, 2, 9);
  g.add_edge(3, 5, 20);
  g.add_edge(4, 3, 8);
  g.add_edge(4, 5, 4);
  expect_flow_algorithms_match(g, 0, 5);
}

TEST(MaxflowUnit, BipartiteMatchingLayer) {
  FlowGraph g;
  g.n = 7; // 0 source, 1..3 left, 4..5 right, 6 sink
  for (int l = 1; l <= 3; ++l) {
    g.add_edge(0, l, 1);
  }
  g.add_edge(1, 4, 1);
  g.add_edge(1, 5, 1);
  g.add_edge(2, 4, 1);
  g.add_edge(3, 5, 1);
  for (int r = 4; r <= 5; ++r) {
    g.add_edge(r, 6, 1);
  }
  expect_flow_algorithms_match(g, 0, 6);
}

TEST(MaxflowUnit, SaturatedMiddleBottleneck) {
  FlowGraph g;
  g.n = 4;
  g.add_edge(0, 1, 100);
  g.add_edge(0, 2, 100);
  g.add_edge(1, 3, 5);
  g.add_edge(2, 3, 7);
  expect_flow_algorithms_match(g, 0, 3);
}

// ---------------------------------------------------------------------------
// Stress tests
// ---------------------------------------------------------------------------

TEST(MaxflowStress, RandomGraphsMatchDinic) {
  int ran = 0;
  for (int seed = 0; seed < kStressSeeds; ++seed) {
    StressRng rng(seed);
    const int n = rng.rand_int(kStressMinN, kStressMaxN);
    const int m = rng.rand_int(n, 4 * n);
    FlowGraph g = random_graph(rng, n, m);
    SCOPED_TRACE("seed=" + std::to_string(seed) + " n=" + std::to_string(n) +
                 " m=" + std::to_string(m));
    expect_flow_algorithms_match(g, 0, n - 1);
    ++ran;
  }
  EXPECT_EQ(ran, kStressSeeds);
}

TEST(MaxflowStress, DenseSmallCapacities) {
  for (int seed = 0; seed < kStressSeeds; ++seed) {
    StressRng rng(1000 + seed);
    const int n = rng.rand_int(16, 28);
    FlowGraph g;
    g.n = n;
    for (int u = 0; u < n; ++u) {
      for (int v = u + 1; v < n; ++v) {
        if (rng.rand_int(0, 2) == 0) {
          g.add_edge(u, v, rng.rand_int(1, 20));
        }
      }
    }
    g.add_edge(0, n - 1, 1);
    SCOPED_TRACE("dense seed=" + std::to_string(seed));
    expect_flow_algorithms_match(g, 0, n - 1);
  }
}

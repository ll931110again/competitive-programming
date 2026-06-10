// Google Test: Bowyer–Watson Delaunay / Voronoi vs brute force.

#include "support/delaunay2d_test_helpers.hh"

#include <gtest/gtest.h>

using namespace delaunay2d;
using namespace delaunay2d_test;

// ---------------------------------------------------------------------------
// Unit tests
// ---------------------------------------------------------------------------

TEST(Delaunay2dUnit, EmptyAndSmall) {
  expect_delaunay_edges_match({});
  expect_voronoi_adjacency_match({});

  std::vector<Pt> one = {{0, 0}};
  expect_delaunay_edges_match(one);
  expect_voronoi_adjacency_match(one);

  std::vector<Pt> two = {{0, 0}, {10, 0}};
  expect_delaunay_edges_match(two);
  expect_voronoi_adjacency_match(two);
}

TEST(Delaunay2dUnit, PerturbedSquare) {
  std::vector<Pt> sq = {{0, 0}, {10, 0}, {0, 10}, {11, 10}};
  expect_delaunay_edges_match(sq);
  expect_voronoi_adjacency_match(sq);

  std::vector<Tri> fast = delaunay(std::vector<Pt>(sq));
  EXPECT_EQ(static_cast<int>(fast.size()), 2);

  std::vector<std::set<int>> adj = brute_voronoi_adjacency(sq);
  for (int i = 0; i < 4; ++i) {
    EXPECT_GE(static_cast<int>(adj[i].size()), 2) << "vertex " << i;
  }
}

TEST(Delaunay2dUnit, Triangle) {
  std::vector<Pt> tri = {{0, 0}, {20, 0}, {10, 15}};
  expect_delaunay_edges_match(tri);
  expect_voronoi_adjacency_match(tri);
  EXPECT_EQ(static_cast<int>(brute_delaunay(tri).size()), 1);
}

TEST(Delaunay2dUnit, FivePointsGeneralPosition) {
  std::vector<Pt> pent = {{0, 0}, {20, 2}, {23, 19}, {9, 24}, {-3, 11}};
  expect_delaunay_and_voronoi_match(pent);
  expect_voronoi_midpoints_nearest(pent);
}

TEST(Delaunay2dUnit, CollinearNoTriangles) {
  std::vector<Pt> line = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};
  EXPECT_TRUE(brute_delaunay(line).empty());
  expect_delaunay_edges_match(line);
  expect_voronoi_adjacency_match(line);
}

TEST(Delaunay2dUnit, BruteTrianglePredicate) {
  std::vector<Pt> pts = {{0, 0}, {6, 0}, {3, 5}};
  EXPECT_TRUE(is_delaunay_triangle(0, 1, 2, pts));

  std::vector<Pt> with_interior = {{0, 0}, {8, 0}, {0, 8}, {2, 2}};
  EXPECT_FALSE(is_delaunay_triangle(0, 1, 2, with_interior));
  EXPECT_TRUE(is_delaunay_edge(0, 3, with_interior));
}

// ---------------------------------------------------------------------------
// Stress tests (n = 50..100, general position only)
// ---------------------------------------------------------------------------

TEST(Delaunay2dStress, LargeDelaunayAndVoronoi) {
  int ran = 0;
  int exact = 0;
  for (int seed = 0; seed < kStressSeeds; ++seed) {
    StressRng rng(seed);
    int n = rng.rand_int(kStressMinN, kStressMaxN);
    std::vector<Pt> pts = rng.random_points(n);
    if (has_duplicate_sites(pts) || has_cocircular_quadruple(pts)) {
      continue;
    }
    SCOPED_TRACE("seed=" + std::to_string(seed) + " n=" + std::to_string(n));
    expect_delaunay_triangulation_valid(pts);

    if (delaunay_graphs_equal(pts)) {
      expect_voronoi_adjacency_match(pts);
      expect_voronoi_midpoints_nearest(pts, 48);
      ++exact;
    }
    ++ran;
  }
  EXPECT_GT(ran, 0) << "no general-position stress cases executed";
  EXPECT_GT(exact, 0) << "no exact graph matches; loosen filters or add seeds";
}

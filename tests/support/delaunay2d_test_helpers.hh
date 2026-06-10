// Shared helpers for Delaunay / Voronoi gtests.
#pragma once

#include "lib/delaunay2d.hh"
#include "lib/delaunay2d_bruteforce.hh"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <set>
#include <string>
#include <vector>

namespace delaunay2d_test {

constexpr int kStressSeeds = 8;
constexpr int kStressMinN = 50;
constexpr int kStressMaxN = 100;

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

  long long coord() {
    return rand_int(-500, 500);
  }

  std::vector<delaunay2d::Pt> random_points(int n) {
    std::vector<delaunay2d::Pt> pts;
    pts.reserve(n);
    for (int i = 0; i < n; ++i) {
      pts.push_back({coord(), coord()});
    }
    return pts;
  }
};

inline bool has_duplicate_sites(const std::vector<delaunay2d::Pt>& pts) {
  std::vector<delaunay2d::Pt> sorted = pts;
  std::sort(sorted.begin(), sorted.end());
  for (size_t i = 1; i < sorted.size(); ++i) {
    if (sorted[i] == sorted[i - 1]) {
      return true;
    }
  }
  return false;
}

inline bool on_circumcircle(const delaunay2d::Pt& p, const delaunay2d::Pt& a,
                            const delaunay2d::Pt& b, const delaunay2d::Pt& c) {
  if (delaunay2d::orient128(a, b, c) == 0) {
    return false;
  }
  return delaunay2d::incircle128(p, a, b, c) == 0;
}

inline bool has_cocircular_quadruple(const std::vector<delaunay2d::Pt>& pts) {
  const int n = static_cast<int>(pts.size());
  for (int a = 0; a < n; ++a) {
    for (int b = a + 1; b < n; ++b) {
      for (int c = b + 1; c < n; ++c) {
        if (delaunay2d::orient128(pts[a], pts[b], pts[c]) == 0) {
          continue;
        }
        for (int d = c + 1; d < n; ++d) {
          if (on_circumcircle(pts[d], pts[a], pts[b], pts[c]) ||
              on_circumcircle(pts[c], pts[a], pts[b], pts[d]) ||
              on_circumcircle(pts[b], pts[a], pts[d], pts[c]) ||
              on_circumcircle(pts[a], pts[b], pts[c], pts[d])) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

inline void expect_delaunay_edges_match(const std::vector<delaunay2d::Pt>& pts) {
  if (pts.empty()) {
    EXPECT_TRUE(delaunay2d::brute_delaunay_edges(pts).empty());
    return;
  }
  std::set<std::pair<int, int>> fast_e =
      delaunay2d::delaunayEdges(delaunay2d::delaunay(std::vector<delaunay2d::Pt>(pts)));
  std::set<std::pair<int, int>> slow_e = delaunay2d::brute_delaunay_edges(pts);
  EXPECT_EQ(fast_e, slow_e);
}

inline void
expect_voronoi_adjacency_match(const std::vector<delaunay2d::Pt>& pts,
                               const std::set<std::pair<int, int>>* slow_edges = nullptr) {
  if (pts.empty()) {
    EXPECT_TRUE(delaunay2d::brute_voronoi_adjacency(pts).empty());
    return;
  }
  std::vector<std::set<int>> fast_adj = delaunay2d::fast_voronoi_adjacency(pts);
  std::set<std::pair<int, int>> slow_e =
      slow_edges ? *slow_edges : delaunay2d::brute_delaunay_edges(pts);
  std::vector<std::set<int>> slow_adj(pts.size());
  for (const auto& [i, j] : slow_e) {
    slow_adj[i].insert(j);
    slow_adj[j].insert(i);
  }
  EXPECT_EQ(delaunay2d::adjacency_edges(fast_adj), delaunay2d::adjacency_edges(slow_adj));
}

inline std::set<std::pair<int, int>> bowyer_watson_edges(const std::vector<delaunay2d::Pt>& pts) {
  return delaunay2d::delaunayEdges(delaunay2d::delaunay(std::vector<delaunay2d::Pt>(pts)));
}

// Validates BW triangulation against the brute Delaunay graph.  When the point
// set is in general position, edge sets are equal; near-cocircular inputs may
// yield extra graph edges that no single triangulation includes.
inline void expect_delaunay_triangulation_valid(const std::vector<delaunay2d::Pt>& pts) {
  std::set<std::pair<int, int>> fast_e = bowyer_watson_edges(pts);
  std::set<std::pair<int, int>> slow_e = delaunay2d::brute_delaunay_edges(pts);

  for (const auto& [i, j] : fast_e) {
    EXPECT_TRUE(delaunay2d::is_delaunay_edge(i, j, pts))
        << "triangulation edge (" << i << "," << j << ") not Delaunay";
    EXPECT_TRUE(slow_e.count({i, j}))
        << "triangulation edge (" << i << "," << j << ") missing from brute graph";
  }
  EXPECT_LE(fast_e.size(), slow_e.size());
}

inline bool delaunay_graphs_equal(const std::vector<delaunay2d::Pt>& pts) {
  return bowyer_watson_edges(pts) == delaunay2d::brute_delaunay_edges(pts);
}

inline void expect_delaunay_and_voronoi_match(const std::vector<delaunay2d::Pt>& pts) {
  std::set<std::pair<int, int>> slow_e = delaunay2d::brute_delaunay_edges(pts);
  std::set<std::pair<int, int>> fast_e = bowyer_watson_edges(pts);
  EXPECT_EQ(fast_e, slow_e);
  expect_voronoi_adjacency_match(pts, &slow_e);
}

inline void expect_voronoi_midpoints_nearest(const std::vector<delaunay2d::Pt>& pts,
                                             int max_edges = 32) {
  if (pts.size() < 2) {
    return;
  }
  std::vector<delaunay2d::Tri> tris = delaunay2d::delaunay(std::vector<delaunay2d::Pt>(pts));
  std::vector<delaunay2d::VEdge> vedges = delaunay2d::buildVoronoi(pts, tris);
  int checked = 0;
  for (const delaunay2d::VEdge& e : vedges) {
    if (checked >= max_edges) {
      break;
    }
    ++checked;
    delaunay2d::LdP mid{(e.a.x + e.b.x) / 2, (e.a.y + e.b.y) / 2};
    int near_mid = delaunay2d::brute_nearest_site(mid, pts);
    delaunay2d::ld da = delaunay2d::dist2L(mid, pts[near_mid]);
    for (size_t i = 0; i < pts.size(); ++i) {
      EXPECT_GE(delaunay2d::dist2L(mid, pts[i]) + 1e-6L, da);
    }
  }
}

} // namespace delaunay2d_test

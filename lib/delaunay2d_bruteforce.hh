// Brute-force Delaunay triangulation and Voronoi adjacency (2D, integer sites).
// Used by tests/delaunay2d_test.cc (Google Test) to validate lib/delaunay2d.hh.
#pragma once

#include "delaunay2d.hh"

#include <set>
#include <vector>

namespace delaunay2d {

inline bool is_delaunay_triangle(int a, int b, int c, const std::vector<Pt>& pts) {
  if (orient128(pts[a], pts[b], pts[c]) == 0) {
    return false;
  }
  for (size_t p = 0; p < pts.size(); ++p) {
    if ((int)p == a || (int)p == b || (int)p == c) {
      continue;
    }
    if (inCircumcircle(pts[p], pts[a], pts[b], pts[c])) {
      return false;
    }
  }
  return true;
}

/// True iff some empty circumcircle supports undirected edge {i, j}.
inline bool is_delaunay_edge(int i, int j, const std::vector<Pt>& pts) {
  const int n = (int)pts.size();
  for (int k = 0; k < n; ++k) {
    if (k == i || k == j) {
      continue;
    }
    if (is_delaunay_triangle(i, j, k, pts) || is_delaunay_triangle(i, k, j, pts) ||
        is_delaunay_triangle(k, i, j, pts)) {
      return true;
    }
  }
  return false;
}

/// O(n^3) Delaunay: all triangles with empty circumcircle (non-unique if cocircular).
inline std::vector<Tri> brute_delaunay(const std::vector<Pt>& pts) {
  const int n = (int)pts.size();
  std::vector<Tri> tris;
  for (int a = 0; a < n; ++a) {
    for (int b = a + 1; b < n; ++b) {
      for (int c = b + 1; c < n; ++c) {
        if (is_delaunay_triangle(a, b, c, pts)) {
          tris.push_back({a, b, c});
        }
      }
    }
  }
  return tris;
}

/// Combinatorial Delaunay graph: edges of all empty-circumcircle triangles.
inline std::set<std::pair<int, int>> brute_delaunay_edges(const std::vector<Pt>& pts) {
  return delaunayEdges(brute_delaunay(pts));
}

/// Voronoi adjacency (Delaunay dual): neighbors of each site.
inline std::vector<std::set<int>> brute_voronoi_adjacency(const std::vector<Pt>& pts) {
  const int n = (int)pts.size();
  std::vector<std::set<int>> adj(n);
  for (const auto& [i, j] : brute_delaunay_edges(pts)) {
    adj[i].insert(j);
    adj[j].insert(i);
  }
  return adj;
}

inline std::set<std::pair<int, int>> adjacency_edges(const std::vector<std::set<int>>& adj) {
  std::set<std::pair<int, int>> edges;
  for (int i = 0; i < (int)adj.size(); ++i) {
    for (int j : adj[i]) {
      if (i < j) {
        edges.insert({i, j});
      }
    }
  }
  return edges;
}

inline std::vector<std::set<int>> fast_voronoi_adjacency(const std::vector<Pt>& pts) {
  const int n = (int)pts.size();
  std::vector<std::set<int>> adj(n);
  std::set<std::pair<int, int>> edges = delaunayEdges(delaunay(std::vector<Pt>(pts)));
  for (const auto& [i, j] : edges) {
    adj[i].insert(j);
    adj[j].insert(i);
  }
  return adj;
}

/// Brute O(n) nearest-site query at floating point q.
inline int brute_nearest_site(const LdP& q, const std::vector<Pt>& pts) {
  int best = 0;
  ld best_d = dist2L(q, pts[0]);
  for (int i = 1; i < (int)pts.size(); ++i) {
    ld d = dist2L(q, pts[i]);
    if (d < best_d) {
      best_d = d;
      best = i;
    }
  }
  return best;
}

} // namespace delaunay2d

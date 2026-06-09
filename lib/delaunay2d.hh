// Bowyer–Watson Delaunay triangulation and Voronoi dual (2D, integer sites).
#pragma once

#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <utility>
#include <vector>

namespace delaunay2d {

using ld = long double;
using i128 = __int128_t;

struct Pt {
  long long x, y;
  bool operator<(const Pt& o) const {
    return x < o.x || (x == o.x && y < o.y);
  }
  bool operator==(const Pt& o) const {
    return x == o.x && y == o.y;
  }
};

struct Tri {
  int a, b, c;
};

struct LdP {
  ld x, y;
};

inline i128 orient128(const Pt& a, const Pt& b, const Pt& c) {
  return (i128)(b.x - a.x) * (c.y - a.y) - (i128)(b.y - a.y) * (c.x - a.x);
}

inline i128 incircle128(const Pt& p, const Pt& a, const Pt& b, const Pt& c) {
  i128 ax = a.x - p.x, ay = a.y - p.y;
  i128 bx = b.x - p.x, by = b.y - p.y;
  i128 cx = c.x - p.x, cy = c.y - p.y;
  i128 nax = ax * ax + ay * ay;
  i128 nbx = bx * bx + by * by;
  i128 ncx = cx * cx + cy * cy;
  return ax * (by * ncx - cy * nbx) - ay * (bx * ncx - cx * nbx) + nax * (bx * cy - by * cx);
}

inline bool inCircumcircle(const Pt& p, const Pt& a, const Pt& b, const Pt& c) {
  i128 o = orient128(a, b, c);
  if (o == 0) {
    return false;
  }
  i128 ic = incircle128(p, a, b, c);
  return (o > 0 && ic > 0) || (o < 0 && ic < 0);
}

inline std::vector<Tri> delaunay(std::vector<Pt> pts) {
  int n = (int)pts.size();
  long long mx = pts[0].x, Mx = mx, my = pts[0].y, My = my;
  for (const Pt& p : pts) {
    mx = std::min(mx, p.x);
    Mx = std::max(Mx, p.x);
    my = std::min(my, p.y);
    My = std::max(My, p.y);
  }
  long long d = std::max(Mx - mx, My - my) * 20 + 10;
  int s0 = n, s1 = n + 1, s2 = n + 2;
  pts.push_back({mx - d, my - d});
  pts.push_back({Mx + 3 * d, my - d});
  pts.push_back({mx + d, My + 3 * d});

  std::vector<Tri> tris = {{s0, s1, s2}};
  for (int i = 0; i < n; ++i) {
    std::vector<std::pair<int, int>> edges;
    std::vector<char> dead(tris.size());
    for (size_t t = 0; t < tris.size(); ++t) {
      const Tri& tr = tris[t];
      if (inCircumcircle(pts[i], pts[tr.a], pts[tr.b], pts[tr.c])) {
        dead[t] = 1;
        int e[3][2] = {{tr.a, tr.b}, {tr.b, tr.c}, {tr.c, tr.a}};
        for (auto& ed : e) {
          if (ed[0] > ed[1]) {
            std::swap(ed[0], ed[1]);
          }
          edges.push_back({ed[0], ed[1]});
        }
      }
    }
    std::sort(edges.begin(), edges.end());
    std::vector<std::pair<int, int>> boundary;
    for (size_t j = 0; j < edges.size();) {
      size_t k = j + 1;
      while (k < edges.size() && edges[k] == edges[j]) {
        ++k;
      }
      if (k - j == 1) {
        boundary.push_back(edges[j]);
      }
      j = k;
    }
    std::vector<Tri> nxt;
    for (size_t t = 0; t < tris.size(); ++t) {
      if (!dead[t]) {
        nxt.push_back(tris[t]);
      }
    }
    for (const auto& [u, v] : boundary) {
      nxt.push_back({i, u, v});
    }
    tris.swap(nxt);
  }

  std::vector<Tri> out;
  for (const Tri& tr : tris) {
    if (tr.a < n && tr.b < n && tr.c < n) {
      out.push_back(tr);
    }
  }
  return out;
}

inline std::set<std::pair<int, int>> delaunayEdges(const std::vector<Tri>& tris) {
  std::set<std::pair<int, int>> edges;
  for (const Tri& tr : tris) {
    int e[3][2] = {{tr.a, tr.b}, {tr.b, tr.c}, {tr.c, tr.a}};
    for (auto& ed : e) {
      if (ed[0] > ed[1]) {
        std::swap(ed[0], ed[1]);
      }
      edges.insert({ed[0], ed[1]});
    }
  }
  return edges;
}

inline LdP circumcenter(const Pt& a, const Pt& b, const Pt& c) {
  ld ax = a.x, ay = a.y, bx = b.x, by = b.y, cx = c.x, cy = c.y;
  ld d = 2 * ((bx - ax) * (cy - ay) - (by - ay) * (cx - ax));
  ld na = ax * ax + ay * ay;
  ld nb = bx * bx + by * by;
  ld nc = cx * cx + cy * cy;
  return {(ay * (nc - nb) + by * (na - nc) + cy * (nb - na)) / d,
          (ax * (nb - nc) + bx * (nc - na) + cx * (na - nb)) / d};
}

struct VEdge {
  LdP a, b;
};

inline ld crossL(const LdP& o, const LdP& a, const LdP& b) {
  return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

inline ld dotL(const LdP& a, const LdP& b) {
  return a.x * b.x + a.y * b.y;
}

inline ld dist2L(const LdP& a, const LdP& b) {
  ld dx = a.x - b.x, dy = a.y - b.y;
  return dx * dx + dy * dy;
}

inline ld dist2L(const LdP& a, const Pt& b) {
  ld dx = a.x - b.x, dy = a.y - b.y;
  return dx * dx + dy * dy;
}

inline std::vector<VEdge> buildVoronoi(const std::vector<Pt>& poly, const std::vector<Tri>& tris) {
  std::map<std::pair<int, int>, std::vector<std::pair<LdP, int>>> half;
  for (const Tri& tr : tris) {
    LdP cc = circumcenter(poly[tr.a], poly[tr.b], poly[tr.c]);
    auto add = [&](int i, int j, int opp) {
      if (i > j) {
        std::swap(i, j);
      }
      half[{i, j}].push_back({cc, opp});
    };
    add(tr.a, tr.b, tr.c);
    add(tr.a, tr.c, tr.b);
    add(tr.b, tr.c, tr.a);
  }

  const ld K = 4e6L;
  std::vector<VEdge> edges;
  for (auto& [st, lst] : half) {
    if (lst.size() == 2) {
      edges.push_back({lst[0].first, lst[1].first});
    } else if (lst.size() == 1) {
      LdP x = lst[0].first;
      int u = lst[0].second;
      LdP m{(ld)(poly[st.first].x + poly[st.second].x) / 2,
            (ld)(poly[st.first].y + poly[st.second].y) / 2};
      LdP dir{m.x - x.x, m.y - x.y};
      LdP far1{x.x + K * dir.x, x.y + K * dir.y};
      LdP far2{x.x - K * dir.x, x.y - K * dir.y};
      LdP pu{(ld)poly[u].x, (ld)poly[u].y};
      if (dotL({pu.x - m.x, pu.y - m.y}, {far1.x - m.x, far1.y - m.y}) < 0) {
        edges.push_back({x, far1});
      } else {
        edges.push_back({x, far2});
      }
    }
  }
  return edges;
}

inline bool onSegment(const LdP& a, const LdP& b, const LdP& p) {
  const ld eps = 1e-11L;
  return std::min(a.x, b.x) - eps <= p.x && p.x <= std::max(a.x, b.x) + eps &&
         std::min(a.y, b.y) - eps <= p.y && p.y <= std::max(a.y, b.y) + eps &&
         std::fabsl(crossL(a, b, p)) <= eps * std::max({1.0L, std::fabsl(a.x), std::fabsl(b.x),
                                                        std::fabsl(a.y), std::fabsl(b.y)});
}

inline bool segIntersect(const LdP& p1, const LdP& p2, const LdP& p3, const LdP& p4, LdP& out) {
  ld x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
  ld x3 = p3.x, y3 = p3.y, x4 = p4.x, y4 = p4.y;
  ld a = y2 - y1, b = x1 - x2, c = a * x1 + b * y1;
  ld d = y4 - y3, e = x3 - x4, f = d * x3 + e * y3;
  ld det = a * e - b * d;
  if (std::fabsl(det) > 1e-12L) {
    out = {(e * c - b * f) / det, (a * f - d * c) / det};
    return onSegment(p1, p2, out) && onSegment(p3, p4, out);
  }
  return false;
}

} // namespace delaunay2d

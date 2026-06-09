// ICPC WF 2018 G / Kattis pandapreserve / BOJ 15695 — Panda Preserve
// https://icpc.kattis.com/problems/pandapreserve
//
// Min radius r so disks at every polygon vertex cover the polygon.
// Answer = max_{p in polygon} min_{vertex v} dist(p, v).
// Candidates: Voronoi vertices inside the polygon, and intersections of
// Voronoi edges with polygon edges.

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

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

static i128 orient128(const Pt& a, const Pt& b, const Pt& c) {
  return (i128)(b.x - a.x) * (c.y - a.y) - (i128)(b.y - a.y) * (c.x - a.x);
}

static i128 incircle128(const Pt& p, const Pt& a, const Pt& b, const Pt& c) {
  i128 ax = a.x - p.x, ay = a.y - p.y;
  i128 bx = b.x - p.x, by = b.y - p.y;
  i128 cx = c.x - p.x, cy = c.y - p.y;
  i128 nax = ax * ax + ay * ay;
  i128 nbx = bx * bx + by * by;
  i128 ncx = cx * cx + cy * cy;
  return ax * (by * ncx - cy * nbx) - ay * (bx * ncx - cx * nbx) + nax * (bx * cy - by * cx);
}

static bool inCircumcircle(const Pt& p, const Pt& a, const Pt& b, const Pt& c) {
  i128 o = orient128(a, b, c);
  if (o == 0) {
    return false;
  }
  i128 ic = incircle128(p, a, b, c);
  return (o > 0 && ic > 0) || (o < 0 && ic < 0);
}

struct Tri {
  int a, b, c;
};

static std::vector<Tri> delaunay(std::vector<Pt> pts) {
  int n = (int)pts.size();
  std::vector<int> ord(n);
  for (int i = 0; i < n; ++i) {
    ord[i] = i;
  }
  std::sort(ord.begin(), ord.end(), [&](int i, int j) { return pts[i] < pts[j]; });

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
  for (int ii = 0; ii < n; ++ii) {
    int i = ord[ii];
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

static std::vector<Tri> naiveDelaunay(const std::vector<Pt>& pts) {
  int n = (int)pts.size();
  std::vector<Tri> out;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      for (int k = j + 1; k < n; ++k) {
        if (orient128(pts[i], pts[j], pts[k]) == 0) {
          continue;
        }
        bool ok = true;
        for (int m = 0; m < n; ++m) {
          if (m == i || m == j || m == k) {
            continue;
          }
          if (inCircumcircle(pts[m], pts[i], pts[j], pts[k])) {
            ok = false;
            break;
          }
        }
        if (ok) {
          out.push_back({i, j, k});
        }
      }
    }
  }
  return out;
}

struct LdP {
  ld x, y;
};

static LdP circumcenter(const Pt& a, const Pt& b, const Pt& c) {
  ld ax = a.x, ay = a.y, bx = b.x, by = b.y, cx = c.x, cy = c.y;
  ld d = 2 * ((bx - ax) * (cy - ay) - (by - ay) * (cx - ax));
  ld na = ax * ax + ay * ay;
  ld nb = bx * bx + by * by;
  ld nc = cx * cx + cy * cy;
  return {(ay * (nc - nb) + by * (na - nc) + cy * (nb - na)) / d,
          (ax * (nb - nc) + bx * (nc - na) + cx * (na - nb)) / d};
}

static ld crossL(const LdP& o, const LdP& a, const LdP& b) {
  return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

static ld dotL(const LdP& a, const LdP& b) {
  return a.x * b.x + a.y * b.y;
}

static ld dist2L(const LdP& a, const LdP& b) {
  ld dx = a.x - b.x, dy = a.y - b.y;
  return dx * dx + dy * dy;
}

static ld dist2L(const LdP& a, const Pt& b) {
  ld dx = a.x - b.x, dy = a.y - b.y;
  return dx * dx + dy * dy;
}

struct VEdge {
  LdP a, b;
};

static std::vector<VEdge> buildVoronoi(const std::vector<Pt>& poly, const std::vector<Tri>& tris) {
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

static bool onSegment(const LdP& a, const LdP& b, const LdP& p) {
  const ld eps = 1e-11L;
  return std::min(a.x, b.x) - eps <= p.x && p.x <= std::max(a.x, b.x) + eps &&
         std::min(a.y, b.y) - eps <= p.y && p.y <= std::max(a.y, b.y) + eps &&
         std::fabsl(crossL(a, b, p)) <= eps * std::max({1.0L, std::fabsl(a.x), std::fabsl(b.x),
                                                        std::fabsl(a.y), std::fabsl(b.y)});
}

static bool segIntersect(const LdP& p1, const LdP& p2, const LdP& p3, const LdP& p4, LdP& out) {
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

static int pointInPoly(const LdP& p, const std::vector<Pt>& poly) {
  int n = (int)poly.size();
  int inside = 0;
  const ld eps = 1e-9L;
  for (int i = 0; i < n; ++i) {
    ld ax = poly[i].x - p.x, ay = poly[i].y - p.y;
    ld bx = poly[(i + 1) % n].x - p.x, by = poly[(i + 1) % n].y - p.y;
    ld cr = ax * by - ay * bx;
    ld scale = std::max({1.0L, std::fabsl(ax), std::fabsl(bx), std::fabsl(ay), std::fabsl(by)});
    if (std::fabsl(cr) <= eps * scale && ax * bx + ay * by <= eps * scale) {
      return 1;
    }
    if (ay > by) {
      std::swap(ax, bx);
      std::swap(ay, by);
    }
    if (ay <= 0 && by > 0 && ax * by < ay * bx) {
      inside ^= 1;
    }
  }
  return inside;
}

static ld minDist2(const LdP& q, const std::vector<Pt>& poly) {
  ld best = 1e300L;
  for (const Pt& v : poly) {
    best = std::min(best, dist2L(q, v));
  }
  return best;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n;
  std::cin >> n;
  std::vector<Pt> poly(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> poly[i].x >> poly[i].y;
  }

  auto tris = delaunay(poly);
  if (tris.empty() && n <= 400) {
    tris = naiveDelaunay(poly);
  }
  auto voro = buildVoronoi(poly, tris);

  std::vector<std::pair<LdP, LdP>> polyEdges;
  for (int i = 0; i < n; ++i) {
    polyEdges.push_back(
        {{(ld)poly[i].x, (ld)poly[i].y}, {(ld)poly[(i + 1) % n].x, (ld)poly[(i + 1) % n].y}});
  }

  ld best = 0;
  auto consider = [&](const LdP& q) {
    if (pointInPoly(q, poly)) {
      best = std::max(best, minDist2(q, poly));
    }
  };

  for (const Tri& tr : tris) {
    consider(circumcenter(poly[tr.a], poly[tr.b], poly[tr.c]));
  }

  LdP tmp;
  for (const VEdge& e : voro) {
    consider(e.a);
    consider(e.b);
    for (const auto& [p3, p4] : polyEdges) {
      if (segIntersect(e.a, e.b, p3, p4, tmp)) {
        consider(tmp);
      }
    }
  }

  // Bisector of each Delaunay edge vs polygon edges (guards clipped/infinite rays).
  std::set<std::pair<int, int>> delEdges;
  for (const Tri& tr : tris) {
    int e[3][2] = {{tr.a, tr.b}, {tr.b, tr.c}, {tr.c, tr.a}};
    for (auto& ed : e) {
      if (ed[0] > ed[1]) {
        std::swap(ed[0], ed[1]);
      }
      delEdges.insert({ed[0], ed[1]});
    }
  }
  for (const auto& [i, j] : delEdges) {
    LdP pi{(ld)poly[i].x, (ld)poly[i].y};
    LdP pj{(ld)poly[j].x, (ld)poly[j].y};
    LdP mid{(pi.x + pj.x) / 2, (pi.y + pj.y) / 2};
    LdP dir{-(pj.y - pi.y), pj.x - pi.x};
    for (const auto& [s1, s2] : polyEdges) {
      ld dx = s2.x - s1.x, dy = s2.y - s1.y;
      ld det = dir.x * dy - dir.y * dx;
      if (std::fabsl(det) < 1e-12L) {
        continue;
      }
      ld sx = s1.x - mid.x, sy = s1.y - mid.y;
      ld u = (sx * dir.y - sy * dir.x) / det;
      if (u >= -1e-12L && u <= 1 + 1e-12L) {
        ld t = (sx * dy - sy * dx) / det;
        consider({mid.x + t * dir.x, mid.y + t * dir.y});
      }
    }
  }

  if (tris.empty()) {
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        LdP pi{(ld)poly[i].x, (ld)poly[i].y};
        LdP pj{(ld)poly[j].x, (ld)poly[j].y};
        LdP mid{(pi.x + pj.x) / 2, (pi.y + pj.y) / 2};
        LdP dir{-(pj.y - pi.y), pj.x - pi.x};
        for (const auto& [s1, s2] : polyEdges) {
          ld dx = s2.x - s1.x, dy = s2.y - s1.y;
          ld det = dir.x * dy - dir.y * dx;
          if (std::fabsl(det) < 1e-12L) {
            continue;
          }
          ld sx = s1.x - mid.x, sy = s1.y - mid.y;
          ld u = (sx * dir.y - sy * dir.x) / det;
          if (u >= -1e-12L && u <= 1 + 1e-12L) {
            ld t = (sx * dy - sy * dx) / det;
            consider({mid.x + t * dir.x, mid.y + t * dir.y});
          }
        }
      }
    }
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        for (int k = j + 1; k < n; ++k) {
          if (orient128(poly[i], poly[j], poly[k]) == 0) {
            continue;
          }
          consider(circumcenter(poly[i], poly[j], poly[k]));
        }
      }
    }
  }

  std::cout << std::fixed << std::setprecision(12) << sqrtl(best) << '\n';
  return 0;
}

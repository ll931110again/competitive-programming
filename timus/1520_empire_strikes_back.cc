// Timus 1520 — Empire Strikes Back
// https://acm.timus.ru/problem.aspx?num=1520
//
// Min bomb radius r so disks of radius r at every factory cover the republic
// (disk of radius R).  Answer = max_{|p|<=R} min_i dist(p, factory_i).

#include "../lib/delaunay2d.hh"
#include <bits/stdc++.h>
using namespace std;

using namespace delaunay2d;

namespace {

bool in_disk(const LdP& p, ld R) {
  return p.x * p.x + p.y * p.y <= R * R + 1e-9L;
}

ld min_dist2(const LdP& q, const std::vector<Pt>& pts) {
  ld best = 1e300L;
  for (const Pt& v : pts) {
    best = std::min(best, dist2_l(q, v));
  }
  return best;
}

std::vector<std::pair<LdP, LdP>> circle_segs(ld R) {
  // Approximate circle by many segments for intersection tests.
  std::vector<std::pair<LdP, LdP>> segs;
  const int steps = 360;
  const ld pi = acosl(-1.0L);
  LdP prev{R, 0};
  for (int i = 1; i <= steps; ++i) {
    ld a = 2 * pi * i / steps;
    LdP cur{R * cosl(a), R * sinl(a)};
    segs.push_back({prev, cur});
    prev = cur;
  }
  return segs;
}

} // namespace

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n;
  ld R;
  std::cin >> n >> R;
  std::vector<Pt> pts(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> pts[i].x >> pts[i].y;
  }

  auto tris = delaunay(pts);
  auto voro = build_voronoi(pts, tris);
  auto boundary = circle_segs(R);

  ld best = 0;
  auto consider = [&](const LdP& q) {
    if (in_disk(q, R)) {
      best = std::max(best, min_dist2(q, pts));
    }
  };

  for (const Tri& tr : tris) {
    consider(circumcenter(pts[tr.a], pts[tr.b], pts[tr.c]));
  }

  LdP tmp;
  for (const VEdge& e : voro) {
    consider(e.a);
    consider(e.b);
    for (const auto& [p3, p4] : boundary) {
      if (seg_intersect(e.a, e.b, p3, p4, tmp)) {
        consider(tmp);
      }
    }
  }

  std::set<std::pair<int, int>> del_edges;
  for (const Tri& tr : tris) {
    int ed[3][2] = {{tr.a, tr.b}, {tr.b, tr.c}, {tr.c, tr.a}};
    for (auto& e : ed) {
      if (e[0] > e[1]) {
        std::swap(e[0], e[1]);
      }
      del_edges.insert({e[0], e[1]});
    }
  }
  for (const auto& [i, j] : del_edges) {
    LdP pi{(ld)pts[i].x, (ld)pts[i].y};
    LdP pj{(ld)pts[j].x, (ld)pts[j].y};
    LdP mid{(pi.x + pj.x) / 2, (pi.y + pj.y) / 2};
    LdP dir{-(pj.y - pi.y), pj.x - pi.x};
    for (const auto& [s1, s2] : boundary) {
      ld dx = s2.x - s1.x, dy = s2.y - s1.y;
      ld det = dir.x * dy - dir.y * dx;
      if (std::fabsl(det) < 1e-12L) {
        continue;
      }
      ld t = ((mid.x - s1.x) * dy - (mid.y - s1.y) * dx) / det;
      ld u = ((mid.x - s1.x) * dir.y - (mid.y - s1.y) * dir.x) / det;
      if (u >= -1e-12L && u <= 1 + 1e-12L) {
        consider({mid.x + t * dir.x, mid.y + t * dir.y});
      }
    }
  }

  std::cout << std::fixed << std::setprecision(5) << sqrtl(best) << '\n';
  return 0;
}

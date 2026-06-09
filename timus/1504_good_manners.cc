// Timus 1504 — Good Manners
// https://acm.timus.ru/problem.aspx?num=1504
//
// Pick a point on a circle of radius R maximizing the weight of the nearest cake.
// The Voronoi diagram splits the circle into O(K) arcs; check one point per arc.

#include "../lib/delaunay2d.hh"
#include <bits/stdc++.h>
using namespace std;

using namespace delaunay2d;

namespace {

const ld SCALE = 1000000L;

struct Cake {
  ld x, y;
  int w;
};

int nearest_weight(const LdP& q, const vector<Cake>& cakes) {
  ld best_d = 1e300L;
  int best_w = 0;
  const ld eps = 1e-9L;
  for (const Cake& c : cakes) {
    ld dx = q.x - c.x, dy = q.y - c.y;
    ld d = dx * dx + dy * dy;
    if (d + eps < best_d) {
      best_d = d;
      best_w = c.w;
    } else if (fabsl(d - best_d) <= eps * max(1.0L, best_d)) {
      best_w = max(best_w, c.w);
    }
  }
  return best_w;
}

vector<pair<ld, ld>> circle_line(const LdP& a, const LdP& b, ld R) {
  // Intersection of segment a-b with circle x^2+y^2=R^2; return points on segment.
  ld dx = b.x - a.x, dy = b.y - a.y;
  ld A = dx * dx + dy * dy;
  ld B = 2 * (a.x * dx + a.y * dy);
  ld C = a.x * a.x + a.y * a.y - R * R;
  vector<pair<ld, ld>> out;
  if (fabsl(A) < 1e-18L) {
    if (fabsl(C) <= 1e-12L) {
      out.push_back({a.x, a.y});
    }
    return out;
  }
  ld D = B * B - 4 * A * C;
  if (D < -1e-12L) {
    return out;
  }
  D = max(D, 0.0L);
  ld s_d = sqrtl(D);
  for (ld t : {(-B - s_d) / (2 * A), (-B + s_d) / (2 * A)}) {
    if (t >= -1e-12L && t <= 1 + 1e-12L) {
      out.push_back({a.x + t * dx, a.y + t * dy});
    }
  }
  return out;
}

ld norm_angle(ld a) {
  const ld pi = acosl(-1.0L);
  const ld two = 2 * pi;
  while (a < 0) {
    a += two;
  }
  while (a >= two) {
    a -= two;
  }
  return a;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ld R;
  int K;
  cin >> R >> K;
  vector<Cake> cakes(K);
  vector<Pt> sites(K);
  for (int i = 0; i < K; ++i) {
    cin >> cakes[i].x >> cakes[i].y >> cakes[i].w;
    sites[i].x = llround(cakes[i].x * SCALE);
    sites[i].y = llround(cakes[i].y * SCALE);
  }

  auto tris = delaunay(sites);
  auto voro = build_voronoi(sites, tris);

  vector<ld> angles;
  auto add_angle = [&](ld x, ld y) {
    if (fabsl(x * x + y * y - R * R) > 1e-6L * max(1.0L, R * R)) {
      return;
    }
    angles.push_back(norm_angle(atan2l(y, x)));
  };

  LdP tmp;
  for (const VEdge& e : voro) {
    for (const auto& [x, y] : circle_line(e.a, e.b, R)) {
      add_angle(x, y);
    }
  }

  set<pair<int, int>> del_edges;
  for (const Tri& tr : tris) {
    int ed[3][2] = {{tr.a, tr.b}, {tr.b, tr.c}, {tr.c, tr.a}};
    for (auto& e : ed) {
      if (e[0] > e[1]) {
        swap(e[0], e[1]);
      }
      del_edges.insert({e[0], e[1]});
    }
  }
  for (const auto& [i, j] : del_edges) {
    LdP pi{cakes[i].x, cakes[i].y};
    LdP pj{cakes[j].x, cakes[j].y};
    LdP mid{(pi.x + pj.x) / 2, (pi.y + pj.y) / 2};
    LdP dir{-(pj.y - pi.y), pj.x - pi.x};
    LdP far1{mid.x + 1e7L * dir.x, mid.y + 1e7L * dir.y};
    for (const auto& [x, y] : circle_line(mid, far1, R)) {
      add_angle(x, y);
    }
    LdP far2{mid.x - 1e7L * dir.x, mid.y - 1e7L * dir.y};
    for (const auto& [x, y] : circle_line(mid, far2, R)) {
      add_angle(x, y);
    }
  }

  if (angles.empty()) {
    angles.push_back(0);
  }
  sort(angles.begin(), angles.end());
  angles.erase(
      unique(angles.begin(), angles.end(), [](ld a, ld b) { return fabsl(a - b) < 1e-12L; }),
      angles.end());

  int best_w = -1;
  LdP best_p{0, 0};
  const ld pi = acosl(-1.0L);
  int m = (int)angles.size();
  for (int i = 0; i < m; ++i) {
    ld a1 = angles[i];
    ld a2 = (i + 1 < m) ? angles[i + 1] : angles[0] + 2 * pi;
    ld mid = (a1 + a2) / 2;
    LdP q{R * cosl(mid), R * sinl(mid)};
    int w = nearest_weight(q, cakes);
    if (w > best_w) {
      best_w = w;
      best_p = q;
    }
  }

  cout << fixed << setprecision(7) << best_p.x << ' ' << best_p.y << '\n';
  return 0;
}

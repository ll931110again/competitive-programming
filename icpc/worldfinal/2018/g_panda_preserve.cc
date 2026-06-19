// ICPC WF 2018 G / Kattis pandapreserve / BOJ 15695 — Panda Preserve
// https://icpc.kattis.com/problems/pandapreserve
//
// Min radius r so disks at every polygon vertex cover the polygon.
// Answer = max_{p in polygon} min_{vertex v} dist(p, v).
// Candidates: Delaunay circumcenters inside the polygon, and clipped Voronoi
// edge segments (not infinite bisectors) intersecting the polygon boundary.

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <utility>
#include <vector>

using namespace std;

using ld = double;
using i64 = long long;
using i128 = __int128_t;

namespace {

struct Pt {
  i64 x, y;
  bool operator<(const Pt& o) const {
    return x < o.x || (x == o.x && y < o.y);
  }
  bool operator==(const Pt& o) const {
    return x == o.x && y == o.y;
  }
  i64 cross(const Pt& a, const Pt& b) const {
    return (a.x - x) * (b.y - y) - (a.y - y) * (b.x - x);
  }
  i64 sqr_len() const {
    return x * x + y * y;
  }
};

struct Tri {
  int a, b, c;
};

i128 cross_i(const Pt& o, const Pt& a, const Pt& b) {
  return (i128)(a.x - o.x) * (b.y - o.y) - (i128)(a.y - o.y) * (b.x - o.x);
}

i128 dist2_i(const Pt& a, const Pt& b) {
  i128 dx = a.x - b.x, dy = a.y - b.y;
  return dx * dx + dy * dy;
}

bool in_circumcircle(const Pt& p, const Pt& a, const Pt& b, const Pt& c) {
  i128 o = cross_i(a, b, c);
  if (o == 0) {
    return false;
  }
  i128 val = dist2_i(p, a) * cross_i(b, c, p) + dist2_i(p, b) * cross_i(c, a, p) +
             dist2_i(p, c) * cross_i(a, b, p);
  return o * val > 0;
}

vector<Tri> delaunay_bw(vector<Pt> pts) {
  int n = (int)pts.size();
  i64 mx = pts[0].x, Mx = mx, my = pts[0].y, My = my;
  for (const Pt& p : pts) {
    mx = min(mx, p.x);
    Mx = max(Mx, p.x);
    my = min(my, p.y);
    My = max(My, p.y);
  }
  i64 d = max(Mx - mx, My - my) * 20 + 10;
  int s0 = n, s1 = n + 1, s2 = n + 2;
  pts.push_back({mx - d, my - d});
  pts.push_back({Mx + 3 * d, my - d});
  pts.push_back({mx + d, My + 3 * d});

  vector<Tri> tris = {{s0, s1, s2}};
  vector<int> ord(n);
  iota(ord.begin(), ord.end(), 0);
  sort(ord.begin(), ord.end(), [&](int i, int j) { return pts[i] < pts[j]; });

  vector<pair<int, int>> edges;
  vector<char> dead;
  for (int ii = 0; ii < n; ++ii) {
    int i = ord[ii];
    edges.clear();
    dead.assign(tris.size(), 0);
    for (size_t t = 0; t < tris.size(); ++t) {
      const Tri& tr = tris[t];
      if (in_circumcircle(pts[i], pts[tr.a], pts[tr.b], pts[tr.c])) {
        dead[t] = 1;
        int e[3][2] = {{tr.a, tr.b}, {tr.b, tr.c}, {tr.c, tr.a}};
        for (auto& ed : e) {
          if (ed[0] > ed[1]) {
            swap(ed[0], ed[1]);
          }
          edges.push_back({ed[0], ed[1]});
        }
      }
    }
    sort(edges.begin(), edges.end());
    vector<pair<int, int>> boundary;
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
    vector<Tri> nxt;
    nxt.reserve(tris.size() + boundary.size());
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

  vector<Tri> out;
  out.reserve(tris.size());
  for (const Tri& tr : tris) {
    if (tr.a < n && tr.b < n && tr.c < n) {
      out.push_back(tr);
    }
  }
  return out;
}

// D&C fallback for near-degenerate input (Bowyer–Watson returns no triangles).

const Pt kInfPt = {1000000000000000000LL, 1000000000000000000LL};

struct QuadEdge {
  Pt origin;
  QuadEdge* rot = nullptr;
  QuadEdge* onext = nullptr;
  bool used = false;
  QuadEdge* rev() const {
    return rot->rot;
  }
  QuadEdge* lnext() const {
    return rot->rev()->onext->rot;
  }
  QuadEdge* oprev() const {
    return rot->onext->rot;
  }
  Pt dest() const {
    return rev()->origin;
  }
};

vector<QuadEdge> qpool;

QuadEdge* qalloc() {
  qpool.emplace_back();
  return &qpool.back();
}

QuadEdge* make_edge(Pt from, Pt to) {
  QuadEdge* e1 = qalloc();
  QuadEdge* e2 = qalloc();
  QuadEdge* e3 = qalloc();
  QuadEdge* e4 = qalloc();
  e1->origin = from;
  e2->origin = to;
  e3->origin = e4->origin = kInfPt;
  e1->rot = e3;
  e2->rot = e4;
  e3->rot = e2;
  e4->rot = e1;
  e1->onext = e1;
  e2->onext = e2;
  e3->onext = e4;
  e4->onext = e3;
  return e1;
}

void splice(QuadEdge* a, QuadEdge* b) {
  swap(a->onext->rot->onext, b->onext->rot->onext);
  swap(a->onext, b->onext);
}

void unlink_edge(QuadEdge* e) {
  splice(e, e->oprev());
  splice(e->rev(), e->rev()->oprev());
}

QuadEdge* connect(QuadEdge* a, QuadEdge* b) {
  QuadEdge* e = make_edge(a->dest(), b->origin);
  splice(e, a->lnext());
  splice(e->rev(), b);
  return e;
}

bool left_of(Pt p, QuadEdge* e) {
  return p.cross(e->origin, e->dest()) > 0;
}

bool right_of(Pt p, QuadEdge* e) {
  return p.cross(e->origin, e->dest()) < 0;
}

template <class T> T det3(T a1, T a2, T a3, T b1, T b2, T b3, T c1, T c2, T c3) {
  return a1 * (b2 * c3 - c2 * b3) - a2 * (b1 * c3 - c1 * b3) + a3 * (b1 * c2 - c1 * b2);
}

bool in_circle(Pt a, Pt b, Pt c, Pt d) {
  i128 det = -det3<i128>(b.x, b.y, b.sqr_len(), c.x, c.y, c.sqr_len(), d.x, d.y, d.sqr_len());
  det += det3<i128>(a.x, a.y, a.sqr_len(), c.x, c.y, c.sqr_len(), d.x, d.y, d.sqr_len());
  det -= det3<i128>(a.x, a.y, a.sqr_len(), b.x, b.y, b.sqr_len(), d.x, d.y, d.sqr_len());
  det += det3<i128>(a.x, a.y, a.sqr_len(), b.x, b.y, b.sqr_len(), c.x, c.y, c.sqr_len());
  return det > 0;
}

pair<QuadEdge*, QuadEdge*> build_tr(int l, int r, vector<Pt>& p) {
  if (r - l + 1 == 2) {
    QuadEdge* res = make_edge(p[l], p[r]);
    return {res, res->rev()};
  }
  if (r - l + 1 == 3) {
    QuadEdge *a = make_edge(p[l], p[l + 1]), *b = make_edge(p[l + 1], p[r]);
    splice(a->rev(), b);
    int sg = (p[l].cross(p[l + 1], p[r]) > 0) - (p[l].cross(p[l + 1], p[r]) < 0);
    if (sg == 0) {
      return {a, b->rev()};
    }
    QuadEdge* c = connect(b, a);
    if (sg > 0) {
      return {a, b->rev()};
    }
    return {c->rev(), c};
  }
  int mid = (l + r) / 2;
  QuadEdge *ldo, *ldi, *rdo, *rdi;
  tie(ldo, ldi) = build_tr(l, mid, p);
  tie(rdi, rdo) = build_tr(mid + 1, r, p);
  while (true) {
    if (left_of(rdi->origin, ldi)) {
      ldi = ldi->lnext();
      continue;
    }
    if (right_of(ldi->origin, rdi)) {
      rdi = rdi->rev()->onext;
      continue;
    }
    break;
  }
  QuadEdge* basel = connect(rdi->rev(), ldi);
  auto valid = [&basel](QuadEdge* e) { return right_of(e->dest(), basel); };
  if (ldi->origin == ldo->origin) {
    ldo = basel->rev();
  }
  if (rdi->origin == rdo->origin) {
    rdo = basel;
  }
  while (true) {
    QuadEdge* lcand = basel->rev()->onext;
    if (valid(lcand)) {
      while (in_circle(basel->dest(), basel->origin, lcand->dest(), lcand->onext->dest())) {
        QuadEdge* t = lcand->onext;
        unlink_edge(lcand);
        lcand = t;
      }
    }
    QuadEdge* rcand = basel->oprev();
    if (valid(rcand)) {
      while (in_circle(basel->dest(), basel->origin, rcand->dest(), rcand->oprev()->dest())) {
        QuadEdge* t = rcand->oprev();
        unlink_edge(rcand);
        rcand = t;
      }
    }
    if (!valid(lcand) && !valid(rcand)) {
      break;
    }
    if (!valid(lcand) ||
        (valid(rcand) && in_circle(lcand->dest(), lcand->origin, rcand->origin, rcand->dest()))) {
      basel = connect(rcand, basel->rev());
    } else {
      basel = connect(basel->rev(), lcand->rev());
    }
  }
  return {ldo, rdo};
}

vector<Tri> delaunay_dc(vector<Pt> p) {
  qpool.clear();
  qpool.reserve(24 * p.size());
  sort(p.begin(), p.end());
  auto res = build_tr(0, (int)p.size() - 1, p);
  QuadEdge* e = res.first;
  vector<QuadEdge*> edges = {e};
  while (e->onext->dest().cross(e->dest(), e->origin) < 0) {
    e = e->onext;
  }
  vector<Pt> verts;
  auto add_face = [&]() {
    QuadEdge* curr = e;
    do {
      curr->used = true;
      verts.push_back(curr->origin);
      edges.push_back(curr->rev());
      curr = curr->lnext();
    } while (curr != e);
  };
  add_face();
  verts.clear();
  for (size_t kek = 0; kek < edges.size(); ++kek) {
    e = edges[kek];
    if (!e->used) {
      add_face();
    }
  }
  vector<Tri> out;
  out.reserve(verts.size() / 3);
  for (size_t i = 0; i + 2 < verts.size(); i += 3) {
    const Pt &a = verts[i], &b = verts[i + 1], &c = verts[i + 2];
    int ia = lower_bound(p.begin(), p.end(), a) - p.begin();
    int ib = lower_bound(p.begin(), p.end(), b) - p.begin();
    int ic = lower_bound(p.begin(), p.end(), c) - p.begin();
    out.push_back({ia, ib, ic});
  }
  return out;
}

vector<Tri> delaunay(const vector<Pt>& poly) {
  vector<Tri> tris = delaunay_bw(poly);
  if (!tris.empty()) {
    return tris;
  }
  int n = (int)poly.size();
  vector<int> ord(n);
  iota(ord.begin(), ord.end(), 0);
  sort(ord.begin(), ord.end(), [&](int i, int j) { return poly[i] < poly[j]; });
  vector<Pt> sorted(n);
  for (int i = 0; i < n; ++i) {
    sorted[i] = poly[ord[i]];
  }
  vector<Tri> local = delaunay_dc(sorted);
  vector<Tri> out;
  out.reserve(local.size());
  for (const Tri& tr : local) {
    out.push_back({ord[tr.a], ord[tr.b], ord[tr.c]});
  }
  return out;
}

struct P2 {
  ld x, y;
};

P2 circumcenter(const Pt& a, const Pt& b, const Pt& c) {
  ld ax = a.x, ay = a.y, bx = b.x, by = b.y, cx = c.x, cy = c.y;
  ld d = 2 * ((bx - ax) * (cy - ay) - (by - ay) * (cx - ax));
  ld na = ax * ax + ay * ay;
  ld nb = bx * bx + by * by;
  ld nc = cx * cx + cy * cy;
  return {(ay * (nc - nb) + by * (na - nc) + cy * (nb - na)) / d,
          (ax * (nb - nc) + bx * (nc - na) + cx * (na - nb)) / d};
}

ld dist2(const P2& a, const Pt& b) {
  ld dx = a.x - b.x, dy = a.y - b.y;
  return dx * dx + dy * dy;
}

int point_in_poly(const P2& p, const vector<Pt>& poly) {
  int n = (int)poly.size();
  int inside = 0;
  const ld eps = 1e-9;
  for (int i = 0; i < n; ++i) {
    ld ax = poly[i].x - p.x, ay = poly[i].y - p.y;
    ld bx = poly[(i + 1) % n].x - p.x, by = poly[(i + 1) % n].y - p.y;
    ld cr = ax * by - ay * bx;
    ld scale = max({1.0, fabs(ax), fabs(bx), fabs(ay), fabs(by)});
    if (fabs(cr) <= eps * scale && ax * bx + ay * by <= eps * scale) {
      return 1;
    }
    if (ay > by) {
      swap(ax, bx);
      swap(ay, by);
    }
    if (ay <= 0 && by > 0 && ax * by < ay * bx) {
      inside ^= 1;
    }
  }
  return inside;
}

// Segment (a,b) vs segment (c,d); return true if proper intersection at out.
bool seg_seg(const P2& a, const P2& b, const P2& c, const P2& d, P2& out) {
  ld x1 = a.x, y1 = a.y, x2 = b.x, y2 = b.y;
  ld x3 = c.x, y3 = c.y, x4 = d.x, y4 = d.y;
  ld a1 = y2 - y1, b1 = x1 - x2, c1 = a1 * x1 + b1 * y1;
  ld a2 = y4 - y3, b2 = x3 - x4, c2 = a2 * x3 + b2 * y3;
  ld det = a1 * b2 - b1 * a2;
  if (fabs(det) < 1e-12) {
    return false;
  }
  out = {(b2 * c1 - b1 * c2) / det, (a1 * c2 - a2 * c1) / det};
  const ld eps = 1e-11;
  auto on = [&](ld px, ld py, ld qx, ld qy, ld rx, ld ry) {
    return min(px, qx) - eps <= rx && rx <= max(px, qx) + eps && min(py, qy) - eps <= ry &&
           ry <= max(py, qy) + eps;
  };
  return on(x1, y1, x2, y2, out.x, out.y) && on(x3, y3, x4, y4, out.x, out.y);
}

// On a Voronoi edge between sites i and j, the closest site distance equals dist(q, i).
void consider_voro_point(const P2& q, int i, const vector<Pt>& poly, ld& best) {
  ld d = dist2(q, poly[i]);
  if (d > best) {
    best = d;
  }
}

void consider_voro_segment(const P2& a, const P2& b, int i, const vector<Pt>& poly,
                           const vector<pair<P2, P2>>& poly_edges, ld& best) {
  P2 tmp;
  for (const auto& [c, d] : poly_edges) {
    if (seg_seg(a, b, c, d, tmp)) {
      consider_voro_point(tmp, i, poly, best);
    }
  }
}

void consider_voro_ray(const P2& origin, const P2& far, int i, const vector<Pt>& poly,
                       const vector<pair<P2, P2>>& poly_edges, ld& best) {
  consider_voro_segment(origin, far, i, poly, poly_edges, best);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<Pt> poly(n);
  for (int i = 0; i < n; ++i) {
    cin >> poly[i].x >> poly[i].y;
  }

  auto tris = delaunay(poly);

  vector<pair<P2, P2>> poly_edges(n);
  for (int i = 0; i < n; ++i) {
    poly_edges[i] = {{(ld)poly[i].x, (ld)poly[i].y},
                     {(ld)poly[(i + 1) % n].x, (ld)poly[(i + 1) % n].y}};
  }

  map<pair<int, int>, vector<pair<P2, int>>> half;
  for (const Tri& tr : tris) {
    P2 cc = circumcenter(poly[tr.a], poly[tr.b], poly[tr.c]);
    auto add = [&](int i, int j, int opp) {
      if (i > j) {
        swap(i, j);
      }
      half[{i, j}].push_back({cc, opp});
    };
    add(tr.a, tr.b, tr.c);
    add(tr.a, tr.c, tr.b);
    add(tr.b, tr.c, tr.a);
  }

  ld best = 0;
  for (const Tri& tr : tris) {
    P2 cc = circumcenter(poly[tr.a], poly[tr.b], poly[tr.c]);
    if (point_in_poly(cc, poly)) {
      ld d = dist2(cc, poly[tr.a]);
      if (d > best) {
        best = d;
      }
    }
  }

  ld span = 0;
  for (const Pt& p : poly) {
    span = max(span, ld(max(abs(p.x), abs(p.y))));
  }
  const ld K = max(span * 4, 4e6) + 1e6;

  for (auto& [st, lst] : half) {
    int i = st.first, j = st.second;
    if (lst.size() == 2) {
      consider_voro_segment(lst[0].first, lst[1].first, i, poly, poly_edges, best);
    } else if (lst.size() == 1) {
      P2 x = lst[0].first;
      int u = lst[0].second;
      P2 m{(ld)(poly[i].x + poly[j].x) / 2, (ld)(poly[i].y + poly[j].y) / 2};
      P2 dir{m.x - x.x, m.y - x.y};
      P2 pu{(ld)poly[u].x, (ld)poly[u].y};
      P2 far_pt;
      if ((pu.x - m.x) * (x.x + K * dir.x - m.x) + (pu.y - m.y) * (x.y + K * dir.y - m.y) < 0) {
        far_pt = {x.x + K * dir.x, x.y + K * dir.y};
      } else {
        far_pt = {x.x - K * dir.x, x.y - K * dir.y};
      }
      consider_voro_ray(x, far_pt, i, poly, poly_edges, best);
    }
  }

  cout << fixed << setprecision(12) << sqrt(best) << '\n';
  return 0;
}

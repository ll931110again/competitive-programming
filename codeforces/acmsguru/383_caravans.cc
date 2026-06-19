// SGU 383 / acmsguru 383 — Caravans
// https://codeforces.com/problemsets/acmsguru/problem/99999/383
//
// Optimal caravan path uses only Delaunay edges; the Euclidean MST minimizes
// the maximum edge weight on any path.  Answer per query = max edge on MST path.

#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using ld = long double;

namespace {

bool ge(i64 a, i64 b) {
  return a >= b;
}
bool eq(i64 a, i64 b) {
  return a == b;
}
bool gt(i64 a, i64 b) {
  return a > b;
}
bool lt(i64 a, i64 b) {
  return a < b;
}
int sgn(i64 a) {
  return a > 0 ? 1 : (a < 0 ? -1 : 0);
}

struct Pt {
  i64 x, y;
  int id;
  Pt() = default;
  Pt(i64 x_, i64 y_, int id_) : x(x_), y(y_), id(id_) {}
  Pt operator-(const Pt& p) const {
    return Pt(x - p.x, y - p.y, id);
  }
  i64 cross(const Pt& p) const {
    return x * p.y - y * p.x;
  }
  i64 cross(const Pt& a, const Pt& b) const {
    return (a - *this).cross(b - *this);
  }
  i64 dot(const Pt& p) const {
    return x * p.x + y * p.y;
  }
  i64 sqr_length() const {
    return dot(*this);
  }
  bool operator==(const Pt& p) const {
    return eq(x, p.x) && eq(y, p.y);
  }
};

const Pt inf_pt(1e18, 1e18, -1);

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

struct QuadPool {
  vector<QuadEdge> buf;
  int ptr = 0;

  void reset(int n) {
    buf.clear();
    buf.reserve(64 * n);
    ptr = 0;
  }

  QuadEdge* get() {
    if (ptr >= (int)buf.size()) {
      buf.resize(buf.size() + max(1024, (int)buf.size()));
    }
    return &buf[ptr++];
  }
};

QuadPool pool;

QuadEdge* make_edge(Pt from, Pt to) {
  QuadEdge *e1 = pool.get(), *e2 = pool.get(), *e3 = pool.get(), *e4 = pool.get();
  e1->origin = from;
  e2->origin = to;
  e3->origin = e4->origin = inf_pt;
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

void delete_edge(QuadEdge* e) {
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
  return gt(p.cross(e->origin, e->dest()), 0);
}
bool right_of(Pt p, QuadEdge* e) {
  return lt(p.cross(e->origin, e->dest()), 0);
}

template <class T> static T det3(T a1, T a2, T a3, T b1, T b2, T b3, T c1, T c2, T c3) {
  return a1 * (b2 * c3 - c2 * b3) - a2 * (b1 * c3 - c1 * b3) + a3 * (b1 * c2 - c1 * b2);
}

bool in_circle(Pt a, Pt b, Pt c, Pt d) {
  using i128 = __int128_t;
  i128 det =
      -det3<i128>(b.x, b.y, b.sqr_length(), c.x, c.y, c.sqr_length(), d.x, d.y, d.sqr_length());
  det += det3<i128>(a.x, a.y, a.sqr_length(), c.x, c.y, c.sqr_length(), d.x, d.y, d.sqr_length());
  det -= det3<i128>(a.x, a.y, a.sqr_length(), b.x, b.y, b.sqr_length(), d.x, d.y, d.sqr_length());
  det += det3<i128>(a.x, a.y, a.sqr_length(), b.x, b.y, b.sqr_length(), c.x, c.y, c.sqr_length());
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
    int sg = sgn(p[l].cross(p[l + 1], p[r]));
    if (sg == 0) {
      return {a, b->rev()};
    }
    QuadEdge* c = connect(b, a);
    if (sg == 1) {
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
  auto valid = [&](QuadEdge* e) { return right_of(e->dest(), basel); };
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
        delete_edge(lcand);
        lcand = t;
      }
    }
    QuadEdge* rcand = basel->oprev();
    if (valid(rcand)) {
      while (in_circle(basel->dest(), basel->origin, rcand->dest(), rcand->oprev()->dest())) {
        QuadEdge* t = rcand->oprev();
        delete_edge(rcand);
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

vector<pair<int, int>> delaunay_edges(vector<Pt>& p) {
  pool.reset((int)p.size());
  sort(p.begin(), p.end(),
       [](const Pt& a, const Pt& b) { return lt(a.x, b.x) || (eq(a.x, b.x) && lt(a.y, b.y)); });
  auto res = build_tr(0, (int)p.size() - 1, p);
  QuadEdge* e = res.first;
  vector<QuadEdge*> todo = {e};
  while (lt(e->onext->dest().cross(e->dest(), e->origin), 0)) {
    e = e->onext;
  }

  vector<pair<int, int>> out;
  out.reserve(6 * p.size());
  auto add = [&](QuadEdge* start) {
    QuadEdge* cur = start;
    do {
      cur->used = true;
      int u = cur->origin.id, v = cur->dest().id;
      if (u >= 0 && v >= 0) {
        if (u > v) {
          swap(u, v);
        }
        out.push_back({u, v});
      }
      todo.push_back(cur->rev());
      cur = cur->lnext();
    } while (cur != start);
  };
  add(e);
  for (size_t i = 0; i < todo.size(); ++i) {
    if (!todo[i]->used) {
      add(todo[i]);
    }
  }

  sort(out.begin(), out.end());
  out.erase(unique(out.begin(), out.end()), out.end());
  return out;
}

struct Edge {
  int u, v;
  i64 w2;
  bool operator<(const Edge& o) const {
    return w2 < o.w2;
  }
};

struct Lca {
  int n, lg;
  vector<vector<int>> up;
  vector<vector<ld>> mx;
  vector<int> depth;
  vector<vector<pair<int, ld>>> adj;

  explicit Lca(int n_) : n(n_) {
    lg = 1;
    while ((1 << lg) <= n) {
      ++lg;
    }
    up.assign(lg, vector<int>(n, -1));
    mx.assign(lg, vector<ld>(n, 0));
    depth.assign(n, 0);
    adj.assign(n, {});
  }

  void dfs(int u, int p, ld pw) {
    up[0][u] = p;
    mx[0][u] = pw;
    for (const auto& [v, w] : adj[u]) {
      if (v == p) {
        continue;
      }
      depth[v] = depth[u] + 1;
      dfs(v, u, w);
    }
  }

  void build(int root = 0) {
    dfs(root, -1, 0);
    for (int k = 1; k < lg; ++k) {
      for (int i = 0; i < n; ++i) {
        int mid = up[k - 1][i];
        if (mid == -1) {
          up[k][i] = -1;
          mx[k][i] = mx[k - 1][i];
        } else {
          up[k][i] = up[k - 1][mid];
          mx[k][i] = max(mx[k - 1][i], mx[k - 1][mid]);
        }
      }
    }
  }

  ld query_max(int a, int b) {
    if (a == b) {
      return 0;
    }
    ld ans = 0;
    if (depth[a] < depth[b]) {
      swap(a, b);
    }
    int diff = depth[a] - depth[b];
    for (int k = 0; k < lg; ++k) {
      if (diff >> k & 1) {
        ans = max(ans, mx[k][a]);
        a = up[k][a];
      }
    }
    if (a == b) {
      return ans;
    }
    for (int k = lg - 1; k >= 0; --k) {
      if (up[k][a] != up[k][b]) {
        ans = max({ans, mx[k][a], mx[k][b]});
        a = up[k][a];
        b = up[k][b];
      }
    }
    ans = max({ans, mx[0][a], mx[0][b]});
    return ans;
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<Pt> pts(n);
  for (int i = 0; i < n; ++i) {
    cin >> pts[i].x >> pts[i].y;
    pts[i].id = i;
  }

  auto edges = delaunay_edges(pts);

  vector<Edge> elist;
  elist.reserve(edges.size());
  for (const auto& [u, v] : edges) {
    i64 dx = pts[u].x - pts[v].x;
    i64 dy = pts[u].y - pts[v].y;
    elist.push_back({u, v, dx * dx + dy * dy});
  }
  sort(elist.begin(), elist.end());

  struct Dsu {
    vector<int> p;
    explicit Dsu(int n) : p(n) {
      iota(p.begin(), p.end(), 0);
    }
    int find(int x) {
      return p[x] == x ? x : p[x] = find(p[x]);
    }
    bool unite(int a, int b) {
      a = find(a);
      b = find(b);
      if (a == b) {
        return false;
      }
      p[a] = b;
      return true;
    }
  };

  Lca lca(n);
  Dsu dsu(n);
  int cnt = 0;
  for (const Edge& e : elist) {
    if (!dsu.unite(e.u, e.v)) {
      continue;
    }
    ld w = sqrt((ld)e.w2);
    lca.adj[e.u].push_back({e.v, w});
    lca.adj[e.v].push_back({e.u, w});
    if (++cnt == n - 1) {
      break;
    }
  }
  lca.build(0);

  int q;
  cin >> q;
  while (q--) {
    int s, t;
    cin >> s >> t;
    --s;
    --t;
    cout << setprecision(15) << lca.query_max(s, t) << '\n';
  }
  return 0;
}

// SGU 383 / acmsguru 383 — Caravans
// https://codeforces.com/problemsets/acmsguru/problem/99999/383
//
// Optimal caravan path uses only Delaunay edges; the Euclidean MST minimizes
// the maximum edge weight on any path.  Answer per query = max edge on MST path.

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <tuple>
#include <utility>
#include <vector>

using ll = long long;
using ld = long double;

static bool ge(ll a, ll b) {
  return a >= b;
}
static bool eq(ll a, ll b) {
  return a == b;
}
static bool gt(ll a, ll b) {
  return a > b;
}
static bool lt(ll a, ll b) {
  return a < b;
}
static int sgn(ll a) {
  return a > 0 ? 1 : (a < 0 ? -1 : 0);
}

struct Pt {
  ll x, y;
  int id;
  Pt() = default;
  Pt(ll x_, ll y_, int id_) : x(x_), y(y_), id(id_) {}
  Pt operator-(const Pt& p) const {
    return Pt(x - p.x, y - p.y, id);
  }
  ll cross(const Pt& p) const {
    return x * p.y - y * p.x;
  }
  ll cross(const Pt& a, const Pt& b) const {
    return (a - *this).cross(b - *this);
  }
  ll dot(const Pt& p) const {
    return x * p.x + y * p.y;
  }
  ll sqrLength() const {
    return dot(*this);
  }
  bool operator==(const Pt& p) const {
    return eq(x, p.x) && eq(y, p.y);
  }
};

static const Pt infPt(1e18, 1e18, -1);

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
  std::vector<QuadEdge> buf;
  int ptr = 0;

  void reset(int n) {
    buf.clear();
    buf.reserve(64 * n);
    ptr = 0;
  }

  QuadEdge* get() {
    if (ptr >= (int)buf.size()) {
      buf.resize(buf.size() + std::max(1024, (int)buf.size()));
    }
    return &buf[ptr++];
  }
};

static QuadPool pool;

static QuadEdge* makeEdge(Pt from, Pt to) {
  QuadEdge *e1 = pool.get(), *e2 = pool.get(), *e3 = pool.get(), *e4 = pool.get();
  e1->origin = from;
  e2->origin = to;
  e3->origin = e4->origin = infPt;
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

static void splice(QuadEdge* a, QuadEdge* b) {
  std::swap(a->onext->rot->onext, b->onext->rot->onext);
  std::swap(a->onext, b->onext);
}

static void deleteEdge(QuadEdge* e) {
  splice(e, e->oprev());
  splice(e->rev(), e->rev()->oprev());
}

static QuadEdge* connect(QuadEdge* a, QuadEdge* b) {
  QuadEdge* e = makeEdge(a->dest(), b->origin);
  splice(e, a->lnext());
  splice(e->rev(), b);
  return e;
}

static bool leftOf(Pt p, QuadEdge* e) {
  return gt(p.cross(e->origin, e->dest()), 0);
}
static bool rightOf(Pt p, QuadEdge* e) {
  return lt(p.cross(e->origin, e->dest()), 0);
}

template <class T> static T det3(T a1, T a2, T a3, T b1, T b2, T b3, T c1, T c2, T c3) {
  return a1 * (b2 * c3 - c2 * b3) - a2 * (b1 * c3 - c1 * b3) + a3 * (b1 * c2 - c1 * b2);
}

static bool inCircle(Pt a, Pt b, Pt c, Pt d) {
  using i128 = __int128_t;
  i128 det = -det3<i128>(b.x, b.y, b.sqrLength(), c.x, c.y, c.sqrLength(), d.x, d.y, d.sqrLength());
  det += det3<i128>(a.x, a.y, a.sqrLength(), c.x, c.y, c.sqrLength(), d.x, d.y, d.sqrLength());
  det -= det3<i128>(a.x, a.y, a.sqrLength(), b.x, b.y, b.sqrLength(), d.x, d.y, d.sqrLength());
  det += det3<i128>(a.x, a.y, a.sqrLength(), b.x, b.y, b.sqrLength(), c.x, c.y, c.sqrLength());
  return det > 0;
}

static std::pair<QuadEdge*, QuadEdge*> buildTr(int l, int r, std::vector<Pt>& p) {
  if (r - l + 1 == 2) {
    QuadEdge* res = makeEdge(p[l], p[r]);
    return {res, res->rev()};
  }
  if (r - l + 1 == 3) {
    QuadEdge *a = makeEdge(p[l], p[l + 1]), *b = makeEdge(p[l + 1], p[r]);
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
  std::tie(ldo, ldi) = buildTr(l, mid, p);
  std::tie(rdi, rdo) = buildTr(mid + 1, r, p);
  while (true) {
    if (leftOf(rdi->origin, ldi)) {
      ldi = ldi->lnext();
      continue;
    }
    if (rightOf(ldi->origin, rdi)) {
      rdi = rdi->rev()->onext;
      continue;
    }
    break;
  }
  QuadEdge* basel = connect(rdi->rev(), ldi);
  auto valid = [&](QuadEdge* e) { return rightOf(e->dest(), basel); };
  if (ldi->origin == ldo->origin) {
    ldo = basel->rev();
  }
  if (rdi->origin == rdo->origin) {
    rdo = basel;
  }
  while (true) {
    QuadEdge* lcand = basel->rev()->onext;
    if (valid(lcand)) {
      while (inCircle(basel->dest(), basel->origin, lcand->dest(), lcand->onext->dest())) {
        QuadEdge* t = lcand->onext;
        deleteEdge(lcand);
        lcand = t;
      }
    }
    QuadEdge* rcand = basel->oprev();
    if (valid(rcand)) {
      while (inCircle(basel->dest(), basel->origin, rcand->dest(), rcand->oprev()->dest())) {
        QuadEdge* t = rcand->oprev();
        deleteEdge(rcand);
        rcand = t;
      }
    }
    if (!valid(lcand) && !valid(rcand)) {
      break;
    }
    if (!valid(lcand) ||
        (valid(rcand) && inCircle(lcand->dest(), lcand->origin, rcand->origin, rcand->dest()))) {
      basel = connect(rcand, basel->rev());
    } else {
      basel = connect(basel->rev(), lcand->rev());
    }
  }
  return {ldo, rdo};
}

static std::vector<std::pair<int, int>> delaunayEdges(std::vector<Pt>& p) {
  pool.reset((int)p.size());
  std::sort(p.begin(), p.end(), [](const Pt& a, const Pt& b) {
    return lt(a.x, b.x) || (eq(a.x, b.x) && lt(a.y, b.y));
  });
  auto res = buildTr(0, (int)p.size() - 1, p);
  QuadEdge* e = res.first;
  std::vector<QuadEdge*> todo = {e};
  while (lt(e->onext->dest().cross(e->dest(), e->origin), 0)) {
    e = e->onext;
  }

  std::vector<std::pair<int, int>> out;
  out.reserve(6 * p.size());
  auto add = [&](QuadEdge* start) {
    QuadEdge* cur = start;
    do {
      cur->used = true;
      int u = cur->origin.id, v = cur->dest().id;
      if (u >= 0 && v >= 0) {
        if (u > v) {
          std::swap(u, v);
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

  std::sort(out.begin(), out.end());
  out.erase(std::unique(out.begin(), out.end()), out.end());
  return out;
}

struct Edge {
  int u, v;
  ll w2;
  bool operator<(const Edge& o) const {
    return w2 < o.w2;
  }
};

struct Lca {
  int n, lg;
  std::vector<std::vector<int>> up;
  std::vector<std::vector<ld>> mx;
  std::vector<int> depth;
  std::vector<std::vector<std::pair<int, ld>>> adj;

  explicit Lca(int n_) : n(n_) {
    lg = 1;
    while ((1 << lg) <= n) {
      ++lg;
    }
    up.assign(lg, std::vector<int>(n, -1));
    mx.assign(lg, std::vector<ld>(n, 0));
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
          mx[k][i] = std::max(mx[k - 1][i], mx[k - 1][mid]);
        }
      }
    }
  }

  ld queryMax(int a, int b) {
    if (a == b) {
      return 0;
    }
    ld ans = 0;
    if (depth[a] < depth[b]) {
      std::swap(a, b);
    }
    int diff = depth[a] - depth[b];
    for (int k = 0; k < lg; ++k) {
      if (diff >> k & 1) {
        ans = std::max(ans, mx[k][a]);
        a = up[k][a];
      }
    }
    if (a == b) {
      return ans;
    }
    for (int k = lg - 1; k >= 0; --k) {
      if (up[k][a] != up[k][b]) {
        ans = std::max({ans, mx[k][a], mx[k][b]});
        a = up[k][a];
        b = up[k][b];
      }
    }
    ans = std::max({ans, mx[0][a], mx[0][b]});
    return ans;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n;
  std::cin >> n;
  std::vector<Pt> pts(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> pts[i].x >> pts[i].y;
    pts[i].id = i;
  }

  auto edges = delaunayEdges(pts);

  std::vector<Edge> elist;
  elist.reserve(edges.size());
  for (const auto& [u, v] : edges) {
    ll dx = pts[u].x - pts[v].x;
    ll dy = pts[u].y - pts[v].y;
    elist.push_back({u, v, dx * dx + dy * dy});
  }
  std::sort(elist.begin(), elist.end());

  struct Dsu {
    std::vector<int> p;
    explicit Dsu(int n) : p(n) {
      std::iota(p.begin(), p.end(), 0);
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
    ld w = std::sqrt((ld)e.w2);
    lca.adj[e.u].push_back({e.v, w});
    lca.adj[e.v].push_back({e.u, w});
    if (++cnt == n - 1) {
      break;
    }
  }
  lca.build(0);

  int q;
  std::cin >> q;
  while (q--) {
    int s, t;
    std::cin >> s >> t;
    --s;
    --t;
    std::cout << std::setprecision(15) << lca.queryMax(s, t) << '\n';
  }
  return 0;
}

// AHC057 A - Drift
// https://atcoder.jp/contests/ahc057/tasks/ahc057_a
//
// Connect nearest pairs at t=0 (union-find, component size <= K) to form M groups.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

struct Dsu {
  vector<int> p, sz;
  Dsu(int n) : p(n), sz(n, 1) { iota(p.begin(), p.end(), 0); }
  int find(int x) {
    return p[x] == x ? x : p[x] = find(p[x]);
  }
  bool unite(int a, int b, int k) {
    a = find(a);
    b = find(b);
    if (a == b)
      return false;
    if (sz[a] + sz[b] > k)
      return false;
    if (sz[a] < sz[b])
      swap(a, b);
    p[b] = a;
    sz[a] += sz[b];
    return true;
  }
  int components() {
    int c = 0;
    for (int i = 0; i < (int)p.size(); ++i)
      if (find(i) == i)
        c++;
    return c;
  }
};

i64 torus_dist(i64 x1, i64 y1, i64 x2, i64 y2, i64 L) {
  i64 dx = x1 - x2;
  i64 dy = y1 - y2;
  dx = (dx + L / 2) % L - L / 2;
  dy = (dy + L / 2) % L - L / 2;
  return llround(sqrt((long double)dx * dx + (long double)dy * dy));
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, T, M, K;
  i64 L;
  cin >> N >> T >> M >> K >> L;
  vector<i64> x(N), y(N);
  for (int i = 0; i < N; ++i) {
    i64 vx, vy;
    cin >> x[i] >> y[i] >> vx >> vy;
  }

  struct Edge {
    i64 d;
    int u, v;
    bool operator<(const Edge& o) const { return d < o.d; }
  };
  vector<Edge> edges;
  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j)
      edges.push_back({torus_dist(x[i], y[i], x[j], y[j], L), i, j});
  }
  sort(edges.begin(), edges.end());

  Dsu dsu(N);
  vector<array<int, 3>> ops;
  for (const auto& e : edges) {
    if (dsu.components() <= M)
      break;
    if (dsu.unite(e.u, e.v, K))
      ops.push_back({0, e.u, e.v});
  }

  for (const auto& op : ops)
    cout << op[0] << ' ' << op[1] << ' ' << op[2] << '\n';
  return 0;
}

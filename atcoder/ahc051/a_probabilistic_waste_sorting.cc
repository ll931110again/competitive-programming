// AHC051 A - Probabilistic Waste Sorting
// https://atcoder.jp/contests/ahc051/tasks/ahc051_a
//
// Chain positions by polar angle from inlet; assign sorters by nearest-processor split.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

struct Point {
  i64 x, y;
  int id;
};

i64 cross(i64 ox, i64 oy, i64 ax, i64 ay, i64 bx, i64 by) {
  return (ax - ox) * (by - oy) - (ay - oy) * (bx - ox);
}

bool seg_inter(i64 ax, i64 ay, i64 bx, i64 by, i64 cx, i64 cy, i64 dx, i64 dy) {
  auto orient = [](i64 px, i64 py, i64 qx, i64 qy, i64 rx, i64 ry) {
    return (qx - px) * (ry - py) - (qy - py) * (rx - px);
  };
  const i64 o1 = orient(ax, ay, bx, by, cx, cy);
  const i64 o2 = orient(ax, ay, bx, by, dx, dy);
  const i64 o3 = orient(cx, cy, dx, dy, ax, ay);
  const i64 o4 = orient(cx, cy, dx, dy, bx, by);
  if (o1 == 0 && o2 == 0 && o3 == 0 && o4 == 0) {
    if (max(ax, bx) < min(cx, dx) || max(cx, dx) < min(ax, bx))
      return false;
    if (max(ay, by) < min(cy, dy) || max(cy, dy) < min(ay, by))
      return false;
    return true;
  }
  return (o1 > 0) != (o2 > 0) && (o3 > 0) != (o4 > 0);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M, K;
  cin >> N >> M >> K;
  vector<Point> pts(N + M);
  for (int i = 0; i < N + M; ++i) {
    cin >> pts[i].x >> pts[i].y;
    pts[i].id = i;
  }
  vector<vector<double>> ps(K, vector<double>(N));
  for (int k = 0; k < K; ++k)
    for (int j = 0; j < N; ++j)
      cin >> ps[k][j];

  constexpr i64 kInX = 0, kInY = 5000;
  vector<int> order(N + M);
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(), [&](int a, int b) {
    const auto& p = pts[a];
    const auto& q = pts[b];
    const i64 ca = cross(kInX, kInY, p.x, p.y, 10000, kInY);
    const i64 cb = cross(kInX, kInY, q.x, q.y, 10000, kInY);
    if (ca != cb)
      return ca > cb;
    const i64 da = (p.x - kInX) * (p.x - kInX) + (p.y - kInY) * (p.y - kInY);
    const i64 db = (q.x - kInX) * (q.x - kInX) + (q.y - kInY) * (q.y - kInY);
    return da < db;
  });

  vector<int> ds(N);
  iota(ds.begin(), ds.end(), 0);

  int s = order[0];
  if (s >= N)
    s = 0;

  vector<array<int, 3>> cs(M, {-1, 0, 0});
  vector<char> is_m(M, 0);
  for (int id : order) {
    if (id >= N)
      is_m[id - N] = 1;
  }

  for (int step = 0; step + 1 < (int)order.size(); ++step) {
    const int u = order[step];
    const int v = order[step + 1];
    if (u < N || v >= N + M)
      continue;
    const int m = u - N;
    if (!is_m[m])
      continue;
    int best_k = 0;
    double best_err = 1e9;
    for (int k = 0; k < K; ++k) {
      double err = 0;
      for (int j = 0; j < N; ++j) {
        const int dest = (ps[k][j] >= 0.5) ? u : v;
        if (dest >= N && dest < N + M && cs[dest - N][0] == -1)
          err += 1.0;
        else if (dest < N && ds[dest] != j)
          err += 1.0;
      }
      if (err < best_err) {
        best_err = err;
        best_k = k;
      }
    }
    if (seg_inter(kInX, kInY, pts[u].x, pts[u].y, pts[u].x, pts[u].y, pts[v].x,
                  pts[v].y))
      continue;
    bool ok = true;
    for (int m2 = 0; m2 < M && ok; ++m2) {
      if (cs[m2][0] == -1)
        continue;
      const int a = N + m2;
      const int b1 = cs[m2][1], b2 = cs[m2][2];
      if (seg_inter(pts[u].x, pts[u].y, pts[v].x, pts[v].y, pts[a].x, pts[a].y,
                    pts[b1].x, pts[b1].y))
        ok = false;
      if (seg_inter(pts[u].x, pts[u].y, pts[v].x, pts[v].y, pts[a].x, pts[a].y,
                    pts[b2].x, pts[b2].y))
        ok = false;
    }
    if (ok)
      cs[m] = {best_k, u, v};
  }

  for (int i = 0; i < N; ++i) {
    if (i)
      cout << ' ';
    cout << ds[i];
  }
  cout << '\n' << s << '\n';
  for (int i = 0; i < M; ++i) {
    if (cs[i][0] == -1)
      cout << "-1\n";
    else
      cout << cs[i][0] << ' ' << cs[i][1] << ' ' << cs[i][2] << '\n';
  }
  return 0;
}

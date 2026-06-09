#include <bits/stdc++.h>
using namespace std;

namespace {

struct Bomb {
  int x, y, z;
};

inline long long sq(long long v) {
  return v * v;
}

inline long long dist2(int x, int y, int z, const Bomb& b) {
  return sq((long long)x - b.x) + sq((long long)y - b.y) + sq((long long)z - b.z);
}

inline long long value_at(int x, int y, int z, const vector<Bomb>& bombs) {
  long long best = (1LL << 62);
  for (const auto& b : bombs)
    best = min(best, dist2(x, y, z, b));
  return best;
}

struct Box {
  int x0, x1, y0, y1, z0, z1; // inclusive
  long long ub;               // upper bound on max min-dist^2 inside this box
};

inline long long upper_bound(const Box& box, const vector<Bomb>& bombs) {
  long long bound = (1LL << 62);
  for (const auto& b : bombs) {
    long long dx = max(llabs((long long)b.x - box.x0), llabs((long long)b.x - box.x1));
    long long dy = max(llabs((long long)b.y - box.y0), llabs((long long)b.y - box.y1));
    long long dz = max(llabs((long long)b.z - box.z0), llabs((long long)b.z - box.z1));
    long long far = dx * dx + dy * dy + dz * dz;
    bound = min(bound, far);
  }
  return bound;
}

struct CmpBox {
  bool operator()(const Box& a, const Box& b) const {
    return a.ub < b.ub; // max-heap by ub
  }
};

long long solve_case(const vector<Bomb>& bombs) {
  // Get a decent initial lower bound via a deterministic multi-start local search
  // (helps pruning, but correctness comes from the box search).
  auto improve_from = [&](int sx, int sy, int sz) -> long long {
    int x = sx, y = sy, z = sz;
    long long best = value_at(x, y, z, bombs);
    for (int step = 512; step >= 1; step >>= 1) {
      bool changed = true;
      while (changed) {
        changed = false;
        int bx = x, by = y, bz = z;
        long long bv = best;
        for (int dx : {-step, 0, step}) {
          for (int dy : {-step, 0, step}) {
            for (int dz : {-step, 0, step}) {
              int nx = x + dx, ny = y + dy, nz = z + dz;
              if (nx < 0 || nx > 1000 || ny < 0 || ny > 1000 || nz < 0 || nz > 1000)
                continue;
              long long nv = value_at(nx, ny, nz, bombs);
              if (nv > bv) {
                bv = nv;
                bx = nx;
                by = ny;
                bz = nz;
              }
            }
          }
        }
        if (bv > best) {
          best = bv;
          x = bx;
          y = by;
          z = bz;
          changed = true;
        }
      }
    }
    return best;
  };

  long long best = 0;
  // corners + center
  for (int cx : {0, 1000})
    for (int cy : {0, 1000})
      for (int cz : {0, 1000})
        best = max(best, improve_from(cx, cy, cz));
  best = max(best, improve_from(500, 500, 500));
  // starts near some bombs (clamped)
  for (int i = 0; i < (int)bombs.size() && i < 16; i++) {
    best = max(best, improve_from(bombs[i].x, bombs[i].y, bombs[i].z));
    best = max(best, improve_from(min(1000, bombs[i].x + 1), bombs[i].y, bombs[i].z));
  }

  priority_queue<Box, vector<Box>, CmpBox> pq;
  Box root{0, 1000, 0, 1000, 0, 1000, 0};
  root.ub = upper_bound(root, bombs);
  pq.push(root);

  while (!pq.empty()) {
    Box cur = pq.top();
    pq.pop();
    if (cur.ub <= best)
      continue;

    // Update best using midpoint sample (tightens quickly).
    int mx = (cur.x0 + cur.x1) / 2;
    int my = (cur.y0 + cur.y1) / 2;
    int mz = (cur.z0 + cur.z1) / 2;
    best = max(best, value_at(mx, my, mz, bombs));
    if (cur.ub <= best)
      continue;

    if (cur.x0 == cur.x1 && cur.y0 == cur.y1 && cur.z0 == cur.z1) {
      best = max(best, value_at(cur.x0, cur.y0, cur.z0, bombs));
      continue;
    }

    int lx = cur.x1 - cur.x0;
    int ly = cur.y1 - cur.y0;
    int lz = cur.z1 - cur.z0;

    // Split along the longest dimension.
    if (lx >= ly && lx >= lz && lx > 0) {
      int mid = (cur.x0 + cur.x1) / 2;
      Box a{cur.x0, mid, cur.y0, cur.y1, cur.z0, cur.z1, 0};
      Box b{mid + 1, cur.x1, cur.y0, cur.y1, cur.z0, cur.z1, 0};
      a.ub = upper_bound(a, bombs);
      b.ub = upper_bound(b, bombs);
      if (a.ub > best)
        pq.push(a);
      if (b.ub > best)
        pq.push(b);
    } else if (ly >= lx && ly >= lz && ly > 0) {
      int mid = (cur.y0 + cur.y1) / 2;
      Box a{cur.x0, cur.x1, cur.y0, mid, cur.z0, cur.z1, 0};
      Box b{cur.x0, cur.x1, mid + 1, cur.y1, cur.z0, cur.z1, 0};
      a.ub = upper_bound(a, bombs);
      b.ub = upper_bound(b, bombs);
      if (a.ub > best)
        pq.push(a);
      if (b.ub > best)
        pq.push(b);
    } else {
      int mid = (cur.z0 + cur.z1) / 2;
      Box a{cur.x0, cur.x1, cur.y0, cur.y1, cur.z0, mid, 0};
      Box b{cur.x0, cur.x1, cur.y0, cur.y1, mid + 1, cur.z1, 0};
      a.ub = upper_bound(a, bombs);
      b.ub = upper_bound(b, bombs);
      if (a.ub > best)
        pq.push(a);
      if (b.ub > best)
        pq.push(b);
    }
  }

  return best;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int N;
    cin >> N;
    vector<Bomb> bombs(N);
    for (int i = 0; i < N; i++) {
      cin >> bombs[i].x >> bombs[i].y >> bombs[i].z;
    }
    long long ans = solve_case(bombs);
    cout << "Case #" << tc << ": " << ans << "\n";
  }
  return 0;
}

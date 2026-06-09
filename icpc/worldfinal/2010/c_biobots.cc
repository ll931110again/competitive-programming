#include <bits/stdc++.h>
using namespace std;

namespace {

struct Wall {
  int x1, y, x2;
};

int find_block_index(const vector<int>& coords, int v) {
  // returns i such that coords[i] <= v < coords[i+1]
  int lo = 0, hi = (int)coords.size() - 2;
  while (lo <= hi) {
    int mid = (lo + hi) / 2;
    if (coords[mid] <= v && v < coords[mid + 1])
      return mid;
    if (v < coords[mid])
      hi = mid - 1;
    else
      lo = mid + 1;
  }
  return -1;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int m, n, w;
  int case_num = 1;
  while (cin >> m >> n >> w) {
    if (m == 0 && n == 0 && w == 0)
      break;

    vector<Wall> walls;
    walls.reserve(w);
    long long blocked_cells = 0;

    vector<int> xs, ys;
    xs.push_back(0);
    xs.push_back(n);
    ys.push_back(0);
    ys.push_back(m);

    for (int i = 0; i < w; i++) {
      int x1, y1, x2, y2;
      cin >> x1 >> y1 >> x2 >> y2;
      Wall ww;
      ww.x1 = x1;
      ww.x2 = x2;
      ww.y = y1;
      walls.push_back(ww);
      blocked_cells += (long long)(x2 - x1 + 1);

      xs.push_back(x1);
      xs.push_back(x2 + 1);
      ys.push_back(y1);
      ys.push_back(y1 + 1);
    }

    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    int X = (int)xs.size() - 1; // blocks
    int Y = (int)ys.size() - 1;

    vector<uint8_t> blocked((size_t)X * (size_t)Y, 0);
    auto id = [&](int ix, int iy) -> int { return iy * X + ix; };

    for (const auto& ww : walls) {
      int iy = find_block_index(ys, ww.y);
      int ix1 = find_block_index(xs, ww.x1);
      int ix2 = find_block_index(xs, ww.x2);
      for (int ix = ix1; ix <= ix2; ix++)
        blocked[id(ix, iy)] = 1;
    }

    int sx = find_block_index(xs, n - 1);
    int sy = find_block_index(ys, m - 1);

    vector<uint8_t> vis((size_t)X * (size_t)Y, 0);
    queue<pair<int, int>> q;
    if (sx >= 0 && sy >= 0 && !blocked[id(sx, sy)]) {
      vis[id(sx, sy)] = 1;
      q.push({sx, sy});
    }

    while (!q.empty()) {
      int ix = q.front().first;
      int iy = q.front().second;
      q.pop();

      // move west
      if (ix - 1 >= 0) {
        int nx = ix - 1, ny = iy;
        int nid = id(nx, ny);
        if (!blocked[nid] && !vis[nid]) {
          vis[nid] = 1;
          q.push({nx, ny});
        }
      }
      // move south
      if (iy - 1 >= 0) {
        int nx = ix, ny = iy - 1;
        int nid = id(nx, ny);
        if (!blocked[nid] && !vis[nid]) {
          vis[nid] = 1;
          q.push({nx, ny});
        }
      }
    }

    long long reachable = 0;
    for (int iy = 0; iy < Y; iy++) {
      long long h = (long long)ys[iy + 1] - ys[iy];
      for (int ix = 0; ix < X; ix++) {
        if (!vis[id(ix, iy)])
          continue;
        long long wdt = (long long)xs[ix + 1] - xs[ix];
        reachable += wdt * h;
      }
    }

    long long total_cells = 1LL * m * n;
    long long empty_cells = total_cells - blocked_cells;
    long long stuck = empty_cells - reachable;

    cout << "Case " << case_num++ << ": " << stuck << "\n";
  }
  return 0;
}

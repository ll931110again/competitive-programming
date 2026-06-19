#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class BreakingChocolate {
  vector<int> xs, ys;
  vector<pair<int, int>> special;
  vector<int> memo;
  vector<unsigned short> cnt;
  int nx, ny;
  static const int INF = 1e9;

  int id(int xi, int xj, int yi, int yj) const {
    return ((xi * nx + xj) * ny + yi) * ny + yj;
  }

  i64 area(int xi, int xj, int yi, int yj) const {
    return 1LL * (xs[xj] - xs[xi]) * (ys[yj] - ys[yi]);
  }

  int getCnt(int xi, int xj, int yi, int yj) const {
    return cnt[id(xi, xj, yi, yj)];
  }

  int pieceCost(int xi, int xj, int yi, int yj, int c) {
    if (c == 0 || c == area(xi, xj, yi, yj))
      return 0;
    return dp(xi, xj, yi, yj);
  }

  int dp(int xi, int xj, int yi, int yj) {
    int idx = id(xi, xj, yi, yj);
    if (memo[idx] != -1)
      return memo[idx];

    int c = getCnt(xi, xj, yi, yj);
    int best = INF;
    for (int xk = (xi + 1); xk < (xj); ++xk) {
      int cL = getCnt(xi, xk, yi, yj);
      best = min(best, pieceCost(xi, xk, yi, yj, cL) + pieceCost(xk, xj, yi, yj, c - cL) + 1);
    }
    for (int yk = (yi + 1); yk < (yj); ++yk) {
      int cT = getCnt(xi, xj, yi, yk);
      best = min(best, pieceCost(xi, xj, yi, yk, cT) + pieceCost(xi, xj, yk, yj, c - cT) + 1);
    }
    return memo[idx] = best;
  }

  void addCoord(vector<int>& coords, int v, int lo, int hi) {
    if (v >= lo && v <= hi)
      coords.push_back(v);
  }

  void buildCounts() {
    vector<vector<int>> ps(nx, vector<int>(ny, 0));
    for (int i = 0; i < (special.size(); ++i)) {
      int x = special[i].first, y = special[i].second;
      int xi = lower_bound(xs.begin(), xs.end(), x) - xs.begin();
      int yi = lower_bound(ys.begin(), ys.end(), y) - ys.begin();
      ps[xi + 1][yi + 1]++;
    }
    for (int xi = (1); xi < (nx); ++xi)
      for (int yi = (1); yi < (ny); ++yi)
        ps[xi][yi] += ps[xi - 1][yi] + ps[xi][yi - 1] - ps[xi - 1][yi - 1];

    cnt.assign(nx * nx * ny * ny, 0);
    for (int xi = (0); xi < (nx - 1); ++xi)
      for (int xj = (xi + 1); xj < (nx); ++xj)
        for (int yi = (0); yi < (ny - 1); ++yi)
          for (int yj = (yi + 1); yj < (ny); ++yj)
            cnt[id(xi, xj, yi, yj)] = ps[xj][yj] - ps[xi][yj] - ps[xj][yi] + ps[xi][yi];
  }

public:
  int minSteps(int W, int H, vector<int> sx, vector<int> sy) {
    special.clear();
    for (int i = 0; i < (sx.size(); ++i))
      special.push_back(make_pair(sx[i], sy[i]));

    vector<int> rawX, rawY;
    addCoord(rawX, 1, 1, W + 1);
    addCoord(rawX, W + 1, 1, W + 1);
    addCoord(rawY, 1, 1, H + 1);
    addCoord(rawY, H + 1, 1, H + 1);
    for (int i = 0; i < (sx.size(); ++i)) {
      addCoord(rawX, sx[i], 1, W + 1);
      addCoord(rawX, sx[i] + 1, 1, W + 1);
      addCoord(rawY, sy[i], 1, H + 1);
      addCoord(rawY, sy[i] + 1, 1, H + 1);
    }
    sort(rawX.begin(), rawX.end());
    rawX.erase(unique(rawX.begin(), rawX.end()), rawX.end());
    sort(rawY.begin(), rawY.end());
    rawY.erase(unique(rawY.begin(), rawY.end()), rawY.end());
    xs = rawX;
    ys = rawY;
    nx = (int)xs.size();
    ny = (int)ys.size();

    buildCounts();
    memo.assign(nx * nx * ny * ny, -1);

    vector<int> ix(special.size()), iy(special.size());
    for (int i = 0; i < (special.size(); ++i)) {
      ix[i] = lower_bound(xs.begin(), xs.end(), special[i].first) - xs.begin();
      iy[i] = lower_bound(ys.begin(), ys.end(), special[i].second) - ys.begin();
    }

    vector<pair<int, int>> order;
    for (int i = 0; i < (special.size(); ++i))
      for (int xi = (0); xi < (ix[i] + 1); ++xi)
        for (int xj = (ix[i]); xj < (nx); ++xj)
          for (int yi = (0); yi < (iy[i] + 1); ++yi)
            for (int yj = (iy[i]); yj < (ny); ++yj)
              if (xi < xj && yi < yj) {
                int key = (xj - xi) * ny + (yj - yi);
                order.push_back(make_pair(key, id(xi, xj, yi, yj)));
              }

    sort(order.begin(), order.end());
    order.erase(unique(order.begin(), order.end()), order.end());

    for (const auto& entry : order) {
      int t = entry.second;
      int yj = t % ny;
      t /= ny;
      int yi = t % ny;
      t /= ny;
      int xj = t % nx;
      int xi = t / nx;
      int c = getCnt(xi, xj, yi, yj);
      if (c == 0 || c == area(xi, xj, yi, yj))
        memo[entry.second] = 0;
      else
        dp(xi, xj, yi, yj);
    }

    return memo[id(0, nx - 1, 0, ny - 1)];
  }
};

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
struct AliceInWanderland {
  static const i64 INF = (i64)4e18;
  static const i64 LIMIT = 1000000000000000LL;

  int n;
  vector<int> px, py;
  vector<int> period;
  vector<i64> vx, vy;
  vector<vector<i64>> relx, rely;

  void posAt(int j, i64 t, i64& x, i64& y) const {
    int p = period[j];
    i64 q = t / p;
    int r = (int)(t % p);
    x = px[j] + q * vx[j] + relx[j][r];
    y = py[j] + q * vy[j] + rely[j][r];
  }

  i64 cheb(i64 x1, i64 y1, i64 x2, i64 y2) const {
    return max(llabs(x1 - x2), llabs(y1 - y2));
  }

  i64 alignTime(i64 need, int p, int r) {
    i64 mod = need % p;
    i64 add = r - mod;
    if (add < 0)
      add += p;
    return need + add;
  }

  i64 catchGap(i64 T, i64 ax, i64 ay, int j, int r, i64 k) const {
    int p = period[j];
    i64 t = k * p + r;
    i64 x = px[j] + relx[j][r] + k * vx[j];
    i64 y = py[j] + rely[j][r] + k * vy[j];
    return t - (T + cheb(ax, ay, x, y));
  }

  i64 nextCatch(i64 T, i64 ax, i64 ay, int j) {
    int p = period[j];
    i64 best = INF;
    i64 kCap = (LIMIT - p) / p + 1;

    for (int r = 0; r < p; r++) {
      i64 rx0 = px[j] + relx[j][r];
      i64 ry0 = py[j] + rely[j][r];

      if (vx[j] == 0 && vy[j] == 0) {
        i64 need = T + cheb(ax, ay, rx0, ry0);
        i64 t = alignTime(need, p, r);
        if (t <= LIMIT)
          best = min(best, t);
        continue;
      }

      i64 lo = max(0LL, (T + 1 - r + p - 1) / p);
      i64 hi = kCap;
      if (catchGap(T, ax, ay, j, r, lo) < 0) {
        i64 step = max(1LL, max(llabs(vx[j]), llabs(vy[j])));
        if (p <= step) {
          if (catchGap(T, ax, ay, j, r, hi) < 0)
            continue;
        } else {
          while (lo < hi && catchGap(T, ax, ay, j, r, hi) < 0) {
            if (hi > (i64)1e12) {
              hi = kCap;
              break;
            }
            hi *= 2;
            if (hi > kCap)
              hi = kCap;
          }
          if (catchGap(T, ax, ay, j, r, hi) < 0)
            continue;
        }
      }

      while (lo < hi) {
        i64 mid = lo + (hi - lo) / 2;
        if (catchGap(T, ax, ay, j, r, mid) >= 0)
          hi = mid;
        else
          lo = mid + 1;
      }
      i64 t = lo * p + r;
      if (catchGap(T, ax, ay, j, r, lo) >= 0 && t <= LIMIT)
        best = min(best, t);
    }
    return best;
  }

  int expand(int mask, i64 t, i64 x, i64 y) {
    bool changed = true;
    while (changed) {
      changed = false;
      for (int k = 0; k < n; k++)
        if (!(mask & (1 << k))) {
          i64 cx, cy;
          posAt(k, t, cx, cy);
          if (cx == x && cy == y) {
            mask |= 1 << k;
            changed = true;
          }
        }
    }
    return mask;
  }

  i64 getMinimum(vector<int> rabbitX, vector<int> rabbitY, vector<string> moves) {
    n = rabbitX.size();
    px = rabbitX;
    py = rabbitY;
    period.resize(n);
    vx.assign(n, 0);
    vy.assign(n, 0);
    relx.assign(n, vector<i64>());
    rely.assign(n, vector<i64>());

    for (int i = 0; i < n; i++) {
      period[i] = (int)moves[i].size();
      int p = period[i];
      relx[i].assign(p, 0);
      rely[i].assign(p, 0);
      i64 x = 0, y = 0;
      for (int t = 0; t < p; t++) {
        relx[i][t] = x;
        rely[i][t] = y;
        char c = moves[i][t];
        if (c == 'R')
          x++;
        else if (c == 'L')
          x--;
        else if (c == 'U')
          y++;
        else
          y--;
      }
      vx[i] = x;
      vy[i] = y;
    }

    int full = (1 << n) - 1;
    vector<vector<i64>> dp(1 << n, vector<i64>(n, INF));

    for (int i = 0; i < n; i++) {
      i64 t = nextCatch(0, 0, 0, i);
      if (t >= INF)
        continue;
      i64 x, y;
      posAt(i, t, x, y);
      int mask = expand(1 << i, t, x, y);
      dp[mask][i] = min(dp[mask][i], t);
    }

    for (int mask = 1; mask < (1 << n); mask++) {
      for (int i = 0; i < n; i++) {
        if (!(mask & (1 << i)) || dp[mask][i] >= INF)
          continue;
        i64 T = dp[mask][i];
        i64 ax, ay;
        posAt(i, T, ax, ay);

        for (int j = 0; j < n; j++) {
          if (mask & (1 << j))
            continue;
          i64 t = nextCatch(T, ax, ay, j);
          if (t >= INF)
            continue;
          i64 x, y;
          posAt(j, t, x, y);
          int nmask = expand(mask | (1 << j), t, x, y);
          dp[nmask][j] = min(dp[nmask][j], t);
        }
      }
    }

    i64 ans = INF;
    for (int i = 0; i < n; i++)
      ans = min(ans, dp[full][i]);

    if (ans > LIMIT || ans >= INF)
      return -1;
    return ans;
  }
};

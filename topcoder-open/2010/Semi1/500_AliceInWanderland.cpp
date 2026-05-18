#include <bits/stdc++.h>
using namespace std;

struct AliceInWanderland {
  static const long long INF = (long long)4e18;
  static const long long LIMIT = 1000000000000000LL;

  int n;
  vector<int> px, py;
  vector<int> period;
  vector<long long> vx, vy;
  vector<vector<long long>> relx, rely;

  void posAt(int j, long long t, long long& x, long long& y) const {
    int p = period[j];
    long long q = t / p;
    int r = (int)(t % p);
    x = px[j] + q * vx[j] + relx[j][r];
    y = py[j] + q * vy[j] + rely[j][r];
  }

  long long cheb(long long x1, long long y1, long long x2, long long y2) const {
    return max(llabs(x1 - x2), llabs(y1 - y2));
  }

  long long alignTime(long long need, int p, int r) {
    long long mod = need % p;
    long long add = r - mod;
    if (add < 0)
      add += p;
    return need + add;
  }

  long long catchGap(long long T, long long ax, long long ay, int j, int r, long long k) const {
    int p = period[j];
    long long t = k * p + r;
    long long x = px[j] + relx[j][r] + k * vx[j];
    long long y = py[j] + rely[j][r] + k * vy[j];
    return t - (T + cheb(ax, ay, x, y));
  }

  long long nextCatch(long long T, long long ax, long long ay, int j) {
    int p = period[j];
    long long best = INF;
    long long kCap = (LIMIT - p) / p + 1;

    for (int r = 0; r < p; r++) {
      long long rx0 = px[j] + relx[j][r];
      long long ry0 = py[j] + rely[j][r];

      if (vx[j] == 0 && vy[j] == 0) {
        long long need = T + cheb(ax, ay, rx0, ry0);
        long long t = alignTime(need, p, r);
        if (t <= LIMIT)
          best = min(best, t);
        continue;
      }

      long long lo = max(0LL, (T + 1 - r + p - 1) / p);
      long long hi = kCap;
      if (catchGap(T, ax, ay, j, r, lo) < 0) {
        long long step = max(1LL, max(llabs(vx[j]), llabs(vy[j])));
        if (p <= step) {
          if (catchGap(T, ax, ay, j, r, hi) < 0)
            continue;
        } else {
          while (lo < hi && catchGap(T, ax, ay, j, r, hi) < 0) {
            if (hi > (long long)1e12) {
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
        long long mid = lo + (hi - lo) / 2;
        if (catchGap(T, ax, ay, j, r, mid) >= 0)
          hi = mid;
        else
          lo = mid + 1;
      }
      long long t = lo * p + r;
      if (catchGap(T, ax, ay, j, r, lo) >= 0 && t <= LIMIT)
        best = min(best, t);
    }
    return best;
  }

  int expand(int mask, long long t, long long x, long long y) {
    bool changed = true;
    while (changed) {
      changed = false;
      for (int k = 0; k < n; k++)
        if (!(mask & (1 << k))) {
          long long cx, cy;
          posAt(k, t, cx, cy);
          if (cx == x && cy == y) {
            mask |= 1 << k;
            changed = true;
          }
        }
    }
    return mask;
  }

  long long getMinimum(vector<int> rabbitX, vector<int> rabbitY, vector<string> moves) {
    n = rabbitX.size();
    px = rabbitX;
    py = rabbitY;
    period.resize(n);
    vx.assign(n, 0);
    vy.assign(n, 0);
    relx.assign(n, vector<long long>());
    rely.assign(n, vector<long long>());

    for (int i = 0; i < n; i++) {
      period[i] = (int)moves[i].size();
      int p = period[i];
      relx[i].assign(p, 0);
      rely[i].assign(p, 0);
      long long x = 0, y = 0;
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
    vector<vector<long long>> dp(1 << n, vector<long long>(n, INF));

    for (int i = 0; i < n; i++) {
      long long t = nextCatch(0, 0, 0, i);
      if (t >= INF)
        continue;
      long long x, y;
      posAt(i, t, x, y);
      int mask = expand(1 << i, t, x, y);
      dp[mask][i] = min(dp[mask][i], t);
    }

    for (int mask = 1; mask < (1 << n); mask++) {
      for (int i = 0; i < n; i++) {
        if (!(mask & (1 << i)) || dp[mask][i] >= INF)
          continue;
        long long T = dp[mask][i];
        long long ax, ay;
        posAt(i, T, ax, ay);

        for (int j = 0; j < n; j++) {
          if (mask & (1 << j))
            continue;
          long long t = nextCatch(T, ax, ay, j);
          if (t >= INF)
            continue;
          long long x, y;
          posAt(j, t, x, y);
          int nmask = expand(mask | (1 << j), t, x, y);
          dp[nmask][j] = min(dp[nmask][j], t);
        }
      }
    }

    long long ans = INF;
    for (int i = 0; i < n; i++)
      ans = min(ans, dp[full][i]);

    if (ans > LIMIT || ans >= INF)
      return -1;
    return ans;
  }
};

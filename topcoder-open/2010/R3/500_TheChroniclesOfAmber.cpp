// {{{ Boilerplate Code <--------------------------------------------------
//
// vim:filetype=cpp foldmethod=marker foldmarker={{{,}}}

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>

#define FOR(I, A, B) for (int I = (A); I < (B); ++I)
#define REP(I, N) FOR(I, 0, N)
#define ALL(A) (A).begin(), (A).end()

using namespace std;

// }}}

class TheChroniclesOfAmber {
  static const int MAXN = 55;

  double px[MAXN], py[MAXN], dx[MAXN], dy[MAXN];
  int n;

  double dist(double x1, double y1, double x2, double y2) {
    return hypot(x1 - x2, y1 - y2);
  }

  // Min over P on segment (sx,sy)-(ex,ey) of max(wait, dist((sx,sy),P)) + dist(P, (tx,ty)).
  double segCost(double sx, double sy, double ex, double ey, double tx, double ty, double wait) {
    double L = dist(sx, sy, ex, ey);
    if (L < 1e-12) {
      return wait + dist(sx, sy, tx, ty);
    }
    double best = 1e100;
    auto eval = [&](double t) {
      double x = sx + (ex - sx) * t;
      double y = sy + (ey - sy) * t;
      double ride = max(wait, t * L) + dist(x, y, tx, ty);
      best = min(best, ride);
    };
    eval(0);
    eval(1);
    double t0 = wait / L;
    if (t0 > 0 && t0 < 1)
      eval(t0);
    return best;
  }

  double reach[MAXN][MAXN];
  double finish[MAXN];

  double rideOnPath(int i, int j, double wait) {
    double along = segCost(px[j], py[j], dx[j], dy[j], dx[i], dy[i], wait);
    if (dist(px[j], py[j], dx[i], dy[i]) < 1e-12)
      return max(finish[j], along);
    return along;
  }

  double viaStart(int i, int j) {
    double ride = dist(px[j], py[j], dx[i], dy[i]);
    if (ride < 1e-12)
      return max(finish[j], reach[i][j]);
    return reach[i][j] + ride;
  }

  bool canFinish(double T) {
    REP(i, n) REP(j, n) reach[i][j] = (i == j) ? 0 : dist(px[i], py[i], px[j], py[j]);

    REP(k, n) REP(i, n) REP(j, n) reach[i][j] = min(reach[i][j], reach[i][k] + reach[k][j]);

    REP(i, n) finish[i] = dist(px[i], py[i], dx[i], dy[i]);

    REP(round, n + 1) {
      REP(i, n) REP(j, n) {
        finish[i] = min(finish[i], viaStart(i, j));
        finish[i] = min(finish[i], rideOnPath(i, j, reach[i][j]));
        finish[i] = min(finish[i], rideOnPath(i, j, 0));
      }
      REP(k, n) REP(i, n) REP(j, n) {
        double via = max(finish[k], reach[i][j]);
        finish[i] = min(finish[i], via + dist(px[j], py[j], dx[i], dy[i]));
        finish[i] = min(finish[i], rideOnPath(i, j, via));
      }
    }

    REP(i, n)
    if (finish[i] > T + 1e-9)
      return false;
    return true;
  }

public:
  double minimumTime(vector<int> princeX, vector<int> princeY, vector<int> destinationX,
                     vector<int> destinationY) {
    n = princeX.size();
    REP(i, n) {
      px[i] = princeX[i];
      py[i] = princeY[i];
      dx[i] = destinationX[i];
      dy[i] = destinationY[i];
    }

    double lo = 0, hi = 0;
    REP(i, n) hi = max(hi, dist(px[i], py[i], dx[i], dy[i]));
    hi += 40000;

    REP(it, 80) {
      double mid = (lo + hi) / 2;
      if (canFinish(mid))
        hi = mid;
      else
        lo = mid;
    }
    return hi;
  }
};

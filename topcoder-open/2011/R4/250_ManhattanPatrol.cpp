#include <bits/stdc++.h>
using namespace std;

class ManhattanPatrol {
  bool intersect(long long x1, long long y1, long long x2, long long y2, long long x3, long long y3,
                 long long x4, long long y4) {
    long long ax1 = min(x1, x2), ax2 = max(x1, x2);
    long long ay1 = min(y1, y2), ay2 = max(y1, y2);
    long long bx1 = min(x3, x4), bx2 = max(x3, x4);
    long long by1 = min(y3, y4), by2 = max(y3, y4);
    return max(ax1, bx1) <= min(ax2, bx2) && max(ay1, by1) <= min(ay2, by2);
  }

public:
  long long countIntersections(int N, int AX, int BX, int MX, int AY, int BY, int MY) {
    vector<long long> X(N), Y(N);
    X[0] = BX;
    Y[0] = BY;
    for (int i = 1; i < N; i++) {
      X[i] = (AX * X[i - 1] + BX) % MX;
      Y[i] = (AY * Y[i - 1] + BY) % MY;
    }
    long long ans = 0;
    for (int a = 0; a < N; a++) {
      for (int b = a + 1; b < N; b++) {
        for (int c = 0; c < N; c++) {
          if (c == a || c == b)
            continue;
          for (int d = c + 1; d < N; d++) {
            if (d == a || d == b)
              continue;
            if (intersect(X[a], Y[a], X[b], Y[b], X[c], Y[c], X[d], Y[d]))
              ans++;
          }
        }
      }
    }
    return ans;
  }
};

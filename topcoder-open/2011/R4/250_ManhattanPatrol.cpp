#include <bits/stdc++.h>
using namespace std;

class ManhattanPatrol {
  struct Pt {
    long long x, y;
  };

  long long countPattern(const vector<long long>& y, const int perm[4]) {
    int n = (int)y.size();
    long long ans = 0;
    int idx[4];
    for (int a = 0; a < n; a++) {
      idx[0] = a;
      for (int b = a + 1; b < n; b++) {
        idx[1] = b;
        for (int c = b + 1; c < n; c++) {
          idx[2] = c;
          for (int d = c + 1; d < n; d++) {
            idx[3] = d;
            bool ok = true;
            for (int i = 0; i < 4 && ok; i++)
              for (int j = 0; j < 4; j++)
                if (perm[i] < perm[j] && !(y[idx[i]] < y[idx[j]]))
                  ok = false;
            if (ok)
              ans++;
          }
        }
      }
    }
    return ans;
  }

public:
  long long countIntersections(int N, int AX, int BX, int MX, int AY, int BY, int MY) {
    vector<Pt> p(N);
    p[0] = {BX, BY};
    for (int i = 1; i < N; i++) {
      p[i].x = (AX * p[i - 1].x + BX) % MX;
      p[i].y = (AY * p[i - 1].y + BY) % MY;
    }
    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(),
         [&](int a, int b) { return p[a].x < p[b].x || (p[a].x == p[b].x && p[a].y < p[b].y); });
    vector<long long> y(N);
    for (int i = 0; i < N; i++)
      y[i] = p[order[i]].y;

    int good[4][4] = {{1, 0, 3, 2}, {1, 3, 0, 2}, {2, 0, 3, 1}, {2, 3, 0, 1}};
    long long ans = 0;
    for (int t = 0; t < 4; t++)
      ans += countPattern(y, good[t]);
    return ans;
  }
};

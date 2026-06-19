#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class DistantPoints {
  int cnt(int n) const {
    if (n == 2)
      return 25;
    return 4 * cnt(n - 1) - 4 * ((1 << (n - 1)) + 1) + 1;
  }

  pair<int, int> fixed(int n, int k) const {
    int M = (1 << n) + 1;
    int h = (M + 1) / 2;
    if (k == 1)
      return {1, 1};
    if (k == 2)
      return {M, M};
    if (k == 3)
      return {1, M};
    if (k == 4)
      return {M, 1};
    if (k == 5)
      return {h, h};
    if (k == 6)
      return {1, h};
    if (k == 7)
      return {h, 1};
    if (k == 8)
      return {h, M};
    if (k == 9)
      return {M, h};
    int q1 = (h + 1) / 2;
    int q2 = (M + h) / 2;
    if (k == 10)
      return {q1, q1};
    if (k == 11)
      return {q1, q2};
    if (k == 12)
      return {q2, q1};
    if (k == 13)
      return {q2, q2};
    return {-1, -1};
  }

  pair<int, int> trans(pair<int, int> p) const {
    return {2 * p.first - 1, 2 * p.second - 1};
  }

  int dist2(pair<int, int> a, pair<int, int> b) const {
    int dx = a.first - b.first;
    int dy = a.second - b.second;
    return dx * dx + dy * dy;
  }

  bool better(pair<int, int> a, int da, pair<int, int> b, int db) const {
    if (da != db)
      return da > db;
    if (a.first != b.first)
      return a.first < b.first;
    return a.second < b.second;
  }

  pair<int, int> simulate(int n, int k) const {
    int M = (1 << n) + 1;
    vector<vector<int>> dist(M + 2, vector<int>(M + 2, 1e9));
    vector<vector<char>> painted(M + 2, vector<char>(M + 2));
    painted[1][1] = 1;
    auto relax = [&](int sx, int sy) {
      for (int x = 1; x <= M; x++)
        for (int y = 1; y <= M; y++)
          if (!painted[x][y]) {
            int dx = x - sx, dy = y - sy;
            dist[x][y] = min(dist[x][y], dx * dx + dy * dy);
          }
    };
    relax(1, 1);
    for (int step = 2; step <= k; step++) {
      int best = -1, bx = -1, by = -1;
      for (int x = 1; x <= M; x++)
        for (int y = 1; y <= M; y++)
          if (!painted[x][y]) {
            int d = dist[x][y];
            if (best < 0 || d > best || (d == best && (x < bx || (x == bx && y < by)))) {
              best = d;
              bx = x;
              by = y;
            }
          }
      painted[bx][by] = 1;
      relax(bx, by);
      if (step == k)
        return {bx, by};
    }
    return {1, 1};
  }

  pair<int, int> getKthRec(int n, int k) const {
    if (n == 2)
      return simulate(2, k);
    if (k <= 13)
      return fixed(n, k);
    if (k <= cnt(n - 1))
      return trans(getKthRec(n - 1, k));
    i64 lim = (i64)(1 << n) + 1;
    if ((i64)k * lim * lim <= 120000000LL)
      return simulate(n, k);
    vector<pair<int, int>> prefix;
    for (int i = 1; i <= cnt(n - 1); i++)
      prefix.push_back(getKthRec(n, i));
    int M = (1 << n) + 1;
    int h = (M + 1) / 2;
    int subN = n - 1;
    i64 prog[4] = {0, 0, 0, 0};
    vector<pair<int, int>> painted = prefix;

    auto globalPt = [&](int q, pair<int, int> p) {
      int ox = (q & 1) ? h : 1;
      int oy = (q & 2) ? h : 1;
      return make_pair(ox + p.first - 1, oy + p.second - 1);
    };

    auto nearest = [&](pair<int, int> p) {
      int best = INT_MAX;
      for (const auto& q : painted)
        best = min(best, dist2(p, q));
      return best;
    };

    for (int step = (int)prefix.size() + 1; step <= k; step++) {
      pair<int, int> bestP = {-1, -1};
      int bestD = -1;
      int bestQ = -1;
      for (int q = 0; q < 4; q++) {
        if (prog[q] >= cnt(subN))
          continue;
        pair<int, int> gp = globalPt(q, getKthRec(subN, (int)prog[q] + 1));
        int d = nearest(gp);
        if (bestQ < 0 || better(gp, d, bestP, bestD)) {
          bestQ = q;
          bestP = gp;
          bestD = d;
        }
      }
      painted.push_back(bestP);
      prog[bestQ]++;
    }
    return painted.back();
  }

public:
  vector<int> getKth(int N, int K) {
    pair<int, int> p = getKthRec(N, K);
    return {p.first, p.second};
  }
};

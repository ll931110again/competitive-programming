#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class RadarSabotage {
  int W, H;
  vector<int> rx, ry, pw;

  i64 cellCost(int x, int y) const {
    i64 cost = 0;
    for (int i = 0; i < (int)rx.size(); i++) {
      i64 dx = x - rx[i];
      i64 dy = y - ry[i];
      i64 d2 = dx * dx + dy * dy;
      i64 maxP = 0;
      while (maxP < pw[i]) {
        i64 p = maxP + 1;
        if (p * p * p * p >= d2)
          break;
        maxP = p;
      }
      if (maxP < pw[i])
        cost += pw[i] - maxP;
    }
    return cost;
  }

public:
  int minimumDifference(int W, int H, vector<int> radarX, vector<int> radarY,
                        vector<int> radarPower) {
    this->W = W;
    this->H = H;
    rx = radarX;
    ry = radarY;
    pw = radarPower;

    const i64 INF = (1LL << 60);
    vector<vector<i64>> dist(W + 1, vector<i64>(H + 1, INF));
    priority_queue<pair<i64, pair<int, int>>, vector<pair<i64, pair<int, int>>>,
                   greater<pair<i64, pair<int, int>>>>
        pq;

    for (int x = 0; x <= W; x++) {
      dist[x][0] = cellCost(x, 0);
      pq.push({dist[x][0], {x, 0}});
    }

    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    while (!pq.empty()) {
      auto cur = pq.top();
      pq.pop();
      i64 d = cur.first;
      int x = cur.second.first;
      int y = cur.second.second;
      if (d != dist[x][y])
        continue;
      if (y == H)
        return (int)d;
      for (int k = 0; k < 4; k++) {
        int nx = x + dx[k];
        int ny = y + dy[k];
        if (nx < 0 || nx > W || ny < 0 || ny > H)
          continue;
        i64 nd = d + cellCost(nx, ny);
        if (nd < dist[nx][ny]) {
          dist[nx][ny] = nd;
          pq.push({nd, {nx, ny}});
        }
      }
    }

    i64 ans = INF;
    for (int x = 0; x <= W; x++)
      ans = min(ans, dist[x][H]);
    return (int)ans;
  }
};

#include <bits/stdc++.h>
using namespace std;

class RadarSabotage {
  int W, H;
  vector<int> rx, ry, pw;

  long long cellCost(int x, int y) const {
    long long cost = 0;
    for (int i = 0; i < (int)rx.size(); i++) {
      long long dx = x - rx[i];
      long long dy = y - ry[i];
      long long d2 = dx * dx + dy * dy;
      long long maxP = 0;
      while (maxP < pw[i]) {
        long long p = maxP + 1;
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

    const long long INF = (1LL << 60);
    vector<vector<long long>> dist(W + 1, vector<long long>(H + 1, INF));
    priority_queue<pair<long long, pair<int, int>>, vector<pair<long long, pair<int, int>>>,
                   greater<pair<long long, pair<int, int>>>>
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
      long long d = cur.first;
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
        long long nd = d + cellCost(nx, ny);
        if (nd < dist[nx][ny]) {
          dist[nx][ny] = nd;
          pq.push({nd, {nx, ny}});
        }
      }
    }

    long long ans = INF;
    for (int x = 0; x <= W; x++)
      ans = min(ans, dist[x][H]);
    return (int)ans;
  }
};

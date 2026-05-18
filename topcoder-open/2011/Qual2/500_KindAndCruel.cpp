#include <bits/stdc++.h>
using namespace std;

class KindAndCruel {
  int gcd(int a, int b) {
    return b ? gcd(b, a % b) : a;
  }

  bool canEnter(char cell, int t, int K, int C) {
    if (cell == '.')
      return true;
    if (cell == 'K')
      return t % K != 0;
    return t % C == 0;
  }

public:
  int crossTheField(string field, int K, int C) {
    int m = field.size();
    int period = K / gcd(K, C) * C;
    const int INF = 1e9;
    vector<vector<int>> dist(m, vector<int>(period, INF));
    queue<pair<int, int>> q;
    dist[0][0] = 0;
    q.push({0, 0});

    while (!q.empty()) {
      auto [pos, mod] = q.front();
      q.pop();
      int t = dist[pos][mod];
      if (pos == m - 1)
        return t;
      for (int d = -1; d <= 1; d++) {
        int np = pos + d;
        if (np < 0 || np >= m)
          continue;
        int nt = t + 1;
        int nmod = nt % period;
        if (!canEnter(field[np], nt, K, C))
          continue;
        if (dist[np][nmod] > nt) {
          dist[np][nmod] = nt;
          q.push({np, nmod});
        }
      }
    }
    return -1;
  }
};

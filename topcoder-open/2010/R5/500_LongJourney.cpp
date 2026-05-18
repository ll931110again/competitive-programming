#include <bits/stdc++.h>
using namespace std;

class LongJourney {
public:
  long long minimumCost(vector<int> fuelPrices, int fuelTank, vector<string> roads) {
    int N = fuelPrices.size();
    vector<vector<pair<int, int>>> adj(N);
    string s;
    for (int i = 0; i < (int)roads.size(); i++)
      s += roads[i];
    for (int i = 0; i < (int)s.size();) {
      while (i < (int)s.size() && s[i] == ' ')
        i++;
      if (i >= (int)s.size())
        break;
      int u = 0, v = 0, w = 0;
      for (; i < (int)s.size() && s[i] != ','; i++)
        u = u * 10 + s[i] - '0';
      i++;
      for (; i < (int)s.size() && s[i] != ','; i++)
        v = v * 10 + s[i] - '0';
      i++;
      for (; i < (int)s.size() && s[i] != ' '; i++)
        w = w * 10 + s[i] - '0';
      adj[u].push_back(make_pair(v, w));
      adj[v].push_back(make_pair(u, w));
    }

    const long long INF = (1LL << 62);
    vector<vector<long long>> dist(N, vector<long long>(fuelTank + 1, INF));
    typedef pair<long long, pair<int, int>> State;
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[0][0] = 0;
    pq.push(make_pair(0LL, make_pair(0, 0)));

    while (!pq.empty()) {
      State cur = pq.top();
      pq.pop();
      long long c = cur.first;
      int v = cur.second.first;
      int f = cur.second.second;
      if (c != dist[v][f])
        continue;
      if (f > 0 && dist[v][f - 1] + fuelPrices[v] == c)
        continue;

      for (int g = f; g <= fuelTank; g++) {
        long long nc = c + (long long)(g - f) * fuelPrices[v];
        if (nc < dist[v][g]) {
          dist[v][g] = nc;
          pq.push(make_pair(nc, make_pair(v, g)));
        }
      }

      for (int i = 0; i < (int)adj[v].size(); i++) {
        int u = adj[v][i].first;
        int w = adj[v][i].second;
        for (int g = max(f, w); g <= fuelTank; g++) {
          if (dist[v][g] >= INF)
            continue;
          int nf = g - w;
          if (dist[v][g] < dist[u][nf]) {
            dist[u][nf] = dist[v][g];
            pq.push(make_pair(dist[u][nf], make_pair(u, nf)));
          }
        }
      }
    }

    long long ans = INF;
    for (int f = 0; f <= fuelTank; f++)
      ans = min(ans, dist[1][f]);
    if (ans >= INF)
      return -1;
    return ans;
  }
};

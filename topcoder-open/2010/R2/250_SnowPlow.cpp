#include <bits/stdc++.h>
using namespace std;

class SnowPlow {
public:
  int solve(vector<string> roads) {
    int n = roads.size();
    vector<vector<int>> adj(n);
    int total = 0;
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        int k = roads[i][j] - '0';
        if (k == 0)
          continue;
        total += k;
        adj[i].push_back(j);
        adj[j].push_back(i);
      }
    }

    vector<int> seen(n, 0);
    queue<int> q;
    seen[0] = 1;
    q.push(0);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int v : adj[u]) {
        if (!seen[v]) {
          seen[v] = 1;
          q.push(v);
        }
      }
    }

    for (int i = 0; i < n; i++)
      if (!adj[i].empty() && !seen[i])
        return -1;

    return 2 * total;
  }
};

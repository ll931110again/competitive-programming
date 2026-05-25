// Google Code Jam 2011 — Round 2 — D. A.I. War

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

static int g(const vector<vector<int>>& adj, int a, int b, int c) {
  int cnt = 0;
  for (int v : adj[c]) {
    bool seen = false;
    for (int u : adj[a]) {
      if (u == v) {
        seen = true;
        break;
      }
    }
    if (seen)
      continue;
    for (int u : adj[b]) {
      if (u == v) {
        seen = true;
        break;
      }
    }
    if (!seen)
      cnt++;
  }
  return cnt;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int P, W;
    cin >> P >> W;
    vector<vector<int>> adj(P);
    for (int i = 0; i < W; i++) {
      int x, y;
      char comma;
      cin >> x >> comma >> y;
      adj[x].push_back(y);
      adj[y].push_back(x);
    }

    vector<int> dist(P, -1);
    queue<int> q;
    dist[0] = 0;
    q.push(0);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int v : adj[u]) {
        if (dist[v] == -1) {
          dist[v] = dist[u] + 1;
          q.push(v);
        }
      }
    }

    int D = dist[1];
    int conquered = max(0, D - 1);
    int threatened = 0;

    if (D == 1) {
      for (int v : adj[0]) {
        if (v != 0)
          threatened++;
      }
    } else {
      vector<unordered_map<int, int>> f(P);
      for (int b : adj[0]) {
        if (dist[b] != 1)
          continue;
        vector<bool> seen(P, false);
        seen[0] = seen[b] = true;
        int cnt = 2;
        for (int u : {0, b}) {
          for (int v : adj[u]) {
            if (!seen[v]) {
              seen[v] = true;
              cnt++;
            }
          }
        }
        f[b][0] = cnt;
      }

      for (int d = 2; d <= D - 1; d++) {
        vector<unordered_map<int, int>> nf(P);
        for (int b = 0; b < P; b++) {
          if (dist[b] != d - 1)
            continue;
          for (auto [a, fa] : f[b]) {
            if (dist[a] != d - 2)
              continue;
            for (int c : adj[b]) {
              if (dist[c] != d)
                continue;
              int val = fa + g(adj, a, b, c);
              auto it = nf[c].find(b);
              if (it == nf[c].end() || it->second < val)
                nf[c][b] = val;
            }
          }
        }
        f.swap(nf);
      }

      int best = 0;
      for (int b = 0; b < P; b++) {
        if (dist[b] != D - 1)
          continue;
        bool adj_ai = false;
        for (int v : adj[b]) {
          if (v == 1) {
            adj_ai = true;
            break;
          }
        }
        if (!adj_ai)
          continue;
        for (auto [a, fa] : f[b]) {
          if (dist[a] == D - 2)
            best = max(best, fa);
        }
      }
      threatened = best - D;
    }

    cout << "Case #" << tc << ": " << conquered << ' ' << threatened << '\n';
  }
  return 0;
}

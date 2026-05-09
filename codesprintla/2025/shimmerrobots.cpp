// Q – Shimmer Robots
// https://open.kattis.com/problems/shimmerrobots
//
// On each machine k, every type-A operation must finish before every type-B operation.
// Robots are built strictly sequentially; each robot uses at most one operation per machine.
// Model with one auxiliary vertex X_k per machine:
//   robot r uses A on k  -> edge r -> X_k
//   robot r uses B on k  -> edge X_k -> r
// Topological order on N + K vertices yields a valid robot order (robot vertices only).

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M, K;
  if (!(cin >> N >> M >> K))
    return 0;
  const int V = N + K;
  vector<vector<int>> g(V);
  vector<int> indeg(V, 0);
  for (int i = 0; i < M; ++i) {
    int n_i, k_i;
    char c;
    cin >> n_i >> k_i >> c;
    --n_i;
    --k_i;
    int x = N + k_i;
    if (c == 'A') {
      g[n_i].push_back(x);
      indeg[x]++;
    } else {
      g[x].push_back(n_i);
      indeg[n_i]++;
    }
  }

  queue<int> q;
  for (int i = 0; i < V; ++i)
    if (indeg[i] == 0)
      q.push(i);
  vector<int> ord;
  ord.reserve(V);
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    ord.push_back(v);
    for (int to : g[v]) {
      if (--indeg[to] == 0)
        q.push(to);
    }
  }
  if ((int)ord.size() != V) {
    cout << -1 << '\n';
    return 0;
  }
  vector<int> robots;
  robots.reserve(N);
  for (int v : ord)
    if (v < N)
      robots.push_back(v);
  if ((int)robots.size() != N) {
    cout << -1 << '\n';
    return 0;
  }
  for (int i = 0; i < N; ++i) {
    if (i)
      cout << ' ';
    cout << (robots[i] + 1);
  }
  cout << '\n';
  return 0;
}

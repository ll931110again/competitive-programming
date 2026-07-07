// AHC041 A - Christmas Tree Cutting: https://atcoder.jp/contests/ahc041/tasks/ahc041_a
//
// Greedy forest: grow BFS trees from high-beauty roots, depth capped at H.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M, H;
  cin >> N >> M >> H;
  vector<int> A(N);
  for (int i = 0; i < N; i++)
    cin >> A[i];

  vector<vector<int>> adj(N);
  for (int e = 0; e < M; e++) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  for (int i = 0; i < N; i++) {
    int x, y;
    cin >> x >> y;
  }

  vector<int> parent(N, -2);
  vector<int> order(N);
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(), [&](int a, int b) {
    if (A[a] != A[b])
      return A[a] > A[b];
    return a < b;
  });

  for (const int root : order) {
    if (parent[root] != -2)
      continue;
    parent[root] = -1;
    queue<pair<int, int>> q;
    q.emplace(root, 0);
    while (!q.empty()) {
      auto [u, d] = q.front();
      q.pop();
      if (d >= H)
        continue;
      for (const int v : adj[u]) {
        if (parent[v] != -2)
          continue;
        parent[v] = u;
        q.emplace(v, d + 1);
      }
    }
  }

  for (int i = 0; i < N; i++) {
    if (parent[i] == -2)
      parent[i] = -1;
    cout << parent[i] << (i + 1 == N ? '\n' : ' ');
  }
  return 0;
}

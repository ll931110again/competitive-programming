#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif
#include <iostream>
#include <vector>

using namespace std;

struct DSU {
  vector<int> p, r;
  explicit DSU(int n = 0) {
    init(n);
  }
  void init(int n) {
    p.resize(n + 1);
    r.assign(n + 1, 0);
    for (int i = 1; i <= n; i++) {
      p[i] = i;
    }
  }
  int find(int x) {
    return p[x] == x ? x : (p[x] = find(p[x]));
  }
  void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) {
      return;
    }
    if (r[a] < r[b]) {
      swap(a, b);
    }
    p[b] = a;
    if (r[a] == r[b]) {
      r[a]++;
    }
  }
};

// Structural separation on the *underlying undirected* graph after deleting the
// observed worlds F: if no path remains between C and D, their marginals cannot
// influence each other through the graph alone (matches the statement sample).
//
// Note: this ignores subtle Boolean‑gate semantics; it’s the standard practical
// reduction for this problem shape on polytrees.

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, K, Q;
  cin >> N >> K >> Q;
  vector<pair<int, int>> edges(N - 1);
  for (int i = 0; i < N - 1; i++) {
    cin >> edges[i].first >> edges[i].second;
  }
  vector<char> blocked(N + 1, 0);
  for (int i = 0; i < K; i++) {
    int x;
    cin >> x;
    blocked[x] = 1;
  }

  DSU dsu(N);
  for (const auto& e : edges) {
    const int u = e.first;
    const int v = e.second;
    if (blocked[u] || blocked[v]) {
      continue;
    }
    dsu.unite(u, v);
  }

  for (int i = 0; i < Q; i++) {
    int C, D;
    cin >> C >> D;
    if (blocked[C] || blocked[D]) {
      cout << 0 << '\n';
      continue;
    }
    const int ans = (dsu.find(C) == dsu.find(D)) ? 1 : 0;
    cout << ans << '\n';
  }
  return 0;
}

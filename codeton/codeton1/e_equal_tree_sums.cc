// Codeforces 1656 (CodeTON Round 1 (Div. 1 + Div. 2, Rated, Prizes!)) — E. Equal Tree Sums
// Submission: https://codeforces.com/contest/1656/submission/151134737

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 100005;
int T, n;
vector<int> adj[k_max_n];
int ans[k_max_n];

void DFS(int u, int p, int w) {
  ans[u] = w * adj[u].size();
  for (auto v : adj[u])
    if (v != p) {
      DFS(v, u, -w);
    }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 1; i <= n; i++) {
      adj[i].clear();
    }
    for (int i = 1; i < n; i++) {
      int u, v;
      cin >> u >> v;
      adj[u].push_back(v);
      adj[v].push_back(u);
    }
    DFS(1, -1, 1);
    for (int i = 1; i <= n; i++) {
      printf("%d ", ans[i]);
    }
    printf("\n");
  }
  return 0;
}

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 50005;
#define maxk 505

int n, k, f[k_max_n][maxk];
long long ret = 0;
vector<int> adj[k_max_n];

void DFS(int u, int pre) {
  for (int i = 0; i < adj[u].size(); i++) {
    int v = adj[u][i];
    if (v == pre)
      continue;
    DFS(v, u);
  }
  f[u][0] = 1;
  for (int i = 0; i < adj[u].size(); i++) {
    int v = adj[u][i];
    if (v == pre)
      continue;
    for (int j = 0; j < k; j++)
      ret += 1LL * f[u][k - j - 1] * f[v][j];
    for (int j = 0; j < k; j++)
      f[u][j + 1] += f[v][j];
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> k;
  for (int i = 1; i < n; i++) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  DFS(1, -1);
  cout << ret << endl;
}

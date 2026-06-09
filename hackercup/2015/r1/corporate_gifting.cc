#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
#define maxk 20

int T, n;
int dp[k_max_n][maxk];
int par[k_max_n];
vector<int> adj[k_max_n];
int deg[k_max_n];

void solve(int it) {
  cin >> n;
  for (int i = 1; i <= n; i++) {
    adj[i].clear();
  }

  memset(deg, 0, sizeof deg);
  for (int i = 1; i <= n; i++) {
    cin >> par[i];
    if (par[i]) {
      adj[par[i]].push_back(i);
      deg[par[i]]++;
    }
  }

  queue<int> q;
  for (int i = 1; i <= n; i++)
    if (!deg[i]) {
      q.push(i);
    }

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    for (int c = 1; c < maxk; c++) {
      dp[u][c] = c;
      for (auto v : adj[u]) {
        int min_cost = 1 << 30;
        for (int d = 1; d < maxk; d++)
          if (d != c) {
            min_cost = min(min_cost, dp[v][d]);
          }
        dp[u][c] += min_cost;
      }
    }

    deg[par[u]]--;
    if (par[u] > 0 && deg[par[u]] == 0) {
      q.push(par[u]);
    }
  }

  int ret = dp[1][1];
  for (int i = 2; i < maxk; i++) {
    ret = min(ret, dp[1][i]);
  }
  cout << "Case #" << it << ": " << ret << '\n';
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  for (int it = 1; it <= T; it++) {
    solve(it);
  }
  return 0;
}

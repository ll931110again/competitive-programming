
#include <bits/stdc++.h>
using namespace std;

int T, n, m;

void solve() {
  vector deg(2, vector<int>(n));
  vector<vector<pair<int, int>>> g(n);

  for (int x = 0; x < m; x++) {
    int o, i, j;
    cin >> o >> i >> j;
    o--;
    i--;
    j--;

    g[i].emplace_back(j, o);
    g[j].emplace_back(i, o);
    deg[o][i]++;
    deg[o][j]++;
  }

  queue<int> q;
  vector<bool> vis(n);
  vector<pair<int, int>> t;

  for (int i = 0; i < n; i++)
    if (deg[0][i] == 0 || deg[1][i] == 0) {
      vis[i] = true;
      q.push(i);
    }

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    if (deg[0][u] == 0) {
      t.emplace_back(u, -1);
    } else {
      t.emplace_back(u, 1);
    }

    for (auto [v, o] : g[u]) {
      deg[o][v]--;
      if (deg[o][v] == 0 && !vis[v]) {
        vis[v] = true;
        q.push(v);
      }
    }
  }

  if (t.size() < n) {
    cout << "NO" << endl;
    return;
  }

  vector<int> ans(n);
  reverse(t.begin(), t.end());
  for (int i = 0; i < n; i++) {
    ans[t[i].first] = (i + 1) * t[i].second;
  }

  cout << "YES" << endl;
  for (int i = 0; i < n; i++) {
    cout << ans[i] << ' ';
  }
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n >> m;
    solve();
  }

  return 0;
}

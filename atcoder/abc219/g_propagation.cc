// ABC219 G — Propagation
// https://atcoder.jp/contests/abc219/tasks/abc219_g
//
// Heavy/light by degree (~sqrt(M)): light vertices push to neighbors;
// heavy vertices store a signboard and neighbors pull lazily.

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m, q;
  cin >> n >> m >> q;
  vector<vector<int>> adj(n + 1);
  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  vector<int> queries(q);
  for (int i = 0; i < q; ++i) {
    cin >> queries[i];
  }

  const int th = max(1, (int)sqrt(m) + 1);
  vector<char> heavy(n + 1);
  for (int i = 1; i <= n; ++i) {
    heavy[i] = (int)adj[i].size() >= th;
  }

  vector<vector<int>> heavy_adj(n + 1);
  for (int u = 1; u <= n; ++u) {
    for (int v : adj[u]) {
      if (heavy[v]) {
        heavy_adj[u].push_back(v);
      }
    }
  }

  vector<int> val(n + 1);
  iota(val.begin(), val.end(), 0);
  vector<int> last(n + 1, -1);
  vector<int> done(n + 1, -1);
  vector<int> signboard(n + 1);

  auto sync = [&](int u) {
    for (int h : heavy_adj[u]) {
      if (done[h] > last[u]) {
        last[u] = done[h];
        val[u] = signboard[h];
      }
    }
  };

  for (int i = 0; i < q; ++i) {
    const int x = queries[i];
    sync(x);
    last[x] = i;
    signboard[x] = val[x];
    if (heavy[x]) {
      done[x] = i;
    } else {
      for (int y : adj[x]) {
        val[y] = val[x];
        last[y] = i;
      }
    }
  }

  for (int i = 1; i <= n; ++i) {
    sync(i);
    if (i > 1) {
      cout << ' ';
    }
    cout << val[i];
  }
  cout << '\n';

  return 0;
}

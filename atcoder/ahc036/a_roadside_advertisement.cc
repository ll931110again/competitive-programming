// AHC036 A - Roadside Advertisement: https://atcoder.jp/contests/ahc036/tasks/ahc036_a
//
// BFS between targets; copy signal for the next vertex, then move. initial_a
// stores the green-light sequence.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

struct Input {
  int n, m, t_sz, al, bl;
  vector<pair<int, int>> edges;
  vector<int> targets;
};

vector<vector<int>> g;

vector<int> bfs_path(int s, int t) {
  const int n = (int)g.size();
  vector<int> par(n, -1);
  queue<int> q;
  par[s] = s;
  q.push(s);
  while (!q.empty()) {
    const int v = q.front();
    q.pop();
    if (v == t)
      break;
    for (int u : g[v])
      if (par[u] == -1) {
        par[u] = v;
        q.push(u);
      }
  }
  if (par[t] == -1)
    return {};
  vector<int> path;
  for (int v = t; v != s; v = par[v])
    path.push_back(v);
  reverse(path.begin(), path.end());
  return path;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Input in;
  cin >> in.n >> in.m >> in.t_sz >> in.al >> in.bl;
  in.edges.resize(in.m);
  for (int i = 0; i < in.m; ++i)
    cin >> in.edges[i].first >> in.edges[i].second;
  in.targets.resize(in.t_sz);
  for (int i = 0; i < in.t_sz; ++i)
    cin >> in.targets[i];
  vector<pair<long long, long long>> coords(in.n);
  for (int i = 0; i < in.n; ++i)
    cin >> coords[i].first >> coords[i].second;

  g.assign(in.n, {});
  for (auto [u, v] : in.edges) {
    g[u].push_back(v);
    g[v].push_back(u);
  }

  vector<int> green_seq;
  int cur = 0;
  for (int tgt : in.targets) {
    auto path = bfs_path(cur, tgt);
    if (path.empty() && cur != tgt)
      continue;
    for (int v : path) {
      green_seq.push_back(v);
      cur = v;
    }
  }

  vector<int> init_a(in.al, 0);
  for (int i = 0; i < (int)green_seq.size() && i < in.al; ++i)
    init_a[i] = green_seq[i];

  for (int i = 0; i < in.al; ++i) {
    if (i)
      cout << ' ';
    cout << init_a[i];
  }
  cout << '\n';

  int pos = 0;
  cur = 0;
  for (int tgt : in.targets) {
    auto path = bfs_path(cur, tgt);
    for (int v : path) {
      cout << "s 1 " << pos << " 0\n";
      cout << "m " << v << '\n';
      ++pos;
      if (pos >= in.al)
        pos = 0;
      cur = v;
    }
  }
  return 0;
}

// AHC045 A - Oracle-Guided Road Network Planning: https://atcoder.jp/contests/ahc045/tasks/ahc045_a
//
// Rectangle-centroid grouping + oracle MST queries (official sample strategy).

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

struct Edge {
  int u, v;
};

vector<Edge> query(const vector<int>& c) {
  cout << "? " << c.size();
  for (const int x : c)
    cout << ' ' << x;
  cout << endl;
  const int m = (int)c.size() - 1;
  vector<Edge> e(m);
  for (int i = 0; i < m; i++)
    cin >> e[i].u >> e[i].v;
  return e;
}

void answer(const vector<vector<int>>& groups, const vector<vector<Edge>>& edges) {
  cout << "!" << endl;
  for (int k = 0; k < (int)groups.size(); k++) {
    for (int i = 0; i < (int)groups[k].size(); i++) {
      if (i)
        cout << ' ';
      cout << groups[k][i];
    }
    cout << '\n';
    for (const auto& ed : edges[k])
      cout << ed.u << ' ' << ed.v << '\n';
  }
}


}  // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M, Q, L, W;
  cin >> N >> M >> Q >> L >> W;
  vector<int> G(M);
  for (int i = 0; i < M; i++)
    cin >> G[i];
  vector<int> lx(N), rx(N), ly(N), ry(N);
  for (int i = 0; i < N; i++)
    cin >> lx[i] >> rx[i] >> ly[i] >> ry[i];

  vector<int> x(N), y(N);
  for (int i = 0; i < N; i++) {
    x[i] = (lx[i] + rx[i]) / 2;
    y[i] = (ly[i] + ry[i]) / 2;
  }

  vector<int> cities(N);
  iota(cities.begin(), cities.end(), 0);
  sort(cities.begin(), cities.end(), [&](int a, int b) {
    if (x[a] != x[b])
      return x[a] < x[b];
    return y[a] < y[b];
  });

  vector<vector<int>> groups(M);
  int st = 0;
  for (int k = 0; k < M; k++) {
    groups[k].assign(cities.begin() + st, cities.begin() + st + G[k]);
    st += G[k];
  }

  vector<vector<Edge>> edges(M);
  int used = 0;
  for (int k = 0; k < M; k++) {
    const int sz = G[k];
    for (int i = 0; i < sz - 1; i += 2) {
      if (used >= Q)
        break;
      if (i + 2 < sz - 1) {
        vector<int> q = {groups[k][i], groups[k][i + 1], groups[k][i + 2]};
        auto ret = query(q);
        edges[k].insert(edges[k].end(), ret.begin(), ret.end());
        used++;
      } else {
        edges[k].push_back({groups[k][i], groups[k][i + 1]});
      }
    }
    while ((int)edges[k].size() < sz - 1) {
      const int u = groups[k][edges[k].size() % max(1, sz - 1)];
      const int v = groups[k][(edges[k].size() + 1) % sz];
      edges[k].push_back({u, v});
    }
    if ((int)edges[k].size() > sz - 1)
      edges[k].resize(sz - 1);
  }

  answer(groups, edges);
  return 0;
}

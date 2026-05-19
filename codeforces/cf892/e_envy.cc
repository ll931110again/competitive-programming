// CF 892E — Envy
// https://codeforces.com/contest/892/problem/E
//
// Offline: sort each query's edges by weight; process weights ascending with
// rollback DSU — temporary merges detect cycles (NO), permanent merges add edges.

#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <iostream>
#include <vector>

using namespace std;

struct Edge {
  int u, v, w, id;
};

struct RollbackDsu {
  vector<int> fa, sz;
  vector<pair<int, int>> st;

  void init(int n) {
    fa.resize(n + 1);
    sz.assign(n + 1, 1);
    for (int i = 1; i <= n; ++i) {
      fa[i] = i;
    }
    st.clear();
  }

  int find(int x) const {
    while (fa[x] != x) {
      x = fa[x];
    }
    return x;
  }

  bool unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) {
      return false;
    }
    if (sz[a] < sz[b]) {
      swap(a, b);
    }
    st.emplace_back(a, b);
    fa[b] = a;
    sz[a] += sz[b];
    return true;
  }

  int snapshot() const {
    return static_cast<int>(st.size());
  }

  void rollback(int t) {
    while (static_cast<int>(st.size()) > t) {
      const auto [parent, child] = st.back();
      st.pop_back();
      sz[parent] -= sz[child];
      fa[child] = child;
    }
  }
};

struct Seg {
  int id, l, r;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  vector<Edge> e(m + 1);
  for (int i = 1; i <= m; ++i) {
    cin >> e[i].u >> e[i].v >> e[i].w;
  }

  int q;
  cin >> q;
  vector<vector<int>> qedges(q + 1);
  for (int i = 1; i <= q; ++i) {
    int k;
    cin >> k;
    qedges[i].resize(k);
    for (int j = 0; j < k; ++j) {
      cin >> qedges[i][j];
    }
    sort(qedges[i].begin(), qedges[i].end(), [&](int a, int b) { return e[a].w < e[b].w; });
  }

  const int wmax = 500000 + 5;
  vector<vector<int>> bucket(wmax);
  vector<vector<Seg>> segs(wmax);
  for (int i = 1; i <= m; ++i) {
    bucket[e[i].w].push_back(i);
  }

  for (int i = 1; i <= q; ++i) {
    int from = 0;
    const auto& qe = qedges[i];
    for (int j = 1; j < static_cast<int>(qe.size()); ++j) {
      if (e[qe[j]].w != e[qe[j - 1]].w) {
        segs[e[qe[j - 1]].w].push_back({i, from, j - 1});
        from = j;
      }
    }
    if (!qe.empty()) {
      segs[e[qe.back()].w].push_back({i, from, static_cast<int>(qe.size()) - 1});
    }
  }

  vector<int> bad(q + 1, 0);
  RollbackDsu dsu;
  dsu.init(n);

  for (int w = 1; w < wmax; ++w) {
    for (const auto& s : segs[w]) {
      const int snap = dsu.snapshot();
      bool ok = true;
      for (int j = s.l; j <= s.r; ++j) {
        if (!dsu.unite(e[qedges[s.id][j]].u, e[qedges[s.id][j]].v)) {
          ok = false;
          break;
        }
      }
      if (!ok) {
        bad[s.id] = 1;
      }
      dsu.rollback(snap);
    }
    for (int id : bucket[w]) {
      dsu.unite(e[id].u, e[id].v);
    }
  }

  for (int i = 1; i <= q; ++i) {
    cout << (bad[i] ? "NO" : "YES") << '\n';
  }
  return 0;
}

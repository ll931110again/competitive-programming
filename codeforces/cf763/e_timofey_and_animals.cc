// CF 763E — Timofey and our friends animals
// https://codeforces.com/contest/763/problem/E
//
// Answer for [l, r] = # connected components in the friendship graph on vertices [l, r].
// Mo's algorithm on l with rollback DSU for left extension; permanent right extension.

#include <bits/stdc++.h>
using namespace std;

namespace {

struct RollbackDsu {
  vector<int> p, sz;
  struct Change {
    int x, y, px, sy;
  };
  vector<Change> st;
  int comps = 0;

  void clear(int n) {
    p.resize(n + 1);
    sz.assign(n + 1, 1);
    for (int i = 1; i <= n; ++i) {
      p[i] = i;
    }
    st.clear();
    comps = 0;
  }

  int find(int x) const {
    while (p[x] != x) {
      x = p[x];
    }
    return x;
  }

  // Add v and unite with neighbors already in [L, R] (both directions).
  void add_vertex(int v, int L, int R, const vector<vector<int>>& adj) {
    ++comps;
    for (int u : adj[v]) {
      if (L <= u && u <= R && unite(u, v)) {
        --comps;
      }
    }
  }

  bool unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) {
      return false;
    }
    if (sz[x] > sz[y]) {
      swap(x, y);
    }
    st.push_back({x, y, p[x], sz[y]});
    p[x] = y;
    sz[y] += sz[x];
    return true;
  }

  int snapshot() const {
    return static_cast<int>(st.size());
  }

  void rollback(int t) {
    while (static_cast<int>(st.size()) > t) {
      Change c = st.back();
      st.pop_back();
      sz[c.y] = c.sy;
      p[c.x] = c.px;
      ++comps;
    }
  }
};

struct Query {
  int l, r, id;
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, k;
  cin >> n >> k;
  int m;
  cin >> m;

  vector<vector<int>> adj(n + 1);
  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  int q;
  cin >> q;
  vector<Query> queries(q);
  for (int i = 0; i < q; ++i) {
    cin >> queries[i].l >> queries[i].r;
    queries[i].id = i;
  }

  const int B = max(1, static_cast<int>(sqrt(n)) + 1);
  vector<int> ans(q);

  auto brute = [&](const Query& qu) {
    vector<int> parent(n + 1);
    for (int i = 1; i <= n; ++i) {
      parent[i] = i;
    }
    auto find = [&](int x) {
      while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
      }
      return x;
    };
    int comps = 0;
    for (int v = qu.l; v <= qu.r; ++v) {
      ++comps;
      for (int u : adj[v]) {
        if (qu.l <= u && u <= qu.r) {
          int a = find(v);
          int b = find(u);
          if (a != b) {
            parent[b] = a;
            --comps;
          }
        }
      }
    }
    ans[qu.id] = comps;
  };

  vector<Query> big;
  for (const auto& qu : queries) {
    if (qu.r - qu.l <= B) {
      brute(qu);
    } else {
      big.push_back(qu);
    }
  }

  auto block_id = [&](int l) { return (l - 1) / B; };
  auto block_end = [&](int b) { return min(n, (b + 1) * B); };

  sort(big.begin(), big.end(), [&](const Query& a, const Query& b) {
    int ba = block_id(a.l);
    int bb = block_id(b.l);
    if (ba != bb) {
      return ba < bb;
    }
    return a.r < b.r;
  });

  RollbackDsu dsu;
  int cur_block = -1;
  int cur_r = 0;
  int fixed_l = 0;

  for (const auto& qu : big) {
    int b = block_id(qu.l);
    if (b != cur_block) {
      cur_block = b;
      fixed_l = block_end(b) + 1;
      cur_r = block_end(b);
      dsu.clear(n);
    }
    // Permanent right extension: only merge neighbors in [max(fixed_l, L), R].
    while (cur_r < qu.r) {
      ++cur_r;
      dsu.add_vertex(cur_r, max(fixed_l, qu.l), qu.r, adj);
    }
    const int snap = dsu.snapshot();
    const int saved = dsu.comps;
    // Left extension: vertices [L, min(R, fixed_l - 1)] not yet added on the right.
    int cur_l = min(qu.r, fixed_l - 1);
    while (cur_l >= qu.l) {
      dsu.add_vertex(cur_l, qu.l, qu.r, adj);
      ans[qu.id] = dsu.comps;
      --cur_l;
    }
    dsu.rollback(snap);
    dsu.comps = saved;
  }

  for (int i = 0; i < q; ++i) {
    cout << ans[i] << '\n';
  }
  return 0;
}

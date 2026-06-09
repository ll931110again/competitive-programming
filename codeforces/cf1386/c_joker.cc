// CF 1386C — Joker
// https://codeforces.com/contest/1386/problem/C
//
// Odd cycle in streets NOT in [l, r]  <=>  graph without monitored edges is not bipartite.
// DSU with parity + rollback queue (Codeforces blog 83467).

#include <bits/stdc++.h>
using namespace std;

namespace {

struct BipartiteDsu {
  vector<int> par, sz, parity;
  vector<array<int, 3>> ops;
  bool ok = true;

  explicit BipartiteDsu(int n) {
    par.resize(n + 1);
    sz.assign(n + 1, 1);
    parity.assign(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
      par[i] = i;
    }
  }

  pair<int, int> find(int u) {
    int flip = 0;
    while (par[u] != u) {
      flip ^= parity[u];
      u = par[u];
    }
    return {u, flip};
  }

  void merge(int u, int v) {
    auto pu = find(u);
    auto pv = find(v);
    u = pu.first;
    v = pv.first;
    const bool last = ok;
    const int need = pu.second ^ pv.second ^ 1;
    if (u == v) {
      if (need) {
        ok = false;
      }
      ops.push_back({-1, -1, static_cast<int>(last)});
      return;
    }
    if (sz[u] > sz[v]) {
      swap(u, v);
    }
    ops.push_back({u, parity[u], static_cast<int>(last)});
    parity[u] = need;
    par[u] = v;
    sz[v] += sz[u];
  }

  void undo() {
    auto x = ops.back();
    ops.pop_back();
    ok = x[2];
    const int u = x[0];
    if (u == -1) {
      return;
    }
    sz[par[u]] -= sz[u];
    par[u] = u;
    parity[u] = x[1];
  }

  bool bipartite() const {
    return ok;
  }
};

struct Update {
  int x, y;
  int type = 0;
  Update(int _x, int _y) : x(_x), y(_y) {}
};

struct DsuQueue {
  BipartiteDsu dsu;
  vector<Update> stack;

  explicit DsuQueue(int n) : dsu(n) {}

  void push(Update u) {
    dsu.merge(u.x, u.y);
    stack.push_back(u);
  }

  void pop() {
    vector<Update> batch[2];
    do {
      batch[stack.back().type].push_back(stack.back());
      stack.pop_back();
      dsu.undo();
    } while (batch[1].size() < batch[0].size() && !stack.empty());

    if (batch[1].empty()) {
      for (auto& u : batch[0]) {
        u.type = 1;
        push(u);
      }
    } else {
      for (int t : {0, 1}) {
        reverse(batch[t].begin(), batch[t].end());
        for (auto& u : batch[t]) {
          push(u);
        }
      }
    }
    stack.pop_back();
    dsu.undo();
  }

  bool bipartite() const {
    return dsu.bipartite();
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m, q;
  cin >> n >> m >> q;
  vector<int> u(m + 1), v(m + 1);
  for (int i = 1; i <= m; ++i) {
    cin >> u[i] >> v[i];
  }

  DsuQueue dq(n);
  for (int i = 1; i <= m; ++i) {
    dq.push(Update(u[i], v[i]));
  }

  vector<int> limit(m + 1);
  for (int l = 1, r = 1; l <= m; ++l) {
    while (r < l || (!dq.bipartite() && r <= m)) {
      dq.pop();
      ++r;
    }
    limit[l] = dq.bipartite() ? r - 1 : m + 1;
    dq.push(Update(u[l], v[l]));
  }

  while (q--) {
    int l, r;
    cin >> l >> r;
    // Joker wins (odd cycle) iff unmonitored graph is not bipartite.
    cout << (limit[l] <= r ? "NO" : "YES") << '\n';
  }
  return 0;
}

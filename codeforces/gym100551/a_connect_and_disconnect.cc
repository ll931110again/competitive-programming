// Gym 100551A — Connect and Disconnect
// https://codeforces.com/problemset/gymProblem/100551/A
//
// Offline dynamic connectivity: edge active on [add_time, remove_time); segment tree
// over query time + rollback DSU.

#include "../../lib/dsu_rollback.hh"
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

struct EdgeInterval {
  int u, v, l, r;
};

i64 pack_edge(int u, int v, int n) {
  if (u > v) {
    swap(u, v);
  }
  return 1LL * u * (n + 1) + v;
}

int n, k;
vector<EdgeInterval> edges;
vector<vector<int>> seg;
vector<int> answer_at_time;
DsuRollback dsu;

void seg_add(int node, int tl, int tr, int l, int r, int id) {
  if (l >= tr || r <= tl) {
    return;
  }
  if (l <= tl && tr <= r) {
    seg[node].push_back(id);
    return;
  }
  const int tm = (tl + tr) / 2;
  seg_add(node * 2, tl, tm, l, r, id);
  seg_add(node * 2 + 1, tm, tr, l, r, id);
}

void dfs(int node, int tl, int tr, int components) {
  const int snap = dsu.snapshot();
  for (int id : seg[node]) {
    if (dsu.unite(edges[id].u, edges[id].v)) {
      --components;
    }
  }
  if (tr - tl == 1) {
    answer_at_time[tl] = components;
  } else {
    const int tm = (tl + tr) / 2;
    dfs(node * 2, tl, tm, components);
    dfs(node * 2 + 1, tm, tr, components);
  }
  dsu.rollback(snap);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ifstream fin("connect.in");
  ofstream fout("connect.out");
  if (!fin) {
    fin.open("/dev/stdin");
    fout.open("/dev/stdout");
  }

  fin >> n >> k;
  dsu = DsuRollback(n);

  unordered_map<i64, int> edge_start;
  vector<int> query_times;

  for (int t = 0; t < k; ++t) {
    char op;
    fin >> op;
    if (op == '?') {
      query_times.push_back(t);
      continue;
    }
    int u, v;
    fin >> u >> v;
    --u;
    --v;
    const i64 key = pack_edge(u, v, n);
    if (op == '+') {
      edge_start[key] = t;
    } else {
      const int l = edge_start[key];
      edges.push_back({u, v, l, t});
      edge_start.erase(key);
    }
  }

  for (const auto& [key, l] : edge_start) {
    const int u = static_cast<int>(key / (n + 1));
    const int v = static_cast<int>(key % (n + 1));
    edges.push_back({u, v, l, k});
  }

  answer_at_time.assign(k, n);
  seg.assign(4 * max(k, 1), {});
  for (int i = 0; i < static_cast<int>(edges.size()); ++i) {
    seg_add(1, 0, k, edges[i].l, edges[i].r, i);
  }
  if (k > 0) {
    dfs(1, 0, k, n);
  }

  for (int t : query_times) {
    fout << answer_at_time[t] << '\n';
  }
  return 0;
}

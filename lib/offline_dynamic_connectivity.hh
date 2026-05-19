#pragma once

#include "dsu_rollback.hh"

#include <functional>
#include <vector>

// Offline dynamic connectivity via segment tree over time + rollback DSU.
//
// Each edge is active on a time interval [add_time, remove_time).
// Build a segment tree over [0, q); at each node, unite all edges covering that
// segment, answer queries at leaves, rollback on exit.
//
// Usage:
//   OfflineDynamicConnectivity odc(n);
//   odc.add_edge(u, v, l, r);  // edge active for queries in [l, r)
//   odc.build(q);
//   odc.run([&](int t) { /* answer connectivity query at time t */ });

struct OfflineDynamicConnectivity {
  struct EdgeInterval {
    int u, v, l, r;
  };

  int n = 0;
  int q = 0;
  std::vector<EdgeInterval> edges;
  std::vector<std::vector<int>> seg;
  DsuRollback dsu;

  explicit OfflineDynamicConnectivity(int n_) : n(n_), dsu(n_) {}

  void add_edge(int u, int v, int l, int r) {
    edges.push_back({u, v, l, r});
  }

  void build(int query_count) {
    q = query_count;
    seg.assign(4 * q, {});
    std::function<void(int, int, int, int, int, int)> add = [&](int node, int tl, int tr, int l,
                                                                int r, int id) {
      if (l >= tr || r <= tl) {
        return;
      }
      if (l <= tl && tr <= r) {
        seg[node].push_back(id);
        return;
      }
      const int tm = (tl + tr) / 2;
      add(node * 2, tl, tm, l, r, id);
      add(node * 2 + 1, tm, tr, l, r, id);
    };
    for (int i = 0; i < static_cast<int>(edges.size()); ++i) {
      add(1, 0, q, edges[i].l, edges[i].r, i);
    }
  }

  template <class AnswerFn> void run(AnswerFn answer_at_time) {
    std::function<void(int, int, int)> dfs = [&](int node, int tl, int tr) {
      const int snap = dsu.snapshot();
      for (int id : seg[node]) {
        dsu.unite(edges[id].u, edges[id].v);
      }
      if (tr - tl == 1) {
        answer_at_time(tl);
      } else {
        const int tm = (tl + tr) / 2;
        dfs(node * 2, tl, tm);
        dfs(node * 2 + 1, tm, tr);
      }
      dsu.rollback(snap);
    };
    if (q > 0) {
      dfs(1, 0, q);
    }
  }
};

#pragma once

#include <tuple>
#include <utility>
#include <vector>

// Rollback DSU tracking the number of odd-sized components.
// Sunny paving (all degrees odd) exists iff odd_count == 0.
struct DsuRollbackOdd {
  std::vector<int> p;
  std::vector<int> sz;
  int n = 0;
  int odd_count = 0;
  struct Change {
    int x, y, delta;
  };
  std::vector<Change> ops;

  DsuRollbackOdd() = default;

  explicit DsuRollbackOdd(int n_) : n(n_), odd_count(n_) {
    p.resize(n + 1);
    sz.assign(n + 1, 1);
    for (int i = 1; i <= n; ++i) {
      p[i] = i;
    }
  }

  int find(int x) const {
    while (p[x] != x) {
      x = p[x];
    }
    return x;
  }

  void unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) {
      return;
    }
    if (sz[x] > sz[y]) {
      std::swap(x, y);
    }
    int delta = 0;
    if ((sz[x] & 1) && (sz[y] & 1)) {
      delta = 2;
    }
    odd_count -= delta;
    ops.push_back({x, y, delta});
    p[x] = y;
    sz[y] += sz[x];
  }

  int snapshot() const {
    return static_cast<int>(ops.size());
  }

  void rollback(int t) {
    while (static_cast<int>(ops.size()) > t) {
      Change c = ops.back();
      ops.pop_back();
      odd_count += c.delta;
      sz[c.y] -= sz[c.x];
      p[c.x] = c.x;
    }
  }
};

#pragma once

#include <tuple>
#include <utility>
#include <vector>

// Rollback DSU: union by size, find without path compression.
// snapshot() / rollback(t) undo unite operations since token t.
struct DsuRollback {
  std::vector<int> p;
  std::vector<int> sz;
  int n = 0;
  std::vector<std::tuple<int, int, int>> ops;

  DsuRollback() = default;

  explicit DsuRollback(int n_) : n(n_) {
    p.resize(n);
    sz.assign(n, 1);
    for (int i = 0; i < n; ++i) {
      p[i] = i;
    }
  }

  int find(int x) const {
    while (p[x] != x) {
      x = p[x];
    }
    return x;
  }

  bool unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) {
      return false;
    }
    if (sz[x] > sz[y]) {
      std::swap(x, y);
    }
    ops.emplace_back(0, x, p[x]);
    ops.emplace_back(1, y, sz[y]);
    p[x] = y;
    sz[y] += sz[x];
    return true;
  }

  int snapshot() const {
    return static_cast<int>(ops.size());
  }

  void rollback(int t) {
    while (static_cast<int>(ops.size()) > t) {
      auto [kind, b, c] = ops.back();
      ops.pop_back();
      if (kind == 0) {
        p[b] = c;
      } else {
        sz[b] = c;
      }
    }
  }
};

// Rollback DSU with per-root "marked color id" (first marked vertex in component).
// Used when merging graph components must track which query vertices met.
struct DsuRollbackMarked {
  std::vector<int> p;
  std::vector<int> sz;
  int n = 0;
  std::vector<int> idx; // -1 if no marked vertex in component, else color id
  std::vector<std::tuple<int, int, int>> ops;

  DsuRollbackMarked() = default;

  explicit DsuRollbackMarked(int n_, const std::vector<int>& marked_idx) : n(n_), idx(marked_idx) {
    p.resize(n);
    sz.assign(n, 1);
    for (int i = 0; i < n; ++i) {
      p[i] = i;
    }
  }

  int find(int x) const {
    while (p[x] != x) {
      x = p[x];
    }
    return x;
  }

  bool unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) {
      return false;
    }
    if (sz[x] > sz[y]) {
      std::swap(x, y);
    }
    ops.emplace_back(0, x, p[x]);
    ops.emplace_back(1, y, sz[y]);
    ops.emplace_back(2, y, idx[y]);
    p[x] = y;
    sz[y] += sz[x];
    if (idx[y] == -1) {
      idx[y] = idx[x];
    }
    return true;
  }

  int snapshot() const {
    return static_cast<int>(ops.size());
  }

  void rollback(int t) {
    while (static_cast<int>(ops.size()) > t) {
      auto [kind, b, c] = ops.back();
      ops.pop_back();
      if (kind == 0) {
        p[b] = c;
      } else if (kind == 1) {
        sz[b] = c;
      } else {
        idx[b] = c;
      }
    }
  }
};

#pragma once

#include <numeric>
#include <vector>

// Disjoint set union with path compression (no rollback).
struct Dsu {
  std::vector<int> p;
  int n = 0;

  Dsu() = default;

  explicit Dsu(int n_) : n(n_) {
    p.resize(n);
    std::iota(p.begin(), p.end(), 0);
  }

  int find(int x) {
    return p[x] == x ? x : (p[x] = find(p[x]));
  }

  bool unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) {
      return false;
    }
    p[x] = y;
    return true;
  }
};

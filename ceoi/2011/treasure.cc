/*
 * Solution for CEOI 2011 Day 1: Treasure Hunt.
 *
 * Interactive tree built by path extensions. Node ids are contiguous, so parent
 * arrays and binary lifting support fast LCA and dig queries.
 */

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

vector<int> parent_nodes = {0, 1};
vector<int> depth_nodes = {0, 0};
vector<vector<int>> ancestors;
int log_levels = 1;

void rebuild_ancestors() {
  int node_count = static_cast<int>(parent_nodes.size());
  log_levels = 1;
  while ((1 << log_levels) < node_count) {
    log_levels++;
  }
  ancestors.assign(node_count, vector<int>(log_levels, 1));
  for (int node = 1; node < node_count; node++) {
    ancestors[node][0] = parent_nodes[node];
    for (int level = 1; level < log_levels; level++) {
      ancestors[node][level] = ancestors[ancestors[node][level - 1]][level - 1];
    }
  }
}

void add_node(int previous) {
  parent_nodes.push_back(previous);
  depth_nodes.push_back(depth_nodes[previous] + 1);
  rebuild_ancestors();
}

int go_up(int node, i64 steps) {
  for (int level = 0; steps > 0; level++) {
    if ((steps & 1LL) != 0) {
      node = ancestors[node][level];
    }
    steps >>= 1LL;
  }
  return node;
}

int lca(int left, int right) {
  if (depth_nodes[left] < depth_nodes[right]) {
    swap(left, right);
  }

  int depth_gap = depth_nodes[left] - depth_nodes[right];
  left = go_up(left, depth_gap);
  if (left == right) {
    return left;
  }

  for (int level = log_levels - 1; level >= 0; level--) {
    if (ancestors[left][level] != ancestors[right][level]) {
      left = ancestors[left][level];
      right = ancestors[right][level];
    }
  }
  return parent_nodes[left];
}

} // namespace

void init() {
  parent_nodes = {0, 1};
  depth_nodes = {0, 0};
  rebuild_ancestors();
}

void path(int start, int steps) {
  int previous = start;
  for (int step = 0; step < steps; step++) {
    add_node(previous);
    previous = static_cast<int>(parent_nodes.size()) - 1;
  }
}

int dig(int left, int right) {
  int ancestor = lca(left, right);
  i64 path_length = depth_nodes[left] + depth_nodes[right] - 2 * depth_nodes[ancestor];
  i64 half = path_length / 2;
  if (half <= depth_nodes[left] - depth_nodes[ancestor]) {
    return go_up(left, half);
  }
  return go_up(right, path_length - half);
}

#ifdef TREASURE_LOCAL_GRADER
#include <cstdio>

int main() {
  int queries;
  if (scanf("%d", &queries) != 1) {
    return 0;
  }
  for (int i = 0; i < queries; i++) {
    char kind;
    int a, b;
    scanf(" %c %d %d", &kind, &a, &b);
    if (kind == 'i') {
      init();
    } else if (kind == 'p') {
      path(a, b);
    } else {
      printf("%d\n", dig(a, b));
    }
  }
  return 0;
}
#endif

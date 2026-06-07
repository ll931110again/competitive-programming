// CF 295E — Yaroslav and Points
// https://codeforces.com/contest/295/problem/E
//
// Implicit treap in sorted-by-coordinate order. Each node stores subtree size,
// coordinate sum, and sum of pairwise distances (res). Type 1 moves a point by
// erase/insert; type 2 splits out [L, R] by coordinate and reads res.

#include <cstdio>
#include <vector>

struct Node {
  int ch[2]{};
  int prior = 0;
  int key = 0;
  int size = 1;
  long long sum = 0;
  long long pairs = 0;
};

constexpr int kMaxNodes = 200005;

Node pool[kMaxNodes];
int pool_used = 0;
int root = 0;

int new_node(int coord) {
  Node& node = pool[++pool_used];
  node.key = coord;
  node.prior = rand();
  node.ch[0] = node.ch[1] = 0;
  node.size = 1;
  node.sum = coord;
  node.pairs = 0;
  return pool_used;
}

int size(int t) {
  return t ? pool[t].size : 0;
}
long long sum(int t) {
  return t ? pool[t].sum : 0;
}
long long pairs(int t) {
  return t ? pool[t].pairs : 0;
}

void pull(int t) {
  if (!t) {
    return;
  }
  Node& node = pool[t];
  const int left = node.ch[0];
  const int right = node.ch[1];
  node.size = size(left) + 1 + size(right);
  node.sum = sum(left) + sum(right) + node.key;
  node.pairs = pairs(left) + pairs(right) + static_cast<long long>(size(left)) * sum(right) -
               static_cast<long long>(size(right)) * sum(left) +
               static_cast<long long>(node.key) * (size(left) - size(right)) + sum(right) -
               sum(left);
}

void merge_nodes(int& t, int left, int right) {
  if (!left) {
    t = right;
    return;
  }
  if (!right) {
    t = left;
    return;
  }
  if (pool[left].prior > pool[right].prior) {
    merge_nodes(pool[left].ch[1], pool[left].ch[1], right);
    t = left;
  } else {
    merge_nodes(pool[right].ch[0], left, pool[right].ch[0]);
    t = right;
  }
  pull(t);
}

void split_key(int t, int pivot, int& left, int& right) {
  if (!t) {
    left = right = 0;
    return;
  }
  if (pool[t].key <= pivot) {
    split_key(pool[t].ch[1], pivot, pool[t].ch[1], right);
    left = t;
  } else {
    split_key(pool[t].ch[0], pivot, left, pool[t].ch[0]);
    right = t;
  }
  pull(t);
}

void insert(int coord) {
  int left = 0;
  int right = 0;
  split_key(root, coord, left, right);
  merge_nodes(root, left, new_node(coord));
  merge_nodes(root, root, right);
}

void erase(int coord) {
  int left = 0;
  int rest = 0;
  split_key(root, coord, left, rest);
  int node = 0;
  split_key(left, coord - 1, left, node);
  merge_nodes(root, left, rest);
}

long long query_range(int l, int r) {
  int left = 0;
  int middle = 0;
  int right = 0;
  split_key(root, l - 1, left, middle);
  split_key(middle, r, middle, right);
  const long long answer = pairs(middle);
  merge_nodes(root, left, middle);
  merge_nodes(root, root, right);
  return answer;
}

int main() {
  int n = 0;
  std::scanf("%d", &n);
  std::vector<int> coord(n);
  for (int i = 0; i < n; ++i) {
    std::scanf("%d", &coord[i]);
    insert(coord[i]);
  }

  int m = 0;
  std::scanf("%d", &m);
  while (m--) {
    int type = 0;
    std::scanf("%d", &type);
    if (type == 1) {
      int p = 0;
      int delta = 0;
      std::scanf("%d %d", &p, &delta);
      --p;
      erase(coord[p]);
      coord[p] += delta;
      insert(coord[p]);
    } else {
      int l = 0;
      int r = 0;
      std::scanf("%d %d", &l, &r);
      std::printf("%lld\n", query_range(l, r));
    }
  }
  return 0;
}

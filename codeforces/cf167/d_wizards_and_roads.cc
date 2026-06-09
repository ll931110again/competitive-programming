// CF 167D — Wizards and Roads
// https://codeforces.com/problemset/problem/167/D
//
// Road graph is the Cartesian tree on (x, y): BST by x, heap by y. Each node
// stores max matching DP; query [L, R] splits the treap by x and reads res.

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Node {
  int ch[2]{};
  int key = 0;
  int prior = 0;
  int candidate = 0;
  int match = 0;
};

constexpr int k_mod = 1'000'000'009;
constexpr int k_max_nodes = 100005;

Node pool[k_max_nodes];
int pool_used = 0;
int root = 0;

int new_node(int x, int y) {
  Node& node = pool[++pool_used];
  node.key = x;
  node.prior = y;
  node.ch[0] = node.ch[1] = 0;
  node.candidate = node.match = 0;
  return pool_used;
}

int match_value(int t) {
  return t ? pool[t].match : 0;
}
int candidate_value(int t) {
  return t ? pool[t].candidate : -1;
}

void pull(int t) {
  if (!t) {
    return;
  }
  Node& node = pool[t];
  node.candidate = match_value(node.ch[0]) + match_value(node.ch[1]);
  node.match = node.candidate;
  const int left_take = candidate_value(node.ch[0]) + match_value(node.ch[1]) + 1;
  const int right_take = match_value(node.ch[0]) + candidate_value(node.ch[1]) + 1;
  if (left_take > node.match) {
    node.match = left_take;
  }
  if (right_take > node.match) {
    node.match = right_take;
  }
}

void split(int t, int pivot, int& left, int& right) {
  if (!t) {
    left = right = 0;
    return;
  }
  Node& node = pool[t];
  if (node.key <= pivot) {
    split(node.ch[1], pivot, node.ch[1], right);
    left = t;
  } else {
    split(node.ch[0], pivot, left, node.ch[0]);
    right = t;
  }
  pull(t);
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

void insert(int& t, int node) {
  if (!t) {
    t = node;
    return;
  }
  if (pool[t].prior < pool[node].prior) {
    split(t, pool[node].key, pool[node].ch[0], pool[node].ch[1]);
    t = node;
  } else if (pool[node].key < pool[t].key) {
    insert(pool[t].ch[0], node);
  } else {
    insert(pool[t].ch[1], node);
  }
  pull(t);
}

int query(int l, int r) {
  int left = 0;
  int middle = 0;
  int right = 0;
  split(root, l - 1, left, middle);
  split(middle, r, middle, right);
  const int answer = middle ? pool[middle].match : 0;
  merge_nodes(root, left, middle);
  merge_nodes(root, root, right);
  return answer;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n = 0;
  int k = 0;
  cin >> n >> k;

  int x = 0;
  int y = 0;
  for (int i = 0; i < k; ++i) {
    cin >> x >> y;
    insert(root, new_node(x, y));
  }

  int a = 0;
  int b = 0;
  int c = 0;
  int d = 0;
  scanf("%d %d %d %d", &a, &b, &c, &d);
  for (int i = k; i < n; ++i) {
    x = (static_cast<long long>(a) * x + b) % k_mod;
    y = (static_cast<long long>(c) * y + d) % k_mod;
    insert(root, new_node(x, y));
  }

  int m = 0;
  cin >> m;
  while (m--) {
    int l = 0;
    int r = 0;
    cin >> l >> r;
    printf("%d\n", query(l, r));
  }
  return 0;
}

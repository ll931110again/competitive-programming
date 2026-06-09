// IOI 2011 Day 2 – Dancing Elephants (elephants)
//
// Link-Cut Tree (merge/split treap) solution, O(log N) per update.
// Detailed writeup: elephants_lct.md
//
// Model from the Codeforces discussion (cf. blog entry 56486) and oj.uz #55017:
// each elephant 2*i links to marker 2*i+1; each timeline event links forward in
// sorted (position, id) order. Only marker links change on update. The answer is
// the path sum from sentinel node 2*N to the root.
//
// References:
//   https://codeforces.com/blog/entry/56486
//   https://kazuma8128.hatenablog.com/entry/2018/02/18/014946
//   https://oj.uz/submission/55017

#include <bits/stdc++.h>
using namespace std;

using pii = pair<int, int>;

namespace {

constexpr int k_max_nodes = 1 << 20;
constexpr int k_inf = 2'000'000'000;
constexpr int k_offset = 400042;

int n, l;
int pos[150010];

struct Node {
  int sum = 0;
  int val = 0;
  int size = 0;
  int prior = 0;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;
  Node* path_parent = nullptr;
};

Node pool[k_max_nodes];
int pool_used = 0;

Node* new_node(int value) {
  Node* node = &pool[pool_used++];
  node->sum = node->val = value;
  node->size = 1;
  node->prior = rand();
  node->left = node->right = node->parent = node->path_parent = nullptr;
  return node;
}

inline int node_size(Node* t) {
  return t ? t->size : 0;
}
inline int node_sum(Node* t) {
  return t ? t->sum : 0;
}

void pull(Node* t) {
  if (!t) {
    return;
  }
  t->size = node_size(t->left) + node_size(t->right) + 1;
  t->sum = t->val + node_sum(t->left) + node_sum(t->right);
  t->parent = nullptr;
  if (t->left) {
    t->left->parent = t;
  }
  if (t->right) {
    t->right->parent = t;
  }
  if (t->left && t->left->path_parent) {
    t->path_parent = t->left->path_parent;
    t->left->path_parent = nullptr;
  }
  if (t->right && t->right->path_parent) {
    t->path_parent = t->right->path_parent;
    t->right->path_parent = nullptr;
  }
}

void merge_nodes(Node*& t, Node* a, Node* b) {
  if (!a) {
    t = b;
    pull(t);
    return;
  }
  if (!b) {
    t = a;
    pull(t);
    return;
  }
  if (a->prior > b->prior) {
    merge_nodes(a->right, a->right, b);
    t = a;
  } else {
    merge_nodes(b->left, a, b->left);
    t = b;
  }
  pull(t);
}

void split_nodes(Node* t, Node*& a, Node*& b, int key, int add = 0) {
  if (!t) {
    a = b = nullptr;
    return;
  }
  const int idx = add + node_size(t->left);
  if (idx <= key) {
    split_nodes(t->right, t->right, b, key, idx + 1);
    a = t;
  } else {
    split_nodes(t->left, a, t->left, key, add);
    b = t;
  }
  pull(t);
}

Node* remove_right(Node* u) {
  int index = node_size(u->left);
  Node* root = u;
  while (root->parent) {
    if (root->parent->right == root) {
      index += node_size(root->parent->left) + 1;
    }
    root = root->parent;
  }
  Node *left_part, *right_part;
  Node* saved = root->path_parent;
  root->path_parent = nullptr;
  split_nodes(root, left_part, right_part, index);
  if (right_part) {
    right_part->path_parent = u;
  }
  left_part->path_parent = saved;
  return left_part;
}

Node* remove_left(Node* u) {
  int index = node_size(u->left);
  Node* root = u;
  while (root->parent) {
    if (root->parent->right == root) {
      index += node_size(root->parent->left) + 1;
    }
    root = root->parent;
  }
  Node *left_part, *right_part;
  split_nodes(root, left_part, right_part, index - 1);
  return right_part;
}

Node* merge_trees(Node* a, Node* b) {
  Node* root = nullptr;
  if (b) {
    b->path_parent = nullptr;
  }
  merge_nodes(root, a, b);
  return root;
}

namespace LCT {
Node* tree[k_max_nodes];

Node* access(Node* u) {
  u = remove_right(u);
  while (u->path_parent) {
    Node* p = remove_right(u->path_parent);
    u = merge_trees(p, u);
  }
  return u;
}

int query(Node* u) {
  u = access(u);
  return node_sum(u);
}

void cut(Node* u) {
  access(u);
  remove_left(u);
}

void link(Node* from, Node* to) {
  from = access(from);
  to = access(to);
  merge_trees(to, from);
}

void make_node(int id, int value) {
  tree[id] = new_node(value);
}
} // namespace LCT

set<pii> timeline;

inline int fix_id(int id) {
  return id >= k_offset ? id - k_offset : id;
}

void relink(set<pii>::iterator it) {
  Node* u = LCT::tree[fix_id(it->second)];
  LCT::cut(u);

  Node* parent = nullptr;
  const int raw = fix_id(it->second);
  if (raw != 2 * n && (raw & 1) == 0) {
    parent = LCT::tree[raw + 1];
  } else {
    parent = LCT::tree[fix_id(next(it)->second)];
  }
  LCT::link(u, parent);
}

void relink_all() {
  if (timeline.size() < 2) {
    return;
  }
  const auto last = prev(timeline.end());
  for (auto it = timeline.begin(); it != last; ++it) {
    relink(it);
  }
}

void cut_node(int id) {
  LCT::cut(LCT::tree[fix_id(id)]);
}

void init(int N, int L, int X[]) {
  n = N;
  l = L;
  pool_used = 0;
  timeline.clear();

  LCT::make_node(2 * n, 0);
  LCT::make_node(2 * n + 1, 0);
  timeline.emplace(-42, 2 * n);
  timeline.emplace(k_inf, 2 * n + 1);

  for (int i = 0; i < n; ++i) {
    pos[i] = X[i];
    LCT::make_node(2 * i, 1);
    LCT::make_node(2 * i + 1, 0);
    LCT::link(LCT::tree[2 * i], LCT::tree[2 * i + 1]);
    timeline.emplace(pos[i], 2 * i);
    timeline.emplace(pos[i] + l, 2 * i + 1 + k_offset);
  }

  relink_all();
}

int update(int elephant, int value) {
  auto it_pos = timeline.lower_bound(pii(pos[elephant], 2 * elephant));
  auto it_marker = timeline.lower_bound(pii(pos[elephant] + l, 2 * elephant + 1 + k_offset));

  cut_node(it_pos->second);
  cut_node(it_marker->second);

  auto before_pos = prev(it_pos);
  auto before_marker = prev(it_marker);
  cut_node(before_pos->second);
  cut_node(before_marker->second);

  timeline.erase(it_pos);
  timeline.erase(it_marker);
  pos[elephant] = value;

  it_pos = timeline.insert(pii(pos[elephant], 2 * elephant)).first;
  it_marker = timeline.insert(pii(pos[elephant] + l, 2 * elephant + 1 + k_offset)).first;

  relink(before_pos);
  relink(before_marker);
  relink(it_pos);
  relink(it_marker);
  relink(prev(it_pos));
  relink(prev(it_marker));

  return LCT::query(LCT::tree[2 * n]);
}

} // namespace

// Splay-based Link-Cut Tree (path aggregates).
// Used by elephants_lct-style problems and standard LCT practice.

#pragma once

#include <algorithm>

struct LctNode {
  int ch[2]{};
  int parent = 0;
  int val = 0;
  int sum = 0;
  bool rev = false;

  bool is_root() const {
    return !parent || (LctNode::pool[parent].ch[0] != id && LctNode::pool[parent].ch[1] != id);
  }

  int id = 0;
  static LctNode pool[];
};

inline LctNode LctNode::pool[1 << 20];

struct LinkCutTree {
  explicit LinkCutTree(int n = 0) {
    resize(n);
  }

  void resize(int n) {
    nodes = n + 1;
    for (int i = 1; i <= n; ++i) {
      LctNode::pool[i].id = i;
      LctNode::pool[i].ch[0] = LctNode::pool[i].ch[1] = LctNode::pool[i].parent = 0;
      LctNode::pool[i].val = LctNode::pool[i].sum = 0;
      LctNode::pool[i].rev = false;
    }
  }

  void set_val(int x, int v) {
    access(x);
    splay(x);
    LctNode::pool[x].val = v;
    pull(x);
  }

  void add_val(int x, int delta) {
    access(x);
    splay(x);
    LctNode::pool[x].val += delta;
    pull(x);
  }

  int path_sum(int x) {
    access(x);
    splay(x);
    return LctNode::pool[x].sum;
  }

  void make_root(int x) {
    access(x);
    splay(x);
    apply_rev(x);
  }

  void link(int child, int parent) {
    make_root(child);
    access(parent);
    splay(parent);
    LctNode::pool[child].parent = parent;
    LctNode::pool[parent].ch[1] = child;
    pull(parent);
  }

  void cut(int x) {
    access(x);
    splay(x);
    LctNode& node = LctNode::pool[x];
    if (node.ch[0]) {
      LctNode::pool[node.ch[0]].parent = 0;
      node.ch[0] = 0;
      pull(x);
    }
  }

  int root(int x) {
    access(x);
    splay(x);
    int u = x;
    while (LctNode::pool[u].ch[0]) {
      push(u);
      u = LctNode::pool[u].ch[0];
    }
    splay(u);
    return u;
  }

private:
  int nodes = 0;

  static int side(int x) {
    return LctNode::pool[LctNode::pool[x].parent].ch[1] == x;
  }

  static void pull(int x) {
    LctNode& node = LctNode::pool[x];
    node.sum = node.val;
    if (node.ch[0]) {
      node.sum += LctNode::pool[node.ch[0]].sum;
    }
    if (node.ch[1]) {
      node.sum += LctNode::pool[node.ch[1]].sum;
    }
  }

  static void push(int x) {
    LctNode& node = LctNode::pool[x];
    if (!node.rev) {
      return;
    }
    if (node.ch[0]) {
      apply_rev(node.ch[0]);
    }
    if (node.ch[1]) {
      apply_rev(node.ch[1]);
    }
    node.rev = false;
  }

  static void apply_rev(int x) {
    LctNode& node = LctNode::pool[x];
    std::swap(node.ch[0], node.ch[1]);
    node.rev ^= true;
  }

  static void rotate(int x) {
    const int p = LctNode::pool[x].parent;
    const int g = LctNode::pool[p].parent;
    const int d = side(x);
    const int y = LctNode::pool[x].ch[d ^ 1];

    if (!LctNode::pool[p].is_root()) {
      LctNode::pool[g].ch[side(p)] = x;
    }
    LctNode::pool[x].parent = g;
    LctNode::pool[x].ch[d ^ 1] = p;
    LctNode::pool[p].parent = x;
    LctNode::pool[p].ch[d] = y;
    if (y) {
      LctNode::pool[y].parent = p;
    }
    pull(p);
    pull(x);
  }

  static void splay(int x) {
    static int stk[1 << 20];
    int top = 0;
    stk[++top] = x;
    for (int i = x; !LctNode::pool[i].is_root(); i = LctNode::pool[i].parent) {
      stk[++top] = LctNode::pool[i].parent;
    }
    while (top) {
      push(stk[top--]);
    }
    while (!LctNode::pool[x].is_root()) {
      const int p = LctNode::pool[x].parent;
      const int g = LctNode::pool[p].parent;
      if (LctNode::pool[p].is_root()) {
        rotate(x);
      } else if (side(x) == side(p)) {
        rotate(p);
        rotate(x);
      } else {
        rotate(x);
        rotate(x);
      }
    }
  }

  static void access(int x) {
    int last = 0;
    for (int y = x; y; y = LctNode::pool[y].parent) {
      splay(y);
      LctNode::pool[y].ch[1] = last;
      pull(y);
      last = y;
    }
    splay(x);
  }
};

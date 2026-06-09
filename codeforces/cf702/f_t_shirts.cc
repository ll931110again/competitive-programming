// CF 702F — T-Shirts
// https://codeforces.com/contest/702/problem/F
//
// Process shirts by descending quality. Customers live in a treap ordered by
// remaining balance (explicit key). For price c, split [0,c-1], [c,2c-1], [2c,inf);
// lazy-update the upper part, reinsert the middle part one-by-one (each at least
// halves, so O(k log^2 k) total).

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Shirt {
  int cost;
  int quality;
};

struct Node {
  int ch[2]{};
  int prior = 0;
  int key = 0;
  int id = 0;
  int shirts = 0;
  int lazy_money = 0;
  int lazy_shirts = 0;
};

constexpr int k_max_nodes = 400005;

Node pool[k_max_nodes];
int pool_used = 0;
int root = 0;

int new_node(int balance, int id) {
  Node& node = pool[++pool_used];
  node.key = balance;
  node.id = id;
  node.prior = rand();
  node.ch[0] = node.ch[1] = 0;
  node.shirts = node.lazy_money = node.lazy_shirts = 0;
  return pool_used;
}

void apply(int t, int money_delta, int shirt_delta) {
  if (!t) {
    return;
  }
  Node& node = pool[t];
  node.key += money_delta;
  node.lazy_money += money_delta;
  node.shirts += shirt_delta;
  node.lazy_shirts += shirt_delta;
}

void push(int t) {
  if (!t) {
    return;
  }
  Node& node = pool[t];
  if (node.lazy_money || node.lazy_shirts) {
    apply(node.ch[0], node.lazy_money, node.lazy_shirts);
    apply(node.ch[1], node.lazy_money, node.lazy_shirts);
    node.lazy_money = node.lazy_shirts = 0;
  }
}

void merge_nodes(int& t, int a, int b) {
  if (!a) {
    t = b;
    return;
  }
  if (!b) {
    t = a;
    return;
  }
  push(a);
  push(b);
  if (pool[a].prior > pool[b].prior) {
    merge_nodes(pool[a].ch[1], pool[a].ch[1], b);
    t = a;
  } else {
    merge_nodes(pool[b].ch[0], a, pool[b].ch[0]);
    t = b;
  }
}

void split_key(int t, int pivot, int& left, int& right) {
  if (!t) {
    left = right = 0;
    return;
  }
  push(t);
  if (pool[t].key <= pivot) {
    split_key(pool[t].ch[1], pivot, pool[t].ch[1], right);
    left = t;
  } else {
    split_key(pool[t].ch[0], pivot, left, pool[t].ch[0]);
    right = t;
  }
}

void insert_by_key(int& t, int node) {
  int left = 0;
  int right = 0;
  split_key(t, pool[node].key, left, right);
  merge_nodes(t, left, node);
  merge_nodes(t, t, right);
}

void collect(int t, vector<int>& out) {
  if (!t) {
    return;
  }
  push(t);
  collect(pool[t].ch[0], out);
  out.push_back(t);
  collect(pool[t].ch[1], out);
}

void buy_shirt(int cost) {
  int low = 0;
  int rest = 0;
  split_key(root, cost - 1, low, rest);
  if (!rest) {
    root = low;
    return;
  }

  apply(rest, -cost, 1);
  int mid = 0;
  int high = 0;
  split_key(rest, cost, mid, high);

  vector<int> reinsert;
  collect(mid, reinsert);
  for (int node : reinsert) {
    pool[node].ch[0] = pool[node].ch[1] = 0;
    insert_by_key(low, node);
  }

  merge_nodes(root, low, high);
}

void output(int t, vector<int>& answers) {
  if (!t) {
    return;
  }
  push(t);
  output(pool[t].ch[0], answers);
  answers[pool[t].id] = pool[t].shirts;
  output(pool[t].ch[1], answers);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<Shirt> shirts(n);
  for (int i = 0; i < n; ++i) {
    cin >> shirts[i].cost >> shirts[i].quality;
  }
  sort(shirts.begin(), shirts.end(), [](const Shirt& a, const Shirt& b) {
    if (a.quality != b.quality) {
      return a.quality > b.quality;
    }
    return a.cost < b.cost;
  });

  int k;
  cin >> k;
  vector<int> order(k);
  for (int i = 0; i < k; ++i) {
    int balance = 0;
    cin >> balance;
    order[i] = new_node(balance, i);
  }
  sort(order.begin(), order.end(), [](int a, int b) { return pool[a].key < pool[b].key; });
  for (int node : order) {
    merge_nodes(root, root, node);
  }

  for (const Shirt& shirt : shirts) {
    buy_shirt(shirt.cost);
  }

  vector<int> answers(k, 0);
  output(root, answers);
  for (int i = 0; i < k; ++i) {
    printf("%d%c", answers[i], " \n"[i + 1 == k]);
  }
  return 0;
}

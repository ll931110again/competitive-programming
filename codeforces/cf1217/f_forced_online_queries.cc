// CF 1217F — Forced Online Queries Problem
// https://codeforces.com/contest/1217/problem/F
//
// last ∈ {0,1} ⇒ decode offline. Edge toggles become time intervals; CDQ on time +
// rollback DSU (codgician).

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Change {
  int* ptr;
  int old;
};

vector<Change> stk;

int parent[400010];
int siz[400010];

int get_parent(int x) {
  if (parent[x] == x) {
    return x;
  }
  return get_parent(parent[x]);
}

bool merge_nodes(int a, int b) {
  a = get_parent(a);
  b = get_parent(b);
  if (a == b) {
    return false;
  }
  if (siz[a] < siz[b]) {
    swap(a, b);
  }
  stk.push_back({parent + b, parent[b]});
  parent[b] = a;
  stk.push_back({siz + a, siz[a]});
  siz[a] += siz[b];
  return true;
}

void undo_all(int target) {
  while (static_cast<int>(stk.size()) > target) {
    *stk.back().ptr = stk.back().old;
    stk.pop_back();
  }
}

struct EdgeOp {
  int type, x, y;
};

struct Interval {
  int from, to, left, right;
};

EdgeOp ops[400010];
vector<Interval> intervals;
unordered_map<long long, int> edge_idx;
int n, last_ans;

long long pack_edge(int x, int y) {
  if (x > y) {
    swap(x, y);
  }
  return 1LL * x * 400010LL + y;
}

void divide_conquer(int left, int right, const vector<Interval>& vec) {
  const int mid = (left + right) >> 1;
  const int snap = static_cast<int>(stk.size());
  vector<Interval> left_vec, right_vec;

  for (const auto& q : vec) {
    if (q.left == left && q.right == right) {
      if (edge_idx[pack_edge(q.from, q.to)]) {
        merge_nodes(q.from, q.to);
      }
    } else {
      if (q.left <= mid) {
        left_vec.push_back({q.from, q.to, q.left, min(q.right, mid)});
      }
      if (q.right > mid) {
        right_vec.push_back({q.from, q.to, max(q.left, mid + 1), q.right});
      }
    }
  }

  if (left == right) {
    auto& e = ops[left];
    e.x = (e.x + last_ans - 1) % n + 1;
    e.y = (e.y + last_ans - 1) % n + 1;
    if (e.type == 2) {
      last_ans = (get_parent(e.x) == get_parent(e.y)) ? 1 : 0;
      cout << last_ans;
    } else {
      edge_idx[pack_edge(e.x, e.y)] ^= 1;
    }
    undo_all(snap);
    return;
  }

  divide_conquer(left, mid, left_vec);
  divide_conquer(mid + 1, right, right_vec);
  undo_all(snap);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int m;
  cin >> n >> m;
  for (int i = 1; i <= n; ++i) {
    parent[i] = i;
    siz[i] = 1;
  }

  for (int i = 0; i < m; ++i) {
    cin >> ops[i].type >> ops[i].x >> ops[i].y;
  }

  auto add_interval = [&](int from, int to, int l, int r) {
    if (l > r) {
      return;
    }
    const long long key = pack_edge(from, to);
    if (!edge_idx.count(key)) {
      edge_idx[key] = static_cast<int>(intervals.size());
      intervals.push_back({from, to, l, r});
    } else {
      intervals[edge_idx[key]].right = min(intervals[edge_idx[key]].left, r - 1);
      if (l <= r) {
        edge_idx[key] = static_cast<int>(intervals.size());
        intervals.push_back({from, to, l, r});
      }
    }
  };

  for (int i = 0; i < m; ++i) {
    if (ops[i].type != 1) {
      continue;
    }
  }

  edge_idx.clear();
  intervals.clear();

  auto register_toggle = [&](int i, int x, int y) {
    const long long key = pack_edge(x, y);
    if (!edge_idx.count(key)) {
      edge_idx[key] = static_cast<int>(intervals.size());
      intervals.push_back({x, y, i + 1, m - 1});
    } else {
      intervals[edge_idx[key]].right = min(intervals[edge_idx[key]].right, i - 1);
      if (i + 1 <= m - 1) {
        edge_idx[key] = static_cast<int>(intervals.size());
        intervals.push_back({x, y, i + 1, m - 1});
      }
    }
  };

  for (int i = 0; i < m; ++i) {
    if (ops[i].type == 1) {
      int x = ops[i].x;
      int y = ops[i].y;
      register_toggle(i, x, y);
      register_toggle(i, x % n + 1, y % n + 1);
      int x2 = x % n + 1;
      int y2 = y % n + 1;
      if (x2 == 0) {
        x2 = n;
      }
      if (y2 == 0) {
        y2 = n;
      }
      register_toggle(i, x2, y2);
    }
  }

  edge_idx.clear();
  last_ans = 0;
  divide_conquer(0, m - 1, intervals);
  cout << '\n';
  return 0;
}

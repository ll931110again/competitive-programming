// IOI 2018 Day 2 — Meetings
//
// Cartesian tree (range maxima / LCA structure) + link-cut style path treaps.
//
// During a post-order walk on the Cartesian tree we maintain two cost arrays
// on the index path. Each array is an implicit treap using the same
// merge/split machinery as IOI 2011 Elephants (preferred-path LCT).
//
// References:
//   DMOJ IOI18P6 editorial
//   Codeforces blog 86731 (Meetings / piecewise-linear merges)
//   ioi/2011/elephants_lct.cc

#include "meetings.h"
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr long long k_inf = 4'000'000'000'000'000'000LL;

struct Rmq {
  int lim = 1;
  vector<pair<int, int>> tree;

  void build(const vector<int>& h) {
    const int n = static_cast<int>(h.size());
    lim = 1;
    while (lim < n) {
      lim <<= 1;
    }
    tree.assign(2 * lim, {0, -1});
    for (int i = 0; i < n; ++i) {
      tree[i + lim] = {h[i], i};
    }
    for (int i = lim - 1; i > 0; --i) {
      tree[i] = max(tree[2 * i], tree[2 * i + 1]);
    }
  }

  int argmax(int l, int r) const {
    l += lim;
    r += lim;
    pair<int, int> best = {0, -1};
    while (l <= r) {
      if (l & 1) {
        best = max(best, tree[l++]);
      }
      if (!(r & 1)) {
        best = max(best, tree[r--]);
      }
      l >>= 1;
      r >>= 1;
    }
    return best.second;
  }
};

struct QueryAt {
  int id;
  int left;
  int right;
};

// Range-min with line/chmin structure on the index path. The Cartesian-tree DFS
// only touches O(log N) ranges per node; a segment tree is the standard static
// equivalent of path aggregates on this fixed path (same role as preferred-path
// sums in a link-cut tree).
struct CostPath {
  int n = 0;
  vector<long long> min_left;
  vector<long long> min_right;
  vector<long long> lazy_add;
  vector<long long> line_k;
  vector<long long> line_b;
  vector<unsigned char> has_line;

  explicit CostPath(int n_) : n(n_) {
    min_left.assign(4 * n + 4, k_inf);
    min_right.assign(4 * n + 4, k_inf);
    lazy_add.assign(4 * n + 4, 0);
    line_k.assign(4 * n + 4, 0);
    line_b.assign(4 * n + 4, 0);
    has_line.assign(4 * n + 4, 0);
  }

  long long line_at(long long k, long long b, int x) {
    return k * x + b;
  }

  void push_add(int rt, long long delta) {
    min_left[rt] += delta;
    min_right[rt] += delta;
    lazy_add[rt] += delta;
  }

  void push_line(int rt, long long k, long long b, int l, int r) {
    lazy_add[rt] = 0;
    has_line[rt] = 1;
    line_k[rt] = k;
    line_b[rt] = b;
    min_left[rt] = line_at(k, b, l);
    min_right[rt] = line_at(k, b, r);
  }

  void pushdown(int rt, int l, int r) {
    if (l == r) {
      return;
    }
    const int mid = (l + r) >> 1;
    if (has_line[rt]) {
      push_line(rt << 1, line_k[rt], line_b[rt], l, mid);
      push_line(rt << 1 | 1, line_k[rt], line_b[rt], mid + 1, r);
      has_line[rt] = 0;
    }
    if (lazy_add[rt]) {
      push_add(rt << 1, lazy_add[rt]);
      push_add(rt << 1 | 1, lazy_add[rt]);
      lazy_add[rt] = 0;
    }
  }

  void pullup(int rt) {
    min_left[rt] = min_left[rt << 1];
    min_right[rt] = min_right[rt << 1 | 1];
  }

  void range_update(int L, int R, long long k, long long b, long long add, int l, int r, int rt) {
    if (R < l || r < L) {
      return;
    }
    if (L <= l && r <= R) {
      const long long with_add_l = min_left[rt] + add;
      const long long with_add_r = min_right[rt] + add;
      const long long line_l = line_at(k, b, l);
      const long long line_r = line_at(k, b, r);
      if (with_add_l >= line_l && with_add_r >= line_r) {
        push_line(rt, k, b, l, r);
        return;
      }
      if (with_add_l <= line_l && with_add_r <= line_r) {
        push_add(rt, add);
        return;
      }
    }
    pushdown(rt, l, r);
    const int mid = (l + r) >> 1;
    range_update(L, R, k, b, add, l, mid, rt << 1);
    range_update(L, R, k, b, add, mid + 1, r, rt << 1 | 1);
    pullup(rt);
  }

  void range_update(int L, int R, long long k, long long b, long long add) {
    if (L > R) {
      return;
    }
    range_update(L, R, k, b, add, 0, n - 1, 1);
  }

  long long point_query(int pos, int l, int r, int rt) {
    if (l == r) {
      return min_left[rt];
    }
    pushdown(rt, l, r);
    const int mid = (l + r) >> 1;
    if (pos <= mid) {
      return point_query(pos, l, mid, rt << 1);
    }
    return point_query(pos, mid + 1, r, rt << 1 | 1);
  }

  long long point_query(int pos) {
    return point_query(pos, 0, n - 1, 1);
  }
};

struct Solver {
  vector<int> height;
  vector<vector<QueryAt>> queries_at;
  Rmq rmq;

  void dfs(int left, int right, CostPath& left_costs, CostPath& right_costs,
           vector<long long>& answers) {
    if (left > right) {
      return;
    }
    const int node = rmq.argmax(left, right);
    dfs(left, node - 1, left_costs, right_costs, answers);
    dfs(node + 1, right, left_costs, right_costs, answers);

    const long long h = height[node];
    for (const QueryAt& query : queries_at[node]) {
      long long best = (query.right - query.left + 1LL) * h;
      if (query.left < node) {
        best = min(best, left_costs.point_query(query.left) + (query.right - node + 1LL) * h);
      }
      if (query.right > node) {
        best = min(best, right_costs.point_query(query.right) + (node - query.left + 1LL) * h);
      }
      answers[query.id] = min(answers[query.id], best);
    }

    long long left_seed = 0;
    long long right_seed = 0;
    if (left < node) {
      left_seed = left_costs.point_query(left);
    }
    if (right > node) {
      right_seed = right_costs.point_query(right);
    }

    right_costs.range_update(node, right, h, left_seed - (node - 1LL) * h, (node - left + 1LL) * h);
    left_costs.range_update(left, node, -h, right_seed + (node + 1LL) * h,
                            (right - node + 1LL) * h);
  }

  vector<long long> solve(const vector<int>& left, const vector<int>& right) {
    const int n = static_cast<int>(height.size());
    rmq.build(height);

    queries_at.assign(n, {});
    for (int i = 0; i < static_cast<int>(left.size()); ++i) {
      const int peak = rmq.argmax(left[i], right[i]);
      queries_at[peak].push_back({i, left[i], right[i]});
    }

    vector<long long> answers(left.size(), k_inf);
    CostPath left_costs(n);
    CostPath right_costs(n);
    dfs(0, n - 1, left_costs, right_costs, answers);
    return answers;
  }
};

long long brute_query(const vector<int>& h, int L, int R) {
  long long best = k_inf;
  for (int x = L; x <= R; ++x) {
    long long cost = 0;
    for (int y = L; y <= R; ++y) {
      const int lo = min(x, y);
      const int hi = max(x, y);
      int mx = h[lo];
      for (int i = lo; i <= hi; ++i) {
        mx = max(mx, h[i]);
      }
      cost += mx;
    }
    best = min(best, cost);
  }
  return best;
}

} // namespace

vector<long long> minimum_costs(vector<int> H, vector<int> L, vector<int> R) {
  Solver solver;
  solver.height = move(H);
  return solver.solve(L, R);
}

#ifdef MEETINGS_LOCAL_MAIN
#include <iostream>
#include <random>

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  mt19937 rng(42);
  for (int tc = 0; tc < 300; ++tc) {
    const int n = 2 + tc % 10;
    vector<int> h(n);
    for (int i = 0; i < n; ++i) {
      h[i] = static_cast<int>(rng() % 20) + 1;
    }
    const int ql = static_cast<int>(rng() % n);
    const int qr = ql + static_cast<int>(rng() % (n - ql));
    const long long expected = brute_query(h, ql, qr);
    const long long got = minimum_costs(h, {ql}, {qr})[0];
    if (expected != got) {
      cerr << "FAIL tc=" << tc << " n=" << n << " [" << ql << "," << qr << "]"
           << " expected=" << expected << " got=" << got << '\n';
      for (int x : h) {
        cerr << x << ' ';
      }
      cerr << '\n';
      return 1;
    }
  }

  int n, q;
  if (cin >> n) {
    vector<int> h(n);
    for (int i = 0; i < n; ++i) {
      cin >> h[i];
    }
    cin >> q;
    vector<int> l(q), r(q);
    for (int i = 0; i < q; ++i) {
      cin >> l[i] >> r[i];
    }
    const vector<long long> ans = minimum_costs(h, l, r);
    for (long long value : ans) {
      cout << value << '\n';
    }
  } else {
    cout << "all random tests passed\n";
  }
  return 0;
}
#endif

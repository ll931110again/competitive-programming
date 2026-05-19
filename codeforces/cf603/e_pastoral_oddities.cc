// CF 603E — Pastoral Oddities
// https://codeforces.com/contest/603/problem/E
//
// Sunny paving exists iff every connected component has even size.
// D&C on query index + rollback DSU; search sorted edge weights.

#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Edge {
  int u, v, w, id;
};

struct DsuOdd {
  vector<int> fa, sz;
  struct Change {
    int x, y, delta;
  };
  vector<Change> st;
  int odd = 0;

  void init(int n) {
    fa.resize(n + 1);
    sz.assign(n + 1, 1);
    for (int i = 1; i <= n; ++i) {
      fa[i] = i;
    }
    st.clear();
    odd = n;
  }

  int find(int x) const {
    while (fa[x] != x) {
      x = fa[x];
    }
    return x;
  }

  void unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) {
      return;
    }
    if (sz[x] > sz[y]) {
      swap(x, y);
    }
    int delta = 0;
    if ((sz[x] & 1) && (sz[y] & 1)) {
      delta = 2;
    }
    odd -= delta;
    st.push_back({x, y, delta});
    fa[x] = y;
    sz[y] += sz[x];
  }

  int snapshot() const {
    return static_cast<int>(st.size());
  }

  void rollback(int t) {
    while (static_cast<int>(st.size()) > t) {
      Change c = st.back();
      st.pop_back();
      odd += c.delta;
      sz[c.y] -= sz[c.x];
      fa[c.x] = c.x;
    }
  }
};

int n, m;
vector<Edge> by_time;
vector<Edge> by_weight;
vector<int> ans;
DsuOdd dsu;

void divide(int l, int r, int lo, int hi) {
  if (l > r) {
    return;
  }
  const int saved = dsu.snapshot();
  const int mid = (l + r) >> 1;
  int ans_mid = -1;

  for (int i = l; i <= mid; ++i) {
    if (by_time[i].id < lo) {
      dsu.unite(by_time[i].u, by_time[i].v);
    }
  }
  for (int i = lo; i <= hi; ++i) {
    if (by_weight[i].id <= mid) {
      dsu.unite(by_weight[i].u, by_weight[i].v);
    }
    if (dsu.odd == 0) {
      ans_mid = i;
      break;
    }
  }
  dsu.rollback(saved);

  if (ans_mid == -1) {
    for (int i = l; i <= mid; ++i) {
      ans[i] = -1;
    }
    for (int i = l; i <= mid; ++i) {
      if (by_time[i].id < lo) {
        dsu.unite(by_time[i].u, by_time[i].v);
      }
    }
    divide(mid + 1, r, lo, hi);
    dsu.rollback(saved);
    return;
  }

  ans[mid] = by_weight[ans_mid].w;
  for (int i = lo; i < ans_mid; ++i) {
    if (by_weight[i].id < l) {
      dsu.unite(by_weight[i].u, by_weight[i].v);
    }
  }
  divide(l, mid - 1, ans_mid, hi);
  dsu.rollback(saved);

  for (int i = l; i <= mid; ++i) {
    if (by_time[i].id < lo) {
      dsu.unite(by_time[i].u, by_time[i].v);
    }
  }
  divide(mid + 1, r, lo, ans_mid);
  dsu.rollback(saved);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  if (n & 1) {
    for (int i = 0; i < m; ++i) {
      cout << -1 << '\n';
    }
    return 0;
  }

  by_time.resize(m + 1);
  by_weight.resize(m + 1);
  for (int i = 1; i <= m; ++i) {
    cin >> by_time[i].u >> by_time[i].v >> by_time[i].w;
    by_time[i].id = i;
    by_weight[i] = by_time[i];
  }
  sort(by_weight.begin() + 1, by_weight.end(),
       [](const Edge& a, const Edge& b) { return a.w < b.w; });
  for (int i = 1; i <= m; ++i) {
    by_time[by_weight[i].id].id = i;
  }

  ans.assign(m + 1, -1);
  dsu.init(n);
  divide(1, m, 1, m);

  for (int i = 1; i <= m; ++i) {
    cout << ans[i] << '\n';
  }
  return 0;
}

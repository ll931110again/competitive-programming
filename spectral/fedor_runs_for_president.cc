#include <algorithm>
#include <cstdint>
#include <deque>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

using namespace std;

using int64 = long long;

static inline int64 comb2(int64 x) { return x * (x - 1) / 2; }

static inline int64 clampI128ToI64(__int128 v) {
  if (v < (__int128)numeric_limits<int64>::min())
    return numeric_limits<int64>::min();
  if (v > (__int128)numeric_limits<int64>::max())
    return numeric_limits<int64>::max();
  return (int64)v;
}

// Convex hull for maximum: add lines with increasing slope, query maximum at x
// (binary search).
struct ConvexHullMax {
  // line: y = m*x + b
  deque<pair<int64, int64>> hull;

  static inline __int128 eval(const pair<int64, int64> &ln, int64 x) {
    return (__int128)ln.first * (__int128)x + (__int128)ln.second;
  }

  // Whether middle line B is unnecessary for max hull.
  static bool isBad(const pair<int64, int64> &A, const pair<int64, int64> &B,
                    const pair<int64, int64> &C) {
    // (C.b-A.b)/(A.m-C.m) <= (B.b-A.b)/(A.m-B.m)
    __int128 left =
        (__int128)(A.second - C.second) * (__int128)(B.first - A.first);
    __int128 right =
        (__int128)(A.second - B.second) * (__int128)(C.first - A.first);
    return left <= right;
  }

  void addLine(int64 m, int64 b) {
    pair<int64, int64> ln{m, b};
    if (!hull.empty() && hull.back().first == m) {
      if (hull.back().second >= b)
        return;
      hull.pop_back();
    }
    hull.push_back(ln);
    while (hull.size() >= 3) {
      int n = (int)hull.size();
      if (isBad(hull[n - 3], hull[n - 2], hull[n - 1])) {
        hull[n - 2] = hull[n - 1];
        hull.pop_back();
      } else {
        break;
      }
    }
  }

  int64 query(int64 x) const {
    int L = -1, R = (int)hull.size() - 1;
    while (R - L > 1) {
      int M = (L + R) / 2;
      if (eval(hull[M], x) >= eval(hull[M + 1], x))
        L = M;
      else
        R = M;
    }
    return clampI128ToI64(eval(hull[R], x));
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;

  vector<vector<int>> adj(n);
  for (int i = 0; i < n - 1; i++) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  // Root at 0.
  vector<int> parent(n, -1);
  vector<int> order;
  order.reserve(n);
  {
    vector<int> st;
    st.push_back(0);
    parent[0] = 0;
    while (!st.empty()) {
      int u = st.back();
      st.pop_back();
      order.push_back(u);
      for (int v : adj[u]) {
        if (v == parent[u])
          continue;
        parent[v] = u;
        st.push_back(v);
      }
    }
  }

  vector<vector<int>> children(n);
  children.assign(n, {});
  for (int v = 1; v < n; v++)
    children[parent[v]].push_back(v);

  vector<int64> subSize(n, 1);
  for (int i = n - 1; i >= 0; i--) {
    int u = order[i];
    int64 s = 1;
    for (int v : children[u])
      s += subSize[v];
    subSize[u] = s;
  }

  vector<int64> dp(n, 0);
  int64 mi = (int64)4e18;

  for (int i = n - 1; i >= 0; i--) {
    int u = order[i];
    int64 Cu = subSize[u];

    dp[u] = comb2(Cu);

    vector<pair<int64, int64>> cand; // (subSize[child], dp[child])
    cand.reserve(children[u].size());

    for (int v : children[u]) {
      int64 Cv = subSize[v];
      dp[u] = min(dp[u], dp[v] + comb2(Cu - Cv));
      cand.push_back({Cv, dp[v]});

      // select one side: choose an edge with one endpoint in subtree v, other
      // outside subtree v
      mi = min(mi, dp[v] + comb2((int64)n - Cv));
    }

    // select two sides: choose endpoints in two different child subtrees of u
    sort(cand.begin(), cand.end()); // increasing size => increasing slopes
    ConvexHullMax ch;
    for (int idx = 0; idx < (int)cand.size(); idx++) {
      int64 sz = cand[idx].first;
      int64 dv = cand[idx].second;
      if (idx > 0) {
        int64 best = ch.query(sz);
        __int128 v = (__int128)best +
                     ((__int128)n * (__int128)(n - 1) + 2 * (__int128)dv -
                      2 * (__int128)n * (__int128)sz +
                      (__int128)sz * (__int128)(sz + 1));
        mi = min(mi, clampI128ToI64(v / 2));
      }
      __int128 b = 2 * (__int128)dv - 2 * (__int128)n * (__int128)sz +
                   (__int128)sz * (__int128)(sz + 1);
      ch.addLine(2 * sz, clampI128ToI64(b));
    }
  }

  __int128 ans = (__int128)n * (__int128)(n - 1) - (__int128)mi;
  cout << (int64)ans << "\n";
  return 0;
}

// CF 2222F — Building Tree
// https://codeforces.com/contest/2222/problem/F
//
// For marked vertices (colors), dist(u,v) = MEX of edge weights on a u–v path in the
// original graph = smallest w such that u,v lie in different components after removing
// all edges with weight w. Build MST on the complete graph of colors with those weights.
// D&C on weight buckets + rollback DSU (tourist-style).

#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <functional>
#include <iostream>
#include <utility>
#include <vector>

#include "../../../lib/dsu.hh"
#include "../../../lib/dsu_rollback.hh"

using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int tt;
  cin >> tt;
  while (tt--) {
    int n, m, q;
    cin >> n >> m >> q;

    vector<vector<pair<int, int>>> by_weight(m + 1);
    for (int i = 0; i < m; ++i) {
      int x, y, w;
      cin >> x >> y >> w;
      --x;
      --y;
      by_weight[w].emplace_back(x, y);
    }

    vector<bool> marked(n, false);
    for (int i = 0; i < q; ++i) {
      int c;
      cin >> c;
      --c;
      marked[c] = true;
    }

    vector<int> color_id(n, -1);
    int colors = 0;
    for (int i = 0; i < n; ++i) {
      if (marked[i]) {
        color_id[i] = colors++;
      }
    }

    if (colors <= 1) {
      cout << 0 << '\n';
      continue;
    }

    DsuRollbackMarked graph_dsu(n, color_id);
    Dsu color_dsu(colors);
    long long ans = 0;

    function<void(int, int)> solve = [&](int left, int right) {
      if (right - left == 1) {
        return;
      }
      const int mid = (left + right) >> 1;
      for (int rot = 0; rot < 2; ++rot) {
        const int w_lo = (rot == 0 ? mid : left);
        const int w_hi = (rot == 0 ? right : mid);
        const int snap = graph_dsu.snapshot();
        for (int w = w_lo; w < w_hi; ++w) {
          for (auto [x, y] : by_weight[w]) {
            x = graph_dsu.find(x);
            y = graph_dsu.find(y);
            if (x == y) {
              continue;
            }
            if (graph_dsu.idx[x] != -1 && graph_dsu.idx[y] != -1) {
              if (color_dsu.unite(graph_dsu.idx[x], graph_dsu.idx[y])) {
                ans += (rot == 0 ? left : mid);
              }
            }
            graph_dsu.unite(x, y);
          }
        }
        if (rot == 0) {
          solve(left, mid);
        } else {
          solve(mid, right);
        }
        graph_dsu.rollback(snap);
      }
    };

    solve(0, m + 1);

    for (int i = 0; i < colors; ++i) {
      if (color_dsu.find(i) != color_dsu.find(0)) {
        ans = -1;
        break;
      }
    }
    cout << ans << '\n';
  }
  return 0;
}

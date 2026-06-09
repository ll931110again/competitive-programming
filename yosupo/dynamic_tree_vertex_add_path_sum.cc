// YOSUPO — Dynamic Tree Vertex Add Path Sum
// https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum
//
// Canonical LCT path-aggregate problem (same data structure family as
// IOI 2011 Elephants LCT solution).

#include "../codeforces/lib/lct_splay.h"
#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, q;
  cin >> n >> q;
  LinkCutTree lct(n);
  for (int i = 1; i <= n; ++i) {
    int x;
    cin >> x;
    lct.set_val(i, x);
  }

  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    cin >> u >> v;
    lct.link(u, v);
  }

  while (q--) {
    int t;
    cin >> t;
    if (t == 0) {
      int u, v;
      cin >> u >> v;
      lct.cut(u);
      lct.link(u, v);
    } else if (t == 1) {
      int p, x;
      cin >> p >> x;
      lct.add_val(p, x);
    } else {
      int u;
      cin >> u;
      cout << lct.path_sum(u) << '\n';
    }
  }

  return 0;
}

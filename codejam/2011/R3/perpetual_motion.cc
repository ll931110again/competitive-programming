#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

const int MOD = 1000003;

struct DSU {
  vector<int> p, sz;
  explicit DSU(int n) : p(n), sz(n, 1) {
    for (int i = 0; i < n; i++)
      p[i] = i;
  }
  int find(int x) {
    return p[x] == x ? x : (p[x] = find(p[x]));
  }
  void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b)
      return;
    if (sz[a] < sz[b])
      swap(a, b);
    p[b] = a;
    sz[a] += sz[b];
  }
};

int mod_pow2(int e) {
  i64 r = 1, a = 2;
  while (e > 0) {
    if (e & 1)
      r = (r * a) % MOD;
    a = (a * a) % MOD;
    e >>= 1;
  }
  return (int)r;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int R, C;
    cin >> R >> C;
    vector<string> g(R);
    for (int i = 0; i < R; i++)
      cin >> g[i];

    int n = R * C;
    DSU dsu(n);
    vector<pair<int, int>> edges;
    edges.reserve(n);

    auto id = [&](int r, int c) { return r * C + c; };
    auto wrap = [&](int r, int c) {
      if (r < 0)
        r += R;
      if (r >= R)
        r -= R;
      if (c < 0)
        c += C;
      if (c >= C)
        c -= C;
      return pair<int, int>(r, c);
    };

    for (int r = 0; r < R; r++) {
      for (int c = 0; c < C; c++) {
        char ch = g[r][c];
        int r1 = r, c1 = c, r2 = r, c2 = c;
        if (ch == '|') {
          auto a = wrap(r - 1, c);
          auto b = wrap(r + 1, c);
          r1 = a.first;
          c1 = a.second;
          r2 = b.first;
          c2 = b.second;
        } else if (ch == '-') {
          auto a = wrap(r, c - 1);
          auto b = wrap(r, c + 1);
          r1 = a.first;
          c1 = a.second;
          r2 = b.first;
          c2 = b.second;
        } else if (ch == '/') {
          auto a = wrap(r - 1, c + 1);
          auto b = wrap(r + 1, c - 1);
          r1 = a.first;
          c1 = a.second;
          r2 = b.first;
          c2 = b.second;
        } else { // '\\'
          auto a = wrap(r - 1, c - 1);
          auto b = wrap(r + 1, c + 1);
          r1 = a.first;
          c1 = a.second;
          r2 = b.first;
          c2 = b.second;
        }

        int u = id(r1, c1);
        int v = id(r2, c2);
        edges.push_back({u, v});
        dsu.unite(u, v);
      }
    }

    // Count vertices and edges per component.
    unordered_map<int, int> comp_edges;
    comp_edges.reserve(n * 2);
    for (auto [u, v] : edges) {
      int root = dsu.find(u);
      comp_edges[root] += 1;
    }

    int comps = 0;
    bool ok = true;
    for (int i = 0; i < n; i++) {
      if (dsu.find(i) != i)
        continue;
      comps++;
      int Vc = dsu.sz[i];
      int Ec = comp_edges[i];
      if (Ec != Vc) {
        ok = false;
        break;
      }
    }

    int ans = ok ? mod_pow2(comps) : 0;
    cout << "Case #" << tc << ": " << ans << "\n";
  }
  return 0;
}

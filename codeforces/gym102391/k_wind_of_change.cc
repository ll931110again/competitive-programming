// CF Gym 102391 K — Wind of Change (Tree Product Metric Voronoi)
// https://codeforces.com/gym/102391/problem/K
//
// Dual centroid decomposition. O(n log^2 n) time, O(n log n) memory.

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
constexpr ll k_inf = numeric_limits<ll>::max() / 4;

namespace {

struct Top2 {
  ll b1 = k_inf;
  ll b2 = k_inf;
  int i1 = -1;
  int i2 = -1;

  void clear() {
    b1 = b2 = k_inf;
    i1 = i2 = -1;
  }

  void add(ll val, int id) {
    if (val < b1) {
      if (i1 != id) {
        b2 = b1;
        i2 = i1;
      }
      b1 = val;
      i1 = id;
    } else if (id != i1 && val < b2) {
      b2 = val;
      i2 = id;
    }
  }

  ll best_excluding(int id) const {
    if (i1 != id) {
      return b1;
    }
    return b2;
  }
};

struct ChainEntry {
  int centroid;
  ll dist;
};

struct CentroidDecomp {
  int n;
  vector<vector<pair<int, ll>>> adj;
  vector<int> sz;
  vector<char> dead;
  vector<vector<ChainEntry>> chain;
  vector<int> verts;
  vector<ll> dists;
  vector<int> pending_v;
  vector<int> pending_l2;
  vector<ll> pending_self;

  explicit CentroidDecomp(int n_) : n(n_), adj(n_), sz(n_), dead(n_), chain(n_) {
    verts.reserve(n);
    dists.reserve(n);
    for (int i = 0; i < n; ++i) {
      chain[i].reserve(20);
    }
  }

  void add_edge(int u, int v, ll w) {
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
  }

  int calc_sz(int u, int p) {
    sz[u] = 1;
    for (const auto& [v, w] : adj[u]) {
      if (v == p || dead[v]) {
        continue;
      }
      sz[u] += calc_sz(v, u);
    }
    return sz[u];
  }

  int find_centroid(int u, int p, int tot) {
    for (const auto& [v, w] : adj[u]) {
      if (v == p || dead[v]) {
        continue;
      }
      if (sz[v] > tot / 2) {
        return find_centroid(v, u, tot);
      }
    }
    return u;
  }

  void collect(int u, int p, ll d) {
    verts.push_back(u);
    dists.push_back(d);
    for (const auto& [v, w] : adj[u]) {
      if (v == p || dead[v]) {
        continue;
      }
      collect(v, u, d + w);
    }
  }

  void decompose(int entry) {
    const int tot = calc_sz(entry, -1);
    const int c = find_centroid(entry, -1, tot);

    verts.clear();
    dists.clear();
    collect(c, -1, 0);
    for (int i = 0; i < (int)verts.size(); ++i) {
      chain[verts[i]].push_back({c, dists[i]});
    }

    dead[c] = true;
    for (const auto& [v, w] : adj[c]) {
      if (!dead[v]) {
        decompose(v);
      }
    }
  }

  void decompose_with_callback(int entry, const vector<vector<ChainEntry>>& chain2,
                               vector<Top2>& bucket, vector<int>& touched, vector<ll>& ans) {
    const int tot = calc_sz(entry, -1);
    const int c = find_centroid(entry, -1, tot);

    verts.clear();
    dists.clear();
    collect(c, -1, 0);
    const int m = (int)verts.size();
    for (int i = 0; i < m; ++i) {
      chain[verts[i]].push_back({c, dists[i]});
    }

    touched.clear();
    pending_v.clear();
    pending_l2.clear();
    pending_self.clear();
    for (int idx = 0; idx < m; ++idx) {
      const int v = verts[idx];
      const ll d1 = dists[idx];
      for (const ChainEntry& e : chain2[v]) {
        Top2& t = bucket[e.centroid];
        if (t.b1 == k_inf) {
          touched.push_back(e.centroid);
        }
        t.add(d1 + e.dist, v);
        pending_v.push_back(v);
        pending_l2.push_back(e.centroid);
        pending_self.push_back(d1 + e.dist);
      }
    }

    for (int i = 0; i < (int)pending_v.size(); ++i) {
      const int v = pending_v[i];
      const ll other = bucket[pending_l2[i]].best_excluding(v);
      if (other < k_inf) {
        const ll cand = pending_self[i] + other;
        if (cand < ans[v]) {
          ans[v] = cand;
        }
      }
    }

    for (int l2 : touched) {
      bucket[l2].clear();
    }

    dead[c] = true;
    for (const auto& [v, w] : adj[c]) {
      if (!dead[v]) {
        decompose_with_callback(v, chain2, bucket, touched, ans);
      }
    }
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;

  CentroidDecomp t1(n), t2(n);
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    ll w;
    cin >> u >> v >> w;
    t1.add_edge(u - 1, v - 1, w);
  }
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    ll w;
    cin >> u >> v >> w;
    t2.add_edge(u - 1, v - 1, w);
  }

  t2.decompose(0);

  vector<ll> ans(n, k_inf);
  vector<Top2> bucket(n);
  vector<int> touched;
  touched.reserve(64);
  t1.pending_v.reserve(n * 20);
  t1.pending_l2.reserve(n * 20);
  t1.pending_self.reserve(n * 20);
  t1.decompose_with_callback(0, t2.chain, bucket, touched, ans);

  for (int i = 0; i < n; ++i) {
    cout << ans[i] << '\n';
  }
  return 0;
}

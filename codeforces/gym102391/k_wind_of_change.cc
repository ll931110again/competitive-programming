// CF Gym 102391 K — Wind of Change (Tree Product Metric Voronoi)
// https://codeforces.com/gym/102391/problem/K
//
// Dual centroid decomposition (KAIST RUN 2019 Fall editorial).
// For each vertex i, answer = min_{j!=i} dist(T1,i,j) + dist(T2,i,j).
// Relax buckets keyed by centroid ancestors (L1, L2): among vertices in both
// subtrees, minimize dist(T1,L1,j) + dist(T2,L2,j). O(n log^2 n) time,
// O(n log n) memory.

#include <iostream>
#include <limits>
#include <vector>

using ll = long long;
static const ll INF = std::numeric_limits<ll>::max() / 4;

struct Top2 {
  ll b1 = INF;
  ll b2 = INF;
  int i1 = -1;
  int i2 = -1;

  void clear() {
    b1 = b2 = INF;
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

  ll bestExcluding(int id) const {
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
  std::vector<std::vector<std::pair<int, ll>>> adj;
  std::vector<int> sz;
  std::vector<char> dead;
  std::vector<std::vector<ChainEntry>> chain;
  std::vector<int> verts;
  std::vector<ll> dists;
  std::vector<int> pending_v;
  std::vector<int> pending_l2;
  std::vector<ll> pending_self;

  explicit CentroidDecomp(int n_) : n(n_), adj(n_), sz(n_), dead(n_), chain(n_), verts(), dists() {
    verts.reserve(n);
    dists.reserve(n);
    for (int i = 0; i < n; ++i) {
      chain[i].reserve(20);
    }
  }

  void addEdge(int u, int v, ll w) {
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
  }

  int calcSz(int u, int p) {
    sz[u] = 1;
    for (const auto& [v, w] : adj[u]) {
      if (v == p || dead[v]) {
        continue;
      }
      sz[u] += calcSz(v, u);
    }
    return sz[u];
  }

  int findCentroid(int u, int p, int tot) {
    for (const auto& [v, w] : adj[u]) {
      if (v == p || dead[v]) {
        continue;
      }
      if (sz[v] > tot / 2) {
        return findCentroid(v, u, tot);
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
    const int tot = calcSz(entry, -1);
    const int c = findCentroid(entry, -1, tot);

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

  void decomposeWithCallback(int entry, const std::vector<std::vector<ChainEntry>>& chain2,
                             std::vector<Top2>& bucket, std::vector<int>& touched,
                             std::vector<ll>& ans) {
    const int tot = calcSz(entry, -1);
    const int c = findCentroid(entry, -1, tot);

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
        if (t.b1 == INF) {
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
      const ll other = bucket[pending_l2[i]].bestExcluding(v);
      if (other < INF) {
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
        decomposeWithCallback(v, chain2, bucket, touched, ans);
      }
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n;
  std::cin >> n;

  CentroidDecomp t1(n), t2(n);
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    ll w;
    std::cin >> u >> v >> w;
    t1.addEdge(u - 1, v - 1, w);
  }
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    ll w;
    std::cin >> u >> v >> w;
    t2.addEdge(u - 1, v - 1, w);
  }

  t2.decompose(0);

  std::vector<ll> ans(n, INF);
  std::vector<Top2> bucket(n);
  std::vector<int> touched;
  touched.reserve(64);
  t1.pending_v.reserve(n * 20);
  t1.pending_l2.reserve(n * 20);
  t1.pending_self.reserve(n * 20);
  t1.decomposeWithCallback(0, t2.chain, bucket, touched, ans);

  for (int i = 0; i < n; ++i) {
    std::cout << ans[i] << '\n';
  }
  return 0;
}

// CF Gym 102058 K / BOJ 16192 — Voronoi Diagram Returns
// https://codeforces.com/gym/102058/problem/K
//
// Build Delaunay triangulation (dual of the Voronoi diagram).  Site i belongs
// to the query's Voronoi region iff d(q, Pi) <= d(q, Pj) for every Delaunay
// neighbor j of i; checking only Delaunay neighbors is sufficient.
//
// Preprocessing: Bowyer–Watson Delaunay in O(n^2) (n <= 2000).
// Query: hill-climb on the Delaunay graph to find min distance, then collect
// every site at that distance whose Voronoi cell (via neighbor half-planes)
// contains the query.

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Pt {
  long long x, y;
};

long long cross(const Pt& o, const Pt& a, const Pt& b) {
  return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

long long dist2(const Pt& a, const Pt& b) {
  long long dx = a.x - b.x, dy = a.y - b.y;
  return dx * dx + dy * dy;
}

bool in_circumcircle(const Pt& p, const Pt& a, const Pt& b, const Pt& c) {
  return cross(a, b, c) * (dist2(p, a) * cross(b, c, p) + dist2(p, b) * cross(c, a, p) +
                           dist2(p, c) * cross(a, b, p)) >
         0;
}

struct Tri {
  int a, b, c;
};

vector<Tri> delaunay(vector<Pt> pts) {
  int n = (int)pts.size();
  long long mx = pts[0].x, Mx = mx, my = pts[0].y, My = my;
  for (const Pt& p : pts) {
    mx = min(mx, p.x);
    Mx = max(Mx, p.x);
    my = min(my, p.y);
    My = max(My, p.y);
  }
  long long d = max(Mx - mx, My - my) * 20 + 10;
  int s0 = n, s1 = n + 1, s2 = n + 2;
  pts.push_back({mx - d, my - d});
  pts.push_back({Mx + 3 * d, my - d});
  pts.push_back({mx + d, My + 3 * d});

  vector<Tri> tris = {{s0, s1, s2}};
  for (int i = 0; i < n; ++i) {
    vector<pair<int, int>> edges;
    vector<char> dead(tris.size());
    for (size_t t = 0; t < tris.size(); ++t) {
      const Tri& tr = tris[t];
      if (in_circumcircle(pts[i], pts[tr.a], pts[tr.b], pts[tr.c])) {
        dead[t] = 1;
        edges.push_back({min(tr.a, tr.b), max(tr.a, tr.b)});
        edges.push_back({min(tr.b, tr.c), max(tr.b, tr.c)});
        edges.push_back({min(tr.c, tr.a), max(tr.c, tr.a)});
      }
    }
    sort(edges.begin(), edges.end());
    vector<pair<int, int>> boundary;
    for (size_t j = 0; j < edges.size();) {
      size_t k = j + 1;
      while (k < edges.size() && edges[k] == edges[j]) {
        ++k;
      }
      if (k - j == 1) {
        boundary.push_back(edges[j]);
      }
      j = k;
    }
    vector<Tri> nxt;
    for (size_t t = 0; t < tris.size(); ++t) {
      if (!dead[t]) {
        nxt.push_back(tris[t]);
      }
    }
    for (const auto& [u, v] : boundary) {
      nxt.push_back({i, u, v});
    }
    tris.swap(nxt);
  }

  vector<Tri> out;
  for (const Tri& tr : tris) {
    if (tr.a < n && tr.b < n && tr.c < n) {
      out.push_back(tr);
    }
  }
  return out;
}

struct Voronoi {
  vector<Pt> site;
  vector<vector<int>> adj;

  void build(const vector<Pt>& pts) {
    site = pts;
    int n = (int)site.size();
    vector<Tri> tris = delaunay(site);
    adj.assign(n, {});
    auto add_edge = [&](int u, int v) {
      if (u == v || u < 0 || v < 0 || u >= n || v >= n) {
        return;
      }
      adj[u].push_back(v);
      adj[v].push_back(u);
    };
    for (const Tri& tr : tris) {
      add_edge(tr.a, tr.b);
      add_edge(tr.b, tr.c);
      add_edge(tr.c, tr.a);
    }
    for (int i = 0; i < n; ++i) {
      sort(adj[i].begin(), adj[i].end());
      adj[i].erase(unique(adj[i].begin(), adj[i].end()), adj[i].end());
    }
  }

  // q lies in the closure of cell i iff it is not strictly closer to any
  // Delaunay neighbor of i (Voronoi cell = intersection of neighbor bisectors).
  bool in_cell(int i, const Pt& q) const {
    long long di = dist2(q, site[i]);
    for (int j : adj[i]) {
      if (di > dist2(q, site[j])) {
        return false;
      }
    }
    return true;
  }

  long long min_dist2(const Pt& q) const {
    int cur = 0;
    long long best = dist2(q, site[cur]);
    bool improved = true;
    while (improved) {
      improved = false;
      for (int u : adj[cur]) {
        long long d = dist2(q, site[u]);
        if (d < best) {
          best = d;
          cur = u;
          improved = true;
        }
      }
    }
    return best;
  }

  vector<int> regions(const Pt& q) const {
    long long md = min_dist2(q);
    vector<int> ans;
    for (int i = 0; i < (int)site.size(); ++i) {
      if (dist2(q, site[i]) == md && in_cell(i, q)) {
        ans.push_back(i);
      }
    }
    return ans;
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, qn;
  cin >> n >> qn;
  vector<Pt> site(n);
  for (int i = 0; i < n; ++i) {
    cin >> site[i].x >> site[i].y;
  }

  Voronoi voro;
  voro.build(site);

  while (qn--) {
    Pt query;
    cin >> query.x >> query.y;

    vector<int> close = voro.regions(query);
    for (int& id : close) {
      ++id;
    }

    if (close.empty()) {
      cout << "NONE\n";
    } else if (close.size() == 1) {
      cout << "REGION " << close[0] << '\n';
    } else if (close.size() == 2) {
      cout << "LINE " << close[0] << ' ' << close[1] << '\n';
    } else {
      cout << "POINT\n";
    }
  }
  return 0;
}

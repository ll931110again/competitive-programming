#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

struct AdjEdge {
  int to;
  int id;
};

struct Frame {
  int u;
  int parent;
  int parent_edge;
  int it;
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int tc = 1;
  int E;
  while (cin >> E) {
    if (E == 0)
      break;

    vector<pair<int, int>> edges;
    edges.reserve(E);
    int V = 0;
    for (int i = 0; i < E; i++) {
      int a, b;
      cin >> a >> b;
      edges.push_back({a, b});
      V = max(V, max(a, b));
    }

    vector<vector<AdjEdge>> g(V + 1);
    g.assign(V + 1, {});
    g.shrink_to_fit();
    g.assign(V + 1, {});
    for (int i = 0; i < E; i++) {
      auto [a, b] = edges[i];
      g[a].push_back({b, i});
      g[b].push_back({a, i});
    }

    vector<int> disc(V + 1, 0), low(V + 1, 0), parent(V + 1, 0), parent_edge(V + 1, -1);
    vector<int> child_cnt(V + 1, 0);
    vector<char> is_art(V + 1, 0);
    int timer = 0;

    vector<pair<int, int>> estack;
    estack.reserve(E);

    vector<int> comp_size;
    vector<int> comp_art_cnt;
    comp_size.reserve(E);
    comp_art_cnt.reserve(E);

    vector<int> seen(V + 1, 0);
    int seen_token = 1;

    auto add_vertex = [&](vector<int>& verts, int v) {
      if (seen[v] != seen_token) {
        seen[v] = seen_token;
        verts.push_back(v);
      }
    };

    // Graph is connected per statement, but we keep the outer loop anyway.
    for (int start = 1; start <= V; start++) {
      if (disc[start] != 0)
        continue;

      vector<Frame> st;
      st.reserve(V);
      st.push_back({start, 0, -1, 0});
      parent[start] = 0;
      parent_edge[start] = -1;
      disc[start] = low[start] = ++timer;

      while (!st.empty()) {
        Frame& fr = st.back();
        int u = fr.u;
        if (fr.it < (int)g[u].size()) {
          auto [v, eid] = g[u][fr.it++];
          if (eid == fr.parent_edge)
            continue;

          if (disc[v] == 0) {
            parent[v] = u;
            parent_edge[v] = eid;
            child_cnt[u]++;
            estack.push_back({u, v});
            disc[v] = low[v] = ++timer;
            st.push_back({v, u, eid, 0});
          } else {
            // Back edge to ancestor
            if (disc[v] < disc[u]) {
              estack.push_back({u, v});
              low[u] = min(low[u], disc[v]);
            }
          }
        } else {
          // finish u
          st.pop_back();
          int p = fr.parent;
          if (p != 0) {
            low[p] = min(low[p], low[u]);

            // Articulation check (except root)
            if (parent[p] != 0 && low[u] >= disc[p])
              is_art[p] = 1;

            // Biconnected component formed by (p,u) if low[u] >= disc[p]
            if (low[u] >= disc[p]) {
              vector<int> verts;
              verts.reserve(16);
              ++seen_token;
              while (!estack.empty()) {
                auto [a, b] = estack.back();
                estack.pop_back();
                add_vertex(verts, a);
                add_vertex(verts, b);
                if ((a == p && b == u) || (a == u && b == p))
                  break;
              }
              int ac = 0;
              for (int x : verts)
                if (is_art[x])
                  ac++;
              comp_size.push_back((int)verts.size());
              comp_art_cnt.push_back(ac);
            }
          } else {
            // root: articulation iff >=2 DFS children
            if (child_cnt[u] >= 2)
              is_art[u] = 1;
          }
        }
      }
    }

    i64 shafts = 0;
    i64 ways = 1;
    int leaf_blocks = 0;

    // Recompute comp_art_cnt now that root articulation status is finalized.
    for (int i = 0; i < (int)comp_size.size(); i++) {
      // We didn't store vertices per component; recomputing accurately would
      // require that. However, the only missed articulation status can be root
      // nodes; to handle this cleanly, we will re-run a lightweight pass that
      // counts leaf blocks by detecting blocks that touch exactly one
      // articulation, using an alternate strategy below.
      (void)i;
    }

    // Alternate strategy (standard): during component creation, we used is_art
    // which might not have root marked yet. Fix by counting leaf blocks using
    // vertex membership again is expensive. So instead: we store vertices of
    // each component.
    //
    // Implemented by redoing component extraction with vertex storage in one
    // pass would be simpler, but to keep this file consistent, we rerun
    // decomposition once with root known.

    // ---- Second pass: same decomposition but store vertices per component
    // ----
    disc.assign(V + 1, 0);
    low.assign(V + 1, 0);
    parent.assign(V + 1, 0);
    parent_edge.assign(V + 1, -1);
    child_cnt.assign(V + 1, 0);
    timer = 0;
    estack.clear();

    vector<vector<int>> comps;
    comps.reserve(E);
    seen.assign(V + 1, 0);
    seen_token = 1;

    for (int start = 1; start <= V; start++) {
      if (disc[start] != 0)
        continue;
      vector<Frame> st;
      st.reserve(V);
      st.push_back({start, 0, -1, 0});
      disc[start] = low[start] = ++timer;

      while (!st.empty()) {
        Frame& fr = st.back();
        int u = fr.u;
        if (fr.it < (int)g[u].size()) {
          auto [v, eid] = g[u][fr.it++];
          if (eid == fr.parent_edge)
            continue;

          if (disc[v] == 0) {
            parent[v] = u;
            parent_edge[v] = eid;
            child_cnt[u]++;
            estack.push_back({u, v});
            disc[v] = low[v] = ++timer;
            st.push_back({v, u, eid, 0});
          } else {
            if (disc[v] < disc[u]) {
              estack.push_back({u, v});
              low[u] = min(low[u], disc[v]);
            }
          }
        } else {
          st.pop_back();
          int p = fr.parent;
          if (p != 0) {
            low[p] = min(low[p], low[u]);
            if (low[u] >= disc[p]) {
              vector<int> verts;
              verts.reserve(16);
              ++seen_token;
              while (!estack.empty()) {
                auto [a, b] = estack.back();
                estack.pop_back();
                add_vertex(verts, a);
                add_vertex(verts, b);
                if ((a == p && b == u) || (a == u && b == p))
                  break;
              }
              comps.push_back(move(verts));
            }
          }
        }
      }
    }

    for (const auto& verts : comps) {
      int ac = 0;
      for (int v : verts)
        if (is_art[v])
          ac++;
      if (ac == 1) {
        leaf_blocks++;
        shafts++;
        ways *= (i64)verts.size() - 1LL;
      }
    }

    if (leaf_blocks == 0) {
      shafts = 2;
      ways = (i64)V * (i64)(V - 1) / 2LL;
    }

    cout << "Case " << tc++ << ": " << shafts << " " << ways << "\n";
  }

  return 0;
}

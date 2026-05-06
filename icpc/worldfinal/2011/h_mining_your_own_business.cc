#include <algorithm>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>
 
using namespace std;
 
struct AdjEdge {
  int to;
  int id;
};
 
struct Frame {
  int u;
  int parent;
  int parentEdge;
  int it;
};
 
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
 
  int tc = 1;
  int E;
  while (cin >> E) {
    if (E == 0) break;
 
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
 
    vector<int> disc(V + 1, 0), low(V + 1, 0), parent(V + 1, 0), parentEdge(V + 1, -1);
    vector<int> childCnt(V + 1, 0);
    vector<char> isArt(V + 1, 0);
    int timer = 0;
 
    vector<pair<int, int>> estack;
    estack.reserve(E);
 
    vector<int> compSize;
    vector<int> compArtCnt;
    compSize.reserve(E);
    compArtCnt.reserve(E);
 
    vector<int> seen(V + 1, 0);
    int seenToken = 1;
 
    auto addVertex = [&](vector<int> &verts, int v) {
      if (seen[v] != seenToken) {
        seen[v] = seenToken;
        verts.push_back(v);
      }
    };
 
    // Graph is connected per statement, but we keep the outer loop anyway.
    for (int start = 1; start <= V; start++) {
      if (disc[start] != 0) continue;
 
      vector<Frame> st;
      st.reserve(V);
      st.push_back({start, 0, -1, 0});
      parent[start] = 0;
      parentEdge[start] = -1;
      disc[start] = low[start] = ++timer;
 
      while (!st.empty()) {
        Frame &fr = st.back();
        int u = fr.u;
        if (fr.it < (int)g[u].size()) {
          auto [v, eid] = g[u][fr.it++];
          if (eid == fr.parentEdge) continue;
 
          if (disc[v] == 0) {
            parent[v] = u;
            parentEdge[v] = eid;
            childCnt[u]++;
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
            if (parent[p] != 0 && low[u] >= disc[p]) isArt[p] = 1;
 
            // Biconnected component formed by (p,u) if low[u] >= disc[p]
            if (low[u] >= disc[p]) {
              vector<int> verts;
              verts.reserve(16);
              ++seenToken;
              while (!estack.empty()) {
                auto [a, b] = estack.back();
                estack.pop_back();
                addVertex(verts, a);
                addVertex(verts, b);
                if ((a == p && b == u) || (a == u && b == p)) break;
              }
              int ac = 0;
              for (int x : verts)
                if (isArt[x]) ac++;
              compSize.push_back((int)verts.size());
              compArtCnt.push_back(ac);
            }
          } else {
            // root: articulation iff >=2 DFS children
            if (childCnt[u] >= 2) isArt[u] = 1;
          }
        }
      }
    }
 
    long long shafts = 0;
    long long ways = 1;
    int leafBlocks = 0;
 
    // Recompute compArtCnt now that root articulation status is finalized.
    for (int i = 0; i < (int)compSize.size(); i++) {
      // We didn't store vertices per component; recomputing accurately would require that.
      // However, the only missed articulation status can be root nodes; to handle this cleanly,
      // we will re-run a lightweight pass that counts leaf blocks by detecting blocks that touch
      // exactly one articulation, using an alternate strategy below.
      (void)i;
    }
 
    // Alternate strategy (standard): during component creation, we used isArt which might not
    // have root marked yet. Fix by counting leaf blocks using vertex membership again is expensive.
    // So instead: we store vertices of each component.
    //
    // Implemented by redoing component extraction with vertex storage in one pass would be simpler,
    // but to keep this file consistent, we rerun decomposition once with root known.
 
    // ---- Second pass: same decomposition but store vertices per component ----
    disc.assign(V + 1, 0);
    low.assign(V + 1, 0);
    parent.assign(V + 1, 0);
    parentEdge.assign(V + 1, -1);
    childCnt.assign(V + 1, 0);
    timer = 0;
    estack.clear();
 
    vector<vector<int>> comps;
    comps.reserve(E);
    seen.assign(V + 1, 0);
    seenToken = 1;
 
    for (int start = 1; start <= V; start++) {
      if (disc[start] != 0) continue;
      vector<Frame> st;
      st.reserve(V);
      st.push_back({start, 0, -1, 0});
      disc[start] = low[start] = ++timer;
 
      while (!st.empty()) {
        Frame &fr = st.back();
        int u = fr.u;
        if (fr.it < (int)g[u].size()) {
          auto [v, eid] = g[u][fr.it++];
          if (eid == fr.parentEdge) continue;
 
          if (disc[v] == 0) {
            parent[v] = u;
            parentEdge[v] = eid;
            childCnt[u]++;
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
              ++seenToken;
              while (!estack.empty()) {
                auto [a, b] = estack.back();
                estack.pop_back();
                addVertex(verts, a);
                addVertex(verts, b);
                if ((a == p && b == u) || (a == u && b == p)) break;
              }
              comps.push_back(std::move(verts));
            }
          }
        }
      }
    }
 
    for (const auto &verts : comps) {
      int ac = 0;
      for (int v : verts)
        if (isArt[v]) ac++;
      if (ac == 1) {
        leafBlocks++;
        shafts++;
        ways *= (long long)verts.size() - 1LL;
      }
    }
 
    if (leafBlocks == 0) {
      shafts = 2;
      ways = (long long)V * (long long)(V - 1) / 2LL;
    }
 
    cout << "Case " << tc++ << ": " << shafts << " " << ways << "\n";
  }
 
  return 0;
}
#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

struct Edge {
  int to;
  int id;
};

struct TarjanFrame {
  int v;
  int peid; // parent edge id into v (-1 for DFS root)
  int ei;   // next adjacency index
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;

  vector<vector<Edge>> g(n + 1);
  vector<pair<int, int>> edges(m);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    edges[i] = {a, b};
    g[a].push_back({b, i});
    g[b].push_back({a, i});
  }

  // ---------- Tarjan BCC: iterative DFS (avoids stack overflow on long paths)
  // ----------
  vector<int> tin(n + 1, 0), low(n + 1, 0);
  int timer = 0;
  vector<int> stEid;
  vector<vector<pair<int, int>>>
      comps; // each component: list of undirected edges (u,v)

  auto addComp = [&](int lastEid) {
    vector<pair<int, int>> comp;
    while (!stEid.empty()) {
      int eid = stEid.back();
      stEid.pop_back();
      comp.push_back(edges[eid]);
      if (eid == lastEid)
        break;
    }
    comps.push_back(std::move(comp));
  };

  vector<TarjanFrame> stk;
  stk.reserve(n + 5);

  for (int start = 1; start <= n; start++) {
    if (tin[start])
      continue;
    stk.clear();
    stk.push_back({start, -1, 0});
    while (!stk.empty()) {
      TarjanFrame &f = stk.back();
      int v = f.v;
      if (!tin[v])
        tin[v] = low[v] = ++timer;

      bool pushedChild = false;
      while (f.ei < (int)g[v].size()) {
        Edge e = g[v][f.ei++];
        if (e.id == f.peid)
          continue;
        int to = e.to;
        if (tin[to]) {
          low[v] = min(low[v], tin[to]);
          if (tin[to] < tin[v])
            stEid.push_back(e.id);
        } else {
          stEid.push_back(e.id);
          stk.push_back({to, e.id, 0});
          pushedChild = true;
          break;
        }
      }
      if (pushedChild)
        continue;

      int completed = v;
      int childPe = f.peid;
      stk.pop_back();
      if (stk.empty())
        continue;

      TarjanFrame &par = stk.back();
      int pv = par.v;
      low[pv] = min(low[pv], low[completed]);
      if (low[completed] >= tin[pv])
        addComp(childPe);
    }
  }

  int B = (int)comps.size();
  vector<vector<int>> vertOfBcc(B);
  for (int bi = 0; bi < B; bi++) {
    for (auto [u, v] : comps[bi]) {
      vertOfBcc[bi].push_back(u);
      vertOfBcc[bi].push_back(v);
    }
    auto &vec = vertOfBcc[bi];
    sort(vec.begin(), vec.end());
    vec.erase(unique(vec.begin(), vec.end()), vec.end());
  }

  vector<int> edgeBcc(m, -1);
  for (int bi = 0; bi < B; bi++) {
    for (auto [u, v] : comps[bi]) {
      for (Edge e : g[u]) {
        if (e.to == v)
          edgeBcc[e.id] = bi;
      }
    }
  }

  vector<int> tmpCol(n + 1, -1);
  vector<char> bipBlock(B, 0);
  for (int bi = 0; bi < B; bi++) {
    const auto &vs = vertOfBcc[bi];
    for (int vv : vs)
      tmpCol[vv] = -1;
    bool ok = true;
    queue<int> q;
    int start = vs[0];
    tmpCol[start] = 0;
    q.push(start);
    while (!q.empty()) {
      int x = q.front();
      q.pop();
      for (Edge e : g[x]) {
        if (edgeBcc[e.id] != bi)
          continue;
        int y = e.to;
        if (tmpCol[y] == -1) {
          tmpCol[y] = tmpCol[x] ^ 1;
          q.push(y);
        } else if (tmpCol[y] == tmpCol[x]) {
          ok = false;
        }
      }
    }
    bipBlock[bi] = ok;
  }

  vector<vector<int>> vertexBccs(n + 1);
  for (int bi = 0; bi < B; bi++) {
    for (int v : vertOfBcc[bi]) {
      vertexBccs[v].push_back(bi);
    }
  }

  vector<char> isArt(n + 1, 0);
  vector<int> belong(n + 1, -1);
  for (int v = 1; v <= n; v++) {
    if ((int)vertexBccs[v].size() >= 2)
      isArt[v] = 1;
    if ((int)vertexBccs[v].size() == 1)
      belong[v] = vertexBccs[v][0];
  }

  // ---------- Block-cut tree ----------
  int T = 0;
  vector<int> blockNode(B, -1);
  for (int bi = 0; bi < B; bi++)
    blockNode[bi] = T++;
  vector<int> artNode(n + 1, -1);
  for (int v = 1; v <= n; v++) {
    if (isArt[v])
      artNode[v] = T++;
  }

  vector<vector<int>> tree(T);
  for (int bi = 0; bi < B; bi++) {
    int bn = blockNode[bi];
    for (int v : vertOfBcc[bi]) {
      if (isArt[v]) {
        int an = artNode[v];
        tree[bn].push_back(an);
        tree[an].push_back(bn);
      }
    }
  }

  auto repr = [&](int v) -> int {
    if (isArt[v])
      return artNode[v];
    if (belong[v] == -1)
      return -1; // isolated vertex (no edges)
    return blockNode[belong[v]];
  };

  vector<char> isBlockNode(T, 0);
  vector<int> blockIdOf(T, -1);
  for (int bi = 0; bi < B; bi++) {
    isBlockNode[blockNode[bi]] = 1;
    blockIdOf[blockNode[bi]] = bi;
  }

  vector<char> oddOnNode(T, 0);
  for (int x = 0; x < T; x++) {
    if (isBlockNode[x] && !bipBlock[blockIdOf[x]])
      oddOnNode[x] = 1;
  }

  // Connected components of original graph (for coloring / sanity)
  vector<int> compId(n + 1, -1);
  vector<char> compBip;
  vector<int> gcolor(n + 1, -1);
  int cid = 0;
  for (int s = 1; s <= n; s++) {
    if (compId[s] != -1)
      continue;
    bool ok = true;
    compId[s] = cid;
    gcolor[s] = 0;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
      int x = q.front();
      q.pop();
      for (Edge e : g[x]) {
        int y = e.to;
        if (compId[y] == -1) {
          compId[y] = cid;
          gcolor[y] = gcolor[x] ^ 1;
          q.push(y);
        } else if (gcolor[y] == gcolor[x]) {
          ok = false;
        }
      }
    }
    compBip.push_back(ok);
    cid++;
  }

  vector<int> parent(T, -1), depth(T, 0);
  {
    vector<char> seen(T, 0);
    queue<int> q;
    for (int root = 0; root < T; root++) {
      if (seen[root])
        continue;
      seen[root] = 1;
      parent[root] = root;
      depth[root] = 0;
      q.push(root);
      while (!q.empty()) {
        int x = q.front();
        q.pop();
        for (int y : tree[x]) {
          if (!seen[y]) {
            seen[y] = 1;
            parent[y] = x;
            depth[y] = depth[x] + 1;
            q.push(y);
          }
        }
      }
    }
    for (int x = 0; x < T; x++) {
      if (parent[x] == -1)
        parent[x] = x;
    }
  }

  const int LOG = 20;
  vector<vector<int>> up(LOG, vector<int>(T));
  vector<vector<char>> mxOdd(LOG, vector<char>(T));
  for (int x = 0; x < T; x++) {
    up[0][x] = parent[x];
    if (parent[x] == x)
      mxOdd[0][x] = oddOnNode[x];
    else
      mxOdd[0][x] = oddOnNode[x] | oddOnNode[parent[x]];
  }
  for (int k = 1; k < LOG; k++) {
    for (int x = 0; x < T; x++) {
      up[k][x] = up[k - 1][up[k - 1][x]];
      mxOdd[k][x] = mxOdd[k - 1][x] | mxOdd[k - 1][up[k - 1][x]];
    }
  }

  auto climbRoot = [&](int x) {
    while (parent[x] != x)
      x = parent[x];
    return x;
  };

  auto lca = [&](int a, int b) -> int {
    if (climbRoot(a) != climbRoot(b))
      return -1;
    if (depth[a] < depth[b])
      swap(a, b);
    int diff = depth[a] - depth[b];
    for (int k = 0; k < LOG; k++) {
      if ((diff >> k) & 1)
        a = up[k][a];
    }
    if (a == b)
      return a;
    for (int k = LOG - 1; k >= 0; k--) {
      if (up[k][a] != up[k][b]) {
        a = up[k][a];
        b = up[k][b];
      }
    }
    return parent[a];
  };

  // OR of oddOnNode on BC-tree path between x and anc (anc must be an ancestor
  // of x in this tree)
  auto pathOrInclusive = [&](int x, int anc) -> bool {
    if (climbRoot(x) != climbRoot(anc))
      return false;
    if (x == anc)
      return oddOnNode[x];
    int diff = depth[x] - depth[anc];
    bool res = false;
    int z = x;
    for (int k = 0; k < LOG; k++) {
      if ((diff >> k) & 1) {
        res |= mxOdd[k][z];
        z = up[k][z];
      }
    }
    return res;
  };

  auto pathHasOdd = [&](int a, int b) -> bool {
    int w = lca(a, b);
    if (w < 0)
      return false;
    return pathOrInclusive(a, w) || pathOrInclusive(b, w);
  };

  int q;
  cin >> q;
  vector<int> bfsVis(n + 1, 0);
  vector<int> distBfs(n + 1, 0);
  int bfsTick = 0;

  while (q--) {
    int u, v;
    cin >> u >> v;
    if (u == v) {
      cout << "No\n";
      continue;
    }
    if (compId[u] != compId[v]) {
      cout << "No\n";
      continue;
    }

    int nu = repr(u), nv = repr(v);
    if (nu < 0 || nv < 0) {
      cout << "No\n";
      continue;
    }
    if (pathHasOdd(nu, nv)) {
      cout << "Yes\n";
      continue;
    }

    int c = compId[u];
    if (compBip[c]) {
      cout << (gcolor[u] != gcolor[v] ? "Yes\n" : "No\n");
      continue;
    }

    int wuv = lca(nu, nv);
    long long distNuNv = (long long)depth[nu] + depth[nv] - 2LL * depth[wuv];

    unordered_map<int, bool> blockMemo;
    blockMemo.reserve(64);

    auto blockOnQueryPath = [&](int bn) -> bool {
      auto it = blockMemo.find(bn);
      if (it != blockMemo.end())
        return it->second;
      int w1 = lca(nu, bn);
      int w2 = lca(bn, nv);
      if (w1 < 0 || w2 < 0) {
        blockMemo[bn] = false;
        return false;
      }
      long long d1 = (long long)depth[nu] + depth[bn] - 2LL * depth[w1];
      long long d2 = (long long)depth[bn] + depth[nv] - 2LL * depth[w2];
      bool ok = d1 + d2 == distNuNv;
      blockMemo[bn] = ok;
      return ok;
    };

    ++bfsTick;
    queue<int> qq;
    bfsVis[u] = bfsTick;
    distBfs[u] = 0;
    qq.push(u);
    while (!qq.empty()) {
      int x = qq.front();
      qq.pop();
      for (Edge e : g[x]) {
        int bi = edgeBcc[e.id];
        if (!blockOnQueryPath(blockNode[bi]))
          continue;
        int y = e.to;
        if (bfsVis[y] != bfsTick) {
          bfsVis[y] = bfsTick;
          distBfs[y] = distBfs[x] + 1;
          qq.push(y);
        }
      }
    }

    cout << ((bfsVis[v] == bfsTick && (distBfs[v] % 2 == 1)) ? "Yes\n"
                                                             : "No\n");
  }

  return 0;
}

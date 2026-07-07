// AHC060 A - Ice Cream Shop Tour: https://atcoder.jp/contests/ahc060/tasks/ahc060_a
//
// BFS tree from shop 0; LCA paths respect current position and no backtrack.
// Multi-start randomized BFS + shop order permutations under time budget.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

struct Input {
  int n = 0, m = 0, k = 0, turn_limit = 0;
  vector<vector<int>> adj;
};

struct TreeLayout {
  vector<int> parent;
  vector<int> depth;
};

struct Solver {
  Input in;

  TreeLayout build_tree(mt19937& rng) const {
    TreeLayout tree;
    tree.parent.assign(in.n, -1);
    tree.depth.assign(in.n, 0);
    vector<char> vis(in.n, 0);
    queue<int> q;
    tree.parent[0] = 0;
    vis[0] = 1;
    q.push(0);
    while (!q.empty()) {
      const int u = q.front();
      q.pop();
      vector<int> nbr = in.adj[u];
      shuffle(nbr.begin(), nbr.end(), rng);
      for (int v : nbr) {
        if (vis[v])
          continue;
        vis[v] = 1;
        tree.parent[v] = u;
        tree.depth[v] = tree.depth[u] + 1;
        q.push(v);
      }
    }
    return tree;
  }

  int lca(int u, int v, const TreeLayout& tree) const {
    while (tree.depth[u] > tree.depth[v])
      u = tree.parent[u];
    while (tree.depth[v] > tree.depth[u])
      v = tree.parent[v];
    while (u != v) {
      u = tree.parent[u];
      v = tree.parent[v];
    }
    return u;
  }

  vector<int> path_vertices(int from, int to, const TreeLayout& tree) const {
    if (from == to)
      return {from};
    if (tree.parent[from] < 0 || tree.parent[to] < 0)
      return {};
    const int anc = lca(from, to, tree);
    vector<int> path;
    for (int v = from; v != anc; v = tree.parent[v])
      path.push_back(v);
    path.push_back(anc);
    vector<int> down;
    for (int v = to; v != anc; v = tree.parent[v])
      down.push_back(v);
    reverse(down.begin(), down.end());
    for (int v : down) {
      if (v != anc)
        path.push_back(v);
    }
    return path;
  }

  bool step(vector<int>& ops, int& cur, int& prev, int nxt) const {
    if (nxt == prev)
      return false;
    for (int v : in.adj[cur]) {
      if (v == nxt) {
        ops.push_back(nxt);
        prev = cur;
        cur = nxt;
        return true;
      }
    }
    return false;
  }

  bool walk(vector<int>& ops, int& cur, int& prev, int target,
            const TreeLayout& tree) const {
    const vector<int> verts = path_vertices(cur, target, tree);
    if (verts.empty() && cur != target)
      return false;
    if (verts.size() >= 2 && verts[1] == prev) {
      for (int v : in.adj[cur]) {
        if (v == prev)
          continue;
        vector<int> trial = ops;
        int tc = cur, tp = prev;
        if (!step(trial, tc, tp, v))
          continue;
        if (walk(trial, tc, tp, target, tree)) {
          ops = std::move(trial);
          cur = tc;
          prev = tp;
          return true;
        }
      }
      return false;
    }
    for (size_t i = 1; i < verts.size() && (int)ops.size() < in.turn_limit; ++i) {
      if (!step(ops, cur, prev, verts[i]))
        return false;
    }
    return cur == target;
  }

  int nearest_tree(int cur, const TreeLayout& tree) const {
    int best = -1, best_d = 1e9;
    for (int v = in.k; v < in.n; ++v) {
      if (tree.parent[v] < 0)
        continue;
      const int d = (int)path_vertices(cur, v, tree).size() - 1;
      if (d < best_d) {
        best_d = d;
        best = v;
      }
    }
    return best;
  }

  i64 score_ops(const vector<int>& ops) const {
    if ((int)ops.size() > in.turn_limit)
      return 0;
    int cur = 0, prev = -1;
    vector<int> delivered(in.k, 0);
    vector<char> cone;
    for (int nxt : ops) {
      if (nxt == prev)
        return 0;
      bool ok = false;
      for (int v : in.adj[cur]) {
        if (v == nxt) {
          ok = true;
          break;
        }
      }
      if (!ok)
        return 0;
      if (nxt < in.k) {
        if (!cone.empty()) {
          delivered[nxt]++;
          cone.clear();
        }
      } else {
        cone.push_back('W');
      }
      prev = cur;
      cur = nxt;
    }
    i64 s = 0;
    for (int d : delivered)
      s += d;
    return s;
  }

  vector<int> run(const TreeLayout& tree, const vector<int>& shop_order) const {
    vector<int> ops;
    ops.reserve(in.turn_limit);
    int cur = 0, prev = -1;
    vector<int> delivered(in.k, 0);

    for (int shop : shop_order) {
      if ((int)ops.size() >= in.turn_limit)
        break;
      if (!walk(ops, cur, prev, shop, tree))
        continue;
      while ((int)ops.size() < in.turn_limit && delivered[shop] < 5) {
        const int tree_v = nearest_tree(cur, tree);
        if (tree_v < 0)
          break;
        if (!walk(ops, cur, prev, tree_v, tree))
          break;
        if (!walk(ops, cur, prev, shop, tree))
          break;
        delivered[shop]++;
      }
    }
    return ops;
  }

  vector<int> solve() const {
    mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());
    const auto deadline =
        chrono::steady_clock::now() + chrono::milliseconds(1900);

    vector<int> best_ops;
    i64 best_score = 0;

    while (chrono::steady_clock::now() < deadline) {
      const TreeLayout tree = build_tree(rng);
      vector<int> ord(in.k);
      iota(ord.begin(), ord.end(), 0);
      vector<vector<int>> orders = {ord};
      sort(ord.begin(), ord.end(),
           [&](int a, int b) { return tree.depth[a] < tree.depth[b]; });
      orders.push_back(ord);
      shuffle(ord.begin(), ord.end(), rng);
      orders.push_back(ord);

      for (const auto& shop_order : orders) {
        const vector<int> ops = run(tree, shop_order);
        const i64 sc = score_ops(ops);
        if (sc > best_score) {
          best_score = sc;
          best_ops = ops;
        }
      }
    }
    return best_ops;
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Input in;
  cin >> in.n >> in.m >> in.k >> in.turn_limit;
  in.adj.assign(in.n, {});
  for (int i = 0; i < in.m; ++i) {
    int u, v;
    cin >> u >> v;
    in.adj[u].push_back(v);
    in.adj[v].push_back(u);
  }
  for (int i = 0; i < in.n; ++i) {
    int x, y;
    cin >> x >> y;
  }

  Solver solver{in};
  for (int op : solver.solve())
    cout << op << '\n';
  return 0;
}

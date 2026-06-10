// Max flow: Dinic level graphs + link-cut tree blocking flow.
#pragma once

#include <algorithm>
#include <climits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace maxflow {

struct LctMaxFlow {
  struct Edge {
    int u = 0;
    int v = 0;
    int flow = 0;
    int capacity = 0;
    int residual = 0;
    int rev = 0;
  };

  struct Node {
    int ch[2]{};
    int parent = 0;
    bool rev = false;
    int vtx = 0;
    int rep_parent = 0;
    int edge_to_parent = -1;
  };

  struct LevelInfo {
    int level = -1;
    std::unordered_set<int> outgoing;
    std::unordered_set<int> incoming;
  };

  int n = 0;
  std::vector<Edge> edges;
  std::vector<std::vector<int>> out_edges;
  std::vector<Node> nodes;
  std::unordered_map<int, LevelInfo> level_graph;

  explicit LctMaxFlow(int n_) : n(n_), out_edges(n_), nodes(n_ + 1) {
    for (int i = 1; i <= n; ++i) {
      nodes[i].vtx = i - 1;
    }
  }

  void add_edge(int u, int v, int cap) {
    const int fwd = static_cast<int>(edges.size());
    const int rev = fwd + 1;
    edges.push_back(Edge{u, v, 0, cap, cap, rev});
    edges.push_back(Edge{v, u, 0, 0, 0, fwd});
    out_edges[u].push_back(fwd);
    out_edges[v].push_back(rev);
  }

  int max_flow(int source, int target) {
    int total = 0;
    while (build_level_graph(source, target)) {
      total += blocking_flow(vtx_node(source), vtx_node(target));
      level_graph.clear();
    }
    return total;
  }

private:
  static int vtx_node(int v) {
    return v + 1;
  }
  static int node_vtx(int x) {
    return x - 1;
  }

  static int side(int x, const std::vector<Node>& t) {
    return t[t[x].parent].ch[1] == x;
  }

  static bool is_aux_root(int x, const std::vector<Node>& t) {
    const int p = t[x].parent;
    return p == 0 || (t[p].ch[0] != x && t[p].ch[1] != x);
  }

  static void apply_rev(int x, std::vector<Node>& t) {
    std::swap(t[x].ch[0], t[x].ch[1]);
    t[x].rev ^= true;
  }

  static void push(int x, std::vector<Node>& t) {
    if (!t[x].rev) {
      return;
    }
    if (t[x].ch[0]) {
      apply_rev(t[x].ch[0], t);
    }
    if (t[x].ch[1]) {
      apply_rev(t[x].ch[1], t);
    }
    t[x].rev = false;
  }

  static void rotate(int x, std::vector<Node>& t) {
    push(x, t);
    const int p = t[x].parent;
    push(p, t);
    const int g = t[p].parent;
    const int d = side(x, t);
    const int y = t[x].ch[d ^ 1];
    if (!is_aux_root(p, t)) {
      t[g].ch[side(p, t)] = x;
    }
    t[x].parent = g;
    t[x].ch[d ^ 1] = p;
    t[p].parent = x;
    t[p].ch[d] = y;
    if (y) {
      t[y].parent = p;
    }
  }

  static void splay(int x, std::vector<Node>& t) {
    static int stk[1 << 12];
    int top = 0;
    stk[++top] = x;
    for (int i = x; !is_aux_root(i, t); i = t[i].parent) {
      stk[++top] = t[i].parent;
    }
    while (top) {
      push(stk[top--], t);
    }
    while (!is_aux_root(x, t)) {
      const int p = t[x].parent;
      const int g = t[p].parent;
      if (is_aux_root(p, t)) {
        rotate(x, t);
      } else if (side(x, t) == side(p, t)) {
        rotate(p, t);
        rotate(x, t);
      } else {
        rotate(x, t);
        rotate(x, t);
      }
    }
  }

  static void access(int x, std::vector<Node>& t) {
    int last = 0;
    for (int y = x; y; y = t[y].parent) {
      splay(y, t);
      t[y].ch[1] = last;
      last = y;
    }
    splay(x, t);
  }

  static void make_root(int x, std::vector<Node>& t) {
    access(x, t);
    apply_rev(x, t);
  }

  void lct_link(int v, int w, int edge_id) {
    make_root(v, nodes);
    access(w, nodes);
    splay(w, nodes);
    nodes[v].parent = w;
    nodes[w].ch[1] = v;
    nodes[v].rep_parent = w;
    nodes[v].edge_to_parent = edge_id;
  }

  void lct_cut(int v) {
    access(v, nodes);
    splay(v, nodes);
    if (nodes[v].ch[0]) {
      nodes[nodes[v].ch[0]].parent = 0;
      nodes[v].ch[0] = 0;
    }
    nodes[v].rep_parent = 0;
    nodes[v].edge_to_parent = -1;
  }

  void reset_lct() {
    for (int i = 1; i <= n; ++i) {
      nodes[i].ch[0] = nodes[i].ch[1] = 0;
      nodes[i].parent = 0;
      nodes[i].rev = false;
      nodes[i].rep_parent = 0;
      nodes[i].edge_to_parent = -1;
    }
  }

  void add_flow_edge(int eid, int delta) {
    Edge& e = edges[eid];
    Edge& re = edges[e.rev];
    e.flow += delta;
    re.flow -= delta;
    e.residual = e.capacity - e.flow;
    re.residual = re.capacity - re.flow;
  }

  void collect_represented_path(int x, std::vector<int>& verts) {
    for (int u = x, depth = 0; u && depth <= n; u = nodes[u].rep_parent, ++depth) {
      verts.push_back(u);
    }
    std::reverse(verts.begin(), verts.end());
  }

  std::pair<int, int> path_mincost(int x) {
    std::vector<int> verts;
    collect_represented_path(x, verts);
    int min_child = -1;
    int min_eid = -1;
    int min_cap = INT_MAX;
    for (size_t i = 1; i < verts.size(); ++i) {
      const int child = verts[i];
      const int eid = nodes[child].edge_to_parent;
      if (eid < 0) {
        continue;
      }
      const int cap = edges[eid].residual;
      if (cap < min_cap) {
        min_cap = cap;
        min_child = child;
        min_eid = eid;
      }
    }
    return {min_child, min_eid};
  }

  void path_augment(int x, int delta) {
    std::vector<int> verts;
    collect_represented_path(x, verts);
    for (size_t i = 1; i < verts.size(); ++i) {
      const int eid = nodes[verts[i]].edge_to_parent;
      if (eid >= 0) {
        add_flow_edge(eid, delta);
      }
    }
  }

  static bool reaches_target(int source, int target, const std::vector<Node>& t, int n) {
    for (int u = source, depth = 0; u && depth <= n; u = t[u].rep_parent, ++depth) {
      if (u == target) {
        return true;
      }
    }
    return false;
  }

  static int represented_root(int x, const std::vector<Node>& t, int n) {
    int r = x;
    for (int depth = 0; t[r].rep_parent && depth <= n; ++depth) {
      r = t[r].rep_parent;
    }
    return r;
  }

  int vertex_level(int v) const {
    auto it = level_graph.find(v);
    return it == level_graph.end() ? -1 : it->second.level;
  }

  bool build_level_graph(int source, int target) {
    level_graph.clear();
    level_graph[source] = LevelInfo{0, {}, {}};

    std::queue<int> q;
    q.push(source);
    while (!q.empty()) {
      const int u = q.front();
      q.pop();
      const int lvl = vertex_level(u);
      for (int eid : out_edges[u]) {
        const Edge& e = edges[eid];
        if (e.residual == 0) {
          continue;
        }
        const int v = e.v;
        auto it = level_graph.find(v);
        if (it != level_graph.end()) {
          if (it->second.level == lvl + 1) {
            level_graph[u].outgoing.insert(eid);
            level_graph[v].incoming.insert(eid);
          }
          continue;
        }
        level_graph[v] = LevelInfo{lvl + 1, {}, {}};
        level_graph[v].incoming.insert(eid);
        level_graph[u].outgoing.insert(eid);
        q.push(v);
      }
    }
    return level_graph.count(target) > 0;
  }

  void erase_level_edge(int eid) {
    const Edge& e = edges[eid];
    level_graph.at(e.u).outgoing.erase(eid);
    level_graph.at(e.v).incoming.erase(eid);
  }

  int blocking_flow(int source, int target) {
    reset_lct();
    int pushed = 0;

    while (true) {
      if (reaches_target(source, target, nodes, n)) {
        bool first = true;
        while (true) {
          auto [child, eid] = path_mincost(source);
          if (eid < 0) {
            break;
          }
          int bottleneck = edges[eid].residual;
          if (first) {
            first = false;
            path_augment(source, bottleneck);
            pushed += bottleneck;
            bottleneck = 0;
          }
          if (bottleneck != 0) {
            break;
          }
          lct_cut(child);
          erase_level_edge(eid);
          if (child == source) {
            break;
          }
        }
      } else {
        const int r = represented_root(source, nodes, n);
        LevelInfo& info = level_graph.at(nodes[r].vtx);
        if (info.outgoing.empty()) {
          if (r == source) {
            break;
          }
          std::vector<int> incoming(info.incoming.begin(), info.incoming.end());
          for (int eid : incoming) {
            const Edge& e = edges[eid];
            lct_cut(vtx_node(e.u));
            erase_level_edge(eid);
          }
          info.incoming.clear();
        } else {
          const int eid = *info.outgoing.begin();
          const Edge& e = edges[eid];
          lct_link(r, vtx_node(e.v), eid);
        }
      }
    }
    return pushed;
  }
};

} // namespace maxflow

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Best {
  long long cost;
  int cnt;
};

inline bool better(const Best& a, const Best& b) {
  if (a.cost != b.cost)
    return a.cost < b.cost;
  return a.cnt < b.cnt;
}

inline Best add_best(const Best& a, const Best& b) {
  return Best{a.cost + b.cost, a.cnt + b.cnt};
}

struct Edge {
  int to;
  long long w;
  int add_cnt;
};

const long long INF_COST = (1LL << 62);
const int INF_CNT = (1 << 29);

void dijkstra(int V, const vector<vector<Edge>>& g, vector<Best>& dist) {
  struct Node {
    long long cost;
    int cnt;
    int v;
  };
  struct Cmp {
    bool operator()(const Node& a, const Node& b) const {
      if (a.cost != b.cost)
        return a.cost > b.cost;
      return a.cnt > b.cnt;
    }
  };

  priority_queue<Node, vector<Node>, Cmp> pq;
  for (int v = 0; v < V; v++) {
    if (dist[v].cost < INF_COST)
      pq.push(Node{dist[v].cost, dist[v].cnt, v});
  }

  vector<char> vis(V, 0);
  while (!pq.empty()) {
    Node cur = pq.top();
    pq.pop();
    int v = cur.v;
    if (vis[v])
      continue;
    if (cur.cost != dist[v].cost || cur.cnt != dist[v].cnt)
      continue;
    vis[v] = 1;

    for (const auto& e : g[v]) {
      Best nd{cur.cost + e.w, cur.cnt + e.add_cnt};
      if (better(nd, dist[e.to])) {
        dist[e.to] = nd;
        pq.push(Node{nd.cost, nd.cnt, e.to});
      }
    }
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int N, F, M;
    cin >> N >> F >> M;

    vector<pair<int, int>> edges;
    edges.reserve(M);
    for (int i = 0; i < M; i++) {
      int u, v;
      cin >> u >> v;
      edges.push_back({u, v});
    }

    vector<int> food(N);
    for (int i = 0; i < N; i++)
      cin >> food[i];

    // Node-splitting to convert vertex costs to edge costs.
    // For each original node i:
    //   in(i)  = 2*i
    //   out(i) = 2*i+1
    // Edge in(i)->out(i) has cost food[i] and adds 1 guest (node i).
    // For each undirected friendship (u,v):
    //   out(u)->in(v) and out(v)->in(u) with cost 0.
    //
    // IMPORTANT: We only allow merging DP states at out(i) nodes, so any
    // Steiner/terminal meeting point necessarily pays that vertex cost.
    int V = 2 * N;
    vector<vector<Edge>> g(V);
    g.assign(V, {});
    for (int i = 0; i < N; i++) {
      g[2 * i].push_back(Edge{2 * i + 1, (long long)food[i], 1});
    }
    for (auto [u, v] : edges) {
      g[2 * u + 1].push_back(Edge{2 * v, 0LL, 0});
      g[2 * v + 1].push_back(Edge{2 * u, 0LL, 0});
    }

    int FULL = (1 << F) - 1;
    vector<vector<Best>> dp(1 << F, vector<Best>(V, Best{INF_COST, INF_CNT}));

    for (int i = 0; i < F; i++) {
      int out = 2 * i + 1;
      dp[1 << i][out] = Best{(long long)food[i], 1};
    }

    for (int mask = 1; mask <= FULL; mask++) {
      // Merge subsets at same vertex.
      for (int sub = (mask - 1) & mask; sub; sub = (sub - 1) & mask) {
        int other = mask ^ sub;
        if (sub > other)
          continue; // avoid doing both (sub,other) and (other,sub)
        // Merge only at out-nodes (odd indices), so we cannot "meet"
        // at in(i) without paying food[i].
        for (int v = 1; v < V; v += 2) {
          const Best& a = dp[sub][v];
          const Best& b = dp[other][v];
          if (a.cost >= INF_COST || b.cost >= INF_COST)
            continue;
          int orig = v / 2;
          Best comb{a.cost + b.cost - (long long)food[orig], a.cnt + b.cnt - 1};
          if (better(comb, dp[mask][v]))
            dp[mask][v] = comb;
        }
      }

      // Shortest paths relaxation over the split graph.
      dijkstra(V, g, dp[mask]);
    }

    Best ans{INF_COST, INF_CNT};
    for (int v = 1; v < V; v += 2) {
      if (better(dp[FULL][v], ans))
        ans = dp[FULL][v];
    }

    cout << "Case #" << tc << ": " << ans.cost << " " << ans.cnt << "\n";
  }
  return 0;
}

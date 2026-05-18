#include <bits/stdc++.h>
using namespace std;

class IPv444 {
  struct Edge {
    int to, rev;
    long long cap;
    long long cost;
  };

  vector<vector<Edge>> g;
  vector<long long> pot, dist;
  vector<int> pv, pe;
  int src, snk;

  void addEdge(int u, int v, long long cap, long long cost) {
    Edge a{v, (int)g[v].size(), cap, cost};
    Edge b{u, (int)g[u].size(), 0, -cost};
    g[u].push_back(a);
    g[v].push_back(b);
  }

  bool dijkstra() {
    fill(dist.begin(), dist.end(), (long long)4e18);
    dist[src] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>>
        pq;
    pq.push({0, src});
    while (!pq.empty()) {
      auto [d, v] = pq.top();
      pq.pop();
      if (d != dist[v])
        continue;
      for (int i = 0; i < (int)g[v].size(); i++) {
        const Edge& e = g[v][i];
        if (e.cap <= 0)
          continue;
        long long nd = d + e.cost + pot[v] - pot[e.to];
        if (nd < dist[e.to]) {
          dist[e.to] = nd;
          pv[e.to] = v;
          pe[e.to] = i;
          pq.push({nd, e.to});
        }
      }
    }
    return dist[snk] < (long long)4e18;
  }

  long long minCostMaxFlow(int s, int t, long long maxFlow) {
    src = s;
    snk = t;
    long long flow = 0, cost = 0;
    while (flow < maxFlow && dijkstra()) {
      for (int i = 0; i < (int)pot.size(); i++)
        if (dist[i] < (long long)4e18)
          pot[i] += dist[i];
      long long add = maxFlow - flow;
      for (int v = snk; v != src; v = pv[v]) {
        Edge& e = g[pv[v]][pe[v]];
        add = min(add, e.cap);
      }
      for (int v = snk; v != src; v = pv[v]) {
        Edge& e = g[pv[v]][pe[v]];
        e.cap -= add;
        g[v][pe[v]].cap += add;
      }
      flow += add;
      cost += add * pot[snk];
    }
    return cost;
  }

  struct Node {
    map<int, int> child;
    int star = -1;
    int leaf = -1;
  };

  vector<Node> trie;
  const long long INF = (long long)1e15;

  int newNode() {
    trie.push_back(Node());
    return (int)trie.size() - 1;
  }

  void link(int u, int v, bool star) {
    long long cap = star ? 1000 : INF;
    addEdge(u, v, cap, 0);
  }

  int go(int u, int digit, bool isStar) {
    if (isStar) {
      if (trie[u].star == -1) {
        trie[u].star = newNode();
        link(u, trie[u].star, true);
      }
      return trie[u].star;
    }
    auto it = trie[u].child.find(digit);
    if (it == trie[u].child.end()) {
      int v = newNode();
      trie[u].child[digit] = v;
      link(u, v, false);
      return v;
    }
    return it->second;
  }

  vector<int> parse(const string& req) {
    vector<int> parts;
    int i = 0;
    while (i < (int)req.size()) {
      if (req[i] == '*') {
        parts.push_back(-1);
        i += 2;
        continue;
      }
      int j = i;
      while (j < (int)req.size() && req[j] != '.')
        j++;
      parts.push_back(stoi(req.substr(i, j - i)));
      i = j + 1;
    }
    return parts;
  }

public:
  long long getMaximumMoney(vector<string> request, vector<int> price) {
    int m = request.size();
    trie.clear();
    trie.push_back(Node());

    int S = 0, T = 1;
    g.assign(2, vector<Edge>());
    int nextId = 2;

    long long totalFlow = 0;
    for (int i = 0; i < m; i++) {
      int trader = nextId++;
      g.emplace_back();

      vector<int> parts = parse(request[i]);
      long long ways = 1;
      for (int p : parts)
        if (p == -1)
          ways *= 1000;
      totalFlow += ways;
      addEdge(S, trader, ways, -price[i]);

      int u = 0;
      for (int o = 0; o < 4; o++) {
        int v;
        if (parts[o] == -1)
          v = go(u, 0, true);
        else
          v = go(u, parts[o], false);
        if (o == 0)
          addEdge(trader, v, ways, 0);
        u = v;
      }

      if (trie[u].leaf == -1) {
        trie[u].leaf = nextId++;
        g.emplace_back();
        addEdge(u, T, 1, 0);
      }
    }

    pot.assign(nextId, 0);
    dist.assign(nextId, 0);
    pv.assign(nextId, 0);
    pe.assign(nextId, 0);

    long long flowCost = minCostMaxFlow(S, T, totalFlow);
    return -flowCost;
  }
};

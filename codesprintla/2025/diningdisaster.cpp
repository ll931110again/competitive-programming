// E – Dining Disaster
// https://open.kattis.com/problems/diningdisaster

#include <bits/stdc++.h>
using namespace std;

struct Dinic {
  struct Edge {
    int to, rev, cap;
  };
  int n, s, t;
  vector<vector<Edge>> g;
  vector<int> level, it;
  Dinic(int n_, int s_, int t_) : n(n_), s(s_), t(t_), g(n_) {}
  void addEdge(int fr, int to, int cap) {
    Edge a{to, (int)g[to].size(), cap};
    Edge b{fr, (int)g[fr].size(), 0};
    g[fr].push_back(a);
    g[to].push_back(b);
  }
  bool bfs() {
    level.assign(n, -1);
    queue<int> q;
    level[s] = 0;
    q.push(s);
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (const Edge &e : g[v]) {
        if (e.cap > 0 && level[e.to] < 0) {
          level[e.to] = level[v] + 1;
          q.push(e.to);
        }
      }
    }
    return level[t] >= 0;
  }
  int dfs(int v, int f) {
    if (v == t)
      return f;
    for (int &i = it[v]; i < (int)g[v].size(); ++i) {
      Edge &e = g[v][i];
      if (e.cap > 0 && level[v] < level[e.to]) {
        int ret = dfs(e.to, min(f, e.cap));
        if (ret > 0) {
          e.cap -= ret;
          g[e.to][e.rev].cap += ret;
          return ret;
        }
      }
    }
    return 0;
  }
  int maxflow() {
    int flow = 0, inf = 1e9;
    while (bfs()) {
      it.assign(n, 0);
      int f;
      while ((f = dfs(s, inf)) > 0)
        flow += f;
    }
    return flow;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, K;
  cin >> N >> K;
  const int V = 2 * N;
  auto vid_north = [&](int i) { return i; };
  auto vid_south = [&](int j) { return N + j; };
  auto color = [&](int v) -> int {
    return v < N ? (v % 2) : (1 - ((v - N) % 2));
  };

  vector<vector<int>> adj(V);
  auto add_edge = [&](int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  };
  for (int i = 0; i < N - 1; ++i) {
    add_edge(vid_north(i), vid_north(i + 1));
    add_edge(vid_south(i), vid_south(i + 1));
  }
  for (int i = 0; i < N; ++i)
    add_edge(vid_north(i), vid_south(i));

  vector<char> forced(V, 0);
  for (int i = 0; i < K; ++i) {
    int a, b;
    cin >> a >> b;
    int v = (a == 1) ? vid_north(b - 1) : vid_south(b - 1);
    forced[v] = 1;
  }

  vector<char> blocked(V, 0);
  for (int v = 0; v < V; ++v) {
    if (forced[v])
      blocked[v] = 1;
    else {
      for (int u : adj[v])
        if (forced[u])
          blocked[v] = 1;
    }
  }

  vector<int> Ls, Rs, idOf(V, -1);
  for (int v = 0; v < V; ++v) {
    if (blocked[v])
      continue;
    if (color(v) == 0)
      idOf[v] = (int)Ls.size(), Ls.push_back(v);
    else
      idOf[v] = (int)Rs.size(), Rs.push_back(v);
  }

  int nL = (int)Ls.size(), nR = (int)Rs.size();
  int S = nL + nR, T = S + 1;
  Dinic dc(S + 2, S, T);
  vector<vector<int>> gM(nL);

  for (int u = 0; u < V; ++u) {
    if (blocked[u] || color(u) != 0)
      continue;
    for (int v : adj[u]) {
      if (blocked[v] || color(v) != 1)
        continue;
      int li = idOf[u], rj = idOf[v];
      dc.addEdge(li, nL + rj, 1);
      gM[li].push_back(rj);
    }
  }
  for (int i = 0; i < nL; ++i)
    dc.addEdge(S, i, 1);
  for (int j = 0; j < nR; ++j)
    dc.addEdge(nL + j, T, 1);

  dc.maxflow();

  vector<int> matchL(nL, -1), matchR(nR, -1);
  for (int li = 0; li < nL; ++li) {
    for (const auto &e : dc.g[li]) {
      if (e.to >= nL && e.to < nL + nR && e.cap == 0) {
        int rj = e.to - nL;
        matchL[li] = rj;
        matchR[rj] = li;
        break;
      }
    }
  }

  vector<char> visL(nL), visR(nR);
  queue<int> q;
  for (int u = 0; u < nL; ++u)
    if (matchL[u] == -1) {
      visL[u] = 1;
      q.push(u);
    }
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int rj : gM[u]) {
      if (matchL[u] != rj && !visR[rj]) {
        visR[rj] = 1;
        int u2 = matchR[rj];
        if (u2 != -1 && !visL[u2]) {
          visL[u2] = 1;
          q.push(u2);
        }
      }
    }
  }

  vector<char> take(V, 0);
  for (int v = 0; v < V; ++v)
    if (forced[v])
      take[v] = 1;
  // Max independent set = (L ∩ Z) ∪ (R \ Z), Z = alternating reachability from free L
  for (int i = 0; i < nL; ++i)
    if (visL[i])
      take[Ls[i]] = 1;
  for (int j = 0; j < nR; ++j)
    if (!visR[j])
      take[Rs[j]] = 1;

  int M = 0;
  for (int v = 0; v < V; ++v)
    M += take[v];
  cout << M << '\n';
  for (int i = 0; i < N; ++i)
    cout << (take[vid_north(i)] ? 'X' : '.');
  cout << '\n';
  for (int j = 0; j < N; ++j)
    cout << (take[vid_south(j)] ? 'X' : '.');
  cout << '\n';
  return 0;
}

// E – Dining Disaster
// https://open.kattis.com/problems/diningdisaster

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Dinic {
  struct Edge {
    int to, rev, cap;
  };
  int n, s, t;
  vector<vector<Edge>> g;
  vector<int> level, it;
  Dinic(int n_, int s_, int t_) : n(n_), s(s_), t(t_), g(n_) {}
  void add_edge(int fr, int to, int cap) {
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
      for (const Edge& e : g[v]) {
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
    for (int& i = it[v]; i < (int)g[v].size(); ++i) {
      Edge& e = g[v][i];
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

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, K;
  cin >> N >> K;
  const int V = 2 * N;
  auto vid_north = [&](int i) { return i; };
  auto vid_south = [&](int j) { return N + j; };
  auto color = [&](int v) -> int { return v < N ? (v % 2) : (1 - ((v - N) % 2)); };

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

  vector<int> Ls, Rs, id_of(V, -1);
  for (int v = 0; v < V; ++v) {
    if (blocked[v])
      continue;
    if (color(v) == 0)
      id_of[v] = (int)Ls.size(), Ls.push_back(v);
    else
      id_of[v] = (int)Rs.size(), Rs.push_back(v);
  }

  int n_l = (int)Ls.size(), n_r = (int)Rs.size();
  int S = n_l + n_r, T = S + 1;
  Dinic dc(S + 2, S, T);
  vector<vector<int>> g_m(n_l);

  for (int u = 0; u < V; ++u) {
    if (blocked[u] || color(u) != 0)
      continue;
    for (int v : adj[u]) {
      if (blocked[v] || color(v) != 1)
        continue;
      int li = id_of[u], rj = id_of[v];
      dc.add_edge(li, n_l + rj, 1);
      g_m[li].push_back(rj);
    }
  }
  for (int i = 0; i < n_l; ++i)
    dc.add_edge(S, i, 1);
  for (int j = 0; j < n_r; ++j)
    dc.add_edge(n_l + j, T, 1);

  dc.maxflow();

  vector<int> match_l(n_l, -1), match_r(n_r, -1);
  for (int li = 0; li < n_l; ++li) {
    for (const auto& e : dc.g[li]) {
      if (e.to >= n_l && e.to < n_l + n_r && e.cap == 0) {
        int rj = e.to - n_l;
        match_l[li] = rj;
        match_r[rj] = li;
        break;
      }
    }
  }

  vector<char> vis_l(n_l), vis_r(n_r);
  queue<int> q;
  for (int u = 0; u < n_l; ++u)
    if (match_l[u] == -1) {
      vis_l[u] = 1;
      q.push(u);
    }
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int rj : g_m[u]) {
      if (match_l[u] != rj && !vis_r[rj]) {
        vis_r[rj] = 1;
        int u2 = match_r[rj];
        if (u2 != -1 && !vis_l[u2]) {
          vis_l[u2] = 1;
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
  for (int i = 0; i < n_l; ++i)
    if (vis_l[i])
      take[Ls[i]] = 1;
  for (int j = 0; j < n_r; ++j)
    if (!vis_r[j])
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

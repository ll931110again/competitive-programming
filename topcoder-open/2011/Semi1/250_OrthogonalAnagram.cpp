#include <bits/stdc++.h>
using namespace std;

class OrthogonalAnagram {
  string S;
  int n;
  vector<int> cnt;

  struct Dinic {
    struct Edge {
      int to, rev;
      int cap;
    };
    int N;
    vector<vector<Edge>> g;
    vector<int> level, it;

    Dinic(int n) : N(n), g(n), level(n), it(n) {}

    void addEdge(int fr, int to, int cap) {
      Edge a{to, (int)g[to].size(), cap};
      Edge b{fr, (int)g[fr].size(), 0};
      g[fr].push_back(a);
      g[to].push_back(b);
    }

    bool bfs(int s, int t) {
      fill(level.begin(), level.end(), -1);
      queue<int> q;
      level[s] = 0;
      q.push(s);
      while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (const Edge& e : g[v]) {
          if (e.cap > 0 && level[e.to] == -1) {
            level[e.to] = level[v] + 1;
            q.push(e.to);
          }
        }
      }
      return level[t] != -1;
    }

    int dfs(int v, int t, int f) {
      if (v == t)
        return f;
      for (int& i = it[v]; i < (int)g[v].size(); i++) {
        Edge& e = g[v][i];
        if (e.cap > 0 && level[e.to] == level[v] + 1) {
          int ret = dfs(e.to, t, min(f, e.cap));
          if (ret > 0) {
            e.cap -= ret;
            g[e.to][e.rev].cap += ret;
            return ret;
          }
        }
      }
      return 0;
    }

    int maxflow(int s, int t) {
      int flow = 0;
      while (bfs(s, t)) {
        fill(it.begin(), it.end(), 0);
        while (true) {
          int pushed = dfs(s, t, 1 << 30);
          if (!pushed)
            break;
          flow += pushed;
        }
      }
      return flow;
    }
  };

  bool feasible(int pos, const vector<int>& rem) {
    int L = 26;
    int src = 0, sink = 1;
    int base = 2;
    Dinic dinic(base + L + (n - pos) + 2);
    int S = src, T = sink;
    int letterBase = base;
    int posBase = base + L;

    for (int c = 0; c < L; c++)
      if (rem[c] > 0)
        dinic.addEdge(S, letterBase + c, rem[c]);

    for (int i = pos; i < n; i++) {
      int pid = posBase + (i - pos);
      dinic.addEdge(pid, T, 1);
      for (int c = 0; c < L; c++)
        if (rem[c] > 0 && S[i] - 'a' != c)
          dinic.addEdge(letterBase + c, pid, 1);
    }

    return dinic.maxflow(S, T) == n - pos;
  }

public:
  string solve(string S_) {
    S = S_;
    n = (int)S.size();
    cnt.assign(26, 0);
    for (char c : S)
      cnt[c - 'a']++;

    string ans;
    vector<int> rem = cnt;
    for (int i = 0; i < n; i++) {
      bool ok = false;
      for (int c = 0; c < 26; c++) {
        if (rem[c] == 0 || c == S[i] - 'a')
          continue;
        rem[c]--;
        if (feasible(i + 1, rem)) {
          ans.push_back(char('a' + c));
          ok = true;
          break;
        }
        rem[c]++;
      }
      if (!ok)
        return "";
    }
    return ans;
  }
};

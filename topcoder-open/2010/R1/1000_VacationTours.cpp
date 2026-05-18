#include <bits/stdc++.h>
using namespace std;

class VacationTours {
public:
  int getIncome(vector<string> c, vector<string> d, int fee) {
    int N = (int)c.size();
    int n = N - 1;
    if (n == 0)
      return 0;

    vector<vector<int>> cost(N, vector<int>(N));
    for (int i = 0; i < (N); ++i)
      for (int j = 0; j < (N); ++j)
        cost[i][j] = decode(c[i][j]) * 64 + decode(d[i][j]);

    const int S = 0, T = 1, V = 2 + 2 * n;
    MinCostFlow mcmf(V);

    for (int i = 0; i < (n); ++i) {
      int in = 2 + 2 * i, out = in + 1;
      mcmf.addEdge(S, in, 1, cost[0][i + 1] - fee);
      mcmf.addEdge(out, T, 1, cost[i + 1][0]);
      mcmf.addEdge(in, out, 1, 0);
      for (int j = 0; j < (n); ++j)
        if (i != j)
          mcmf.addEdge(out, 2 + 2 * j, 1, cost[i + 1][j + 1]);
    }

    long long totalCost = 0;
    while (true) {
      int pathCost = mcmf.augment(S, T);
      if (pathCost >= 0)
        break;
      totalCost += pathCost;
    }
    return (int)max(0LL, -totalCost);
  }

private:
  int decode(char ch) {
    if ('A' <= ch && ch <= 'Z')
      return ch - 'A';
    if ('a' <= ch && ch <= 'z')
      return 26 + ch - 'a';
    if ('0' <= ch && ch <= '9')
      return 52 + ch - '0';
    if (ch == '+')
      return 62;
    return 63;
  }

  struct MinCostFlow {
    struct Edge {
      int to, rev, cap, cost;
    };

    vector<vector<Edge>> g;
    vector<int> dist, parentV, parentE;
    vector<char> inq;

    MinCostFlow(int n) : g(n), dist(n), parentV(n), parentE(n), inq(n) {}

    void addEdge(int from, int to, int cap, int cost) {
      Edge a = {to, (int)g[to].size(), cap, cost};
      Edge b = {from, (int)g[from].size(), 0, -cost};
      g[from].push_back(a);
      g[to].push_back(b);
    }

    bool spfa(int s, int t) {
      const int INF = 1e9;
      fill(dist.begin(), dist.end(), INF);
      fill(inq.begin(), inq.end(), 0);
      dist[s] = 0;
      deque<int> q;
      q.push_back(s);
      inq[s] = 1;
      while (!q.empty()) {
        int u = q.front();
        q.pop_front();
        inq[u] = 0;
        for (int i = 0; i < ((int); ++i)g[u].size()) {
          Edge& e = g[u][i];
          if (e.cap > 0 && dist[u] + e.cost < dist[e.to]) {
            dist[e.to] = dist[u] + e.cost;
            parentV[e.to] = u;
            parentE[e.to] = i;
            if (!inq[e.to]) {
              q.push_back(e.to);
              inq[e.to] = 1;
            }
          }
        }
      }
      return dist[t] < INF;
    }

    int augment(int s, int t) {
      if (!spfa(s, t) || dist[t] >= 0)
        return 0;
      int v = t;
      while (v != s) {
        Edge& e = g[parentV[v]][parentE[v]];
        e.cap--;
        g[v][e.rev].cap++;
        v = parentV[v];
      }
      return dist[t];
    }
  };
};

#ifdef LOCAL_TEST
#include <cassert>
int main() {
  auto run = [](vector<string> c, vector<string> d, int fee, int expected) {
    int got = VacationTours().getIncome(c, d, fee);
    if (got != expected) {
      cerr << "FAIL fee=" << fee << " n=" << c.size() << ": got " << got << " expected " << expected
           << endl;
      exit(1);
    }
  };

  run({"AAA", "AAA", "AAA"}, {"ABJ", "JAB", "BJA"}, 15, 12);
  run({"AAAA", "AAAA", "AAAA", "AAAA"}, {"AAAA", "AAAA", "AAAA", "AAAA"}, 100, 300);
  run({"A//", "/A/", "//A"}, {"A//", "/A/", "//A"}, 1000, 0);
  run({"AAA////", "/AA/A//", "//AA/A/", "A//AA//", "///AAA/", "///A/AA", "AA////A"},
      {"AKo////", "/AU/X//", "//AZ/o/", "j//AK//", "///XAo/", "///y/AK", "KP////A"}, 1000, 1809);
  run({"AA", "AA"}, {"AA", "AA"}, 1, 1);
  run({"A1Dsp", "TAs/2", "ASA8/", "T0CAo", "//kNA"}, {"Ae2MI", "3Ae/q", "bcA7/", "lIiAA", "//qHA"},
      4500, 4646);
  run({"AAA", "AA8", "AAA"}, {"ADD", "BAz", "CBA"}, 9, 9);
  run({"AAAAAA", "AAnApi", "AvA1q8", "AgAAmg", "Ao88AA", "AAhzjA"},
      {"AGGGBG", "EAMBSv", "BlA49M", "DrBALB", "G5N9AB", "FBOBAA"}, 16, 36);
  run({"AAAAAA", "AA5gAq", "A3A5nA", "AArAs3", "AkxiA/", "ApzA6A"},
      {"AFBEGD", "DA3DBX", "GtAP5B", "EB9AKe", "BVjwAf", "F/GBWA"}, 16, 42);

  cerr << "all tests passed" << endl;
}
#endif

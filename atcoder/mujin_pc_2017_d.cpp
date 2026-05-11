// MUJIN Programming Challenge 2017 - D: Oriented Tree
// https://atcoder.jp/contests/mujin-pc-2017/tasks/mujin_pc_2017_d
//
// Statement omits the definition of D in English; by editorial:
//   D = max_{s,t} max(d(s,t), d(t,s))
// Orientations correspond bijectively to integer labels h with a fixed gauge: along a
// directed edge u→v we have h(u) − h(v) = 1. Then
//   max(d(s,t),d(t,s)) = (dist(s,t) + |h(s)-h(t)|) / 2,
// minimum D = ceil(diam/2). Count valid h with gauge-fixing per editorial.

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

static const int MOD = int(1e9 + 7);

struct BFSResult {
  vector<int> dist;
  vector<int> parent;
  int far{};
};

static BFSResult bfs(const vector<vector<int>> &adj, int src, int n) {
  BFSResult res;
  res.dist.assign(n + 1, -1);
  res.parent.assign(n + 1, -1);
  queue<int> q;
  res.dist[src] = 0;
  q.push(src);
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (int to : adj[v]) {
      if (res.dist[to] != -1)
        continue;
      res.dist[to] = res.dist[v] + 1;
      res.parent[to] = v;
      q.push(to);
    }
  }
  res.far = 1;
  for (int i = 2; i <= n; ++i)
    if (res.dist[i] > res.dist[res.far])
      res.far = i;
  return res;
}

// Second BFS started from u; parent[] walks toward u. Walking v→u gives u … v.
static vector<int> diameter_path_from_parent(int v, const vector<int> &parent) {
  vector<int> path;
  for (int x = v; x != -1; x = parent[x])
    path.push_back(x);
  reverse(path.begin(), path.end());
  return path;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  if (!(cin >> N))
    return 0;
  vector<vector<int>> adj(N + 1);
  for (int i = 0; i < N - 1; ++i) {
    int a, b;
    cin >> a >> b;
    adj[a].push_back(b);
    adj[b].push_back(a);
  }

  auto br = bfs(adj, 1, N);
  int u = br.far;
  auto br2 = bfs(adj, u, N);
  int v = br2.far;
  int diam = br2.dist[v];
  vector<int> path = diameter_path_from_parent(v, br2.parent);

  long long answer = 0;

  auto combine_children = [&](int v, int p, int hv,
                               const function<long long(int, int, int)> &dfs) -> long long {
    long long prod = 1;
    for (int c : adj[v]) {
      if (c == p)
        continue;
      long long sm = (dfs(c, v, hv - 1) + dfs(c, v, hv + 1)) % MOD;
      prod = prod * sm % MOD;
    }
    return prod;
  };

  if (diam % 2 == 0) {
    int r = path[diam / 2];
    auto dd = bfs(adj, r, N);
    const vector<int> &dist_r = dd.dist;
    int half = diam / 2;

    vector<vector<long long>> memo(N + 1, vector<long long>(2 * N + 5, -1));

    function<long long(int, int, int)> dfs = [&](int v, int p, int hv) -> long long {
      int idx = hv + N + 2;
      long long &mem = memo[v][idx];
      if (mem != -1)
        return mem;

      int B = half - dist_r[v];
      if (abs(hv) > B || (dist_r[v] == half && hv != 0))
        return mem = 0;

      return mem = combine_children(v, p, hv, dfs);
    };

    for (int hv = -N; hv <= N; ++hv)
      answer = (answer + dfs(r, -1, hv)) % MOD;
  } else {
    int s = path[diam / 2];
    int t = path[diam / 2 + 1];
    auto ds = bfs(adj, s, N);
    auto dt = bfs(adj, t, N);
    const vector<int> &d_s = ds.dist;
    const vector<int> &d_t = dt.dist;

    vector<int> ec(N + 1);
    vector<char> black(N + 1);
    for (int i = 1; i <= N; ++i) {
      ec[i] = min(d_s[i], d_t[i]);
      black[i] = (d_s[i] < d_t[i]);
    }

    int half = diam / 2; // floor

    auto run_case = [&](bool swap_bw, bool intersection) -> long long {
      vector<vector<long long>> memo(N + 1, vector<long long>(2 * N + 5, -1));

      auto ok = [&](int vertex, int hv) -> bool {
        int klim = half - ec[vertex];
        if (!intersection) {
          // Case 1: black tight, white +1. Case 2: swapped.
          int extra = swap_bw ? (black[vertex] ? 1 : 0) : (black[vertex] ? 0 : 1);
          int cap = klim + extra;
          return abs(hv) <= cap;
        }
        // Intersection: white |h| ≤ klim; black |h+1| ≤ klim
        if (!black[vertex])
          return abs(hv) <= klim;
        return abs(hv + 1) <= klim;
      };

      function<long long(int, int, int)> dfs = [&](int v, int p, int hv) -> long long {
        int idx = hv + N + 2;
        long long &mem = memo[v][idx];
        if (mem != -1)
          return mem;
        if (!ok(v, hv))
          return mem = 0;
        return mem = combine_children(v, p, hv, dfs);
      };

      long long sum = 0;
      for (int hv = -N; hv <= N; ++hv)
        sum = (sum + dfs(s, -1, hv)) % MOD;
      return sum;
    };

    // Case 1 / case 2 count valid integer labelings h under two symmetric bounds.
    // Their union (as orientations) satisfies |O1 ∪ O2| = L1 + L2 − 2·L12 where L12
    // is the number of labelings satisfying *both* bound systems (intersection).
    long long L1 = run_case(false, false);
    long long L2 = run_case(true, false);
    long long L12 = run_case(false, true);
    answer = (L1 + L2 - (2 * L12) % MOD + MOD) % MOD;
  }

  cout << answer << '\n';
  return 0;
}

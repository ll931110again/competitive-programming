// Local verifier for AHC017 A (matches official Rust scorer).

#include <bits/stdc++.h>
#include <fstream>
using namespace std;

constexpr int INF = 1'000'000'000;

int main(int argc, char **argv) {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ifstream fin;
  if (argc >= 2)
    fin.open(argv[1]);
  istream &in = fin.is_open() ? static_cast<istream &>(fin) : cin;

  int N, M, D, K;
  in >> N >> M >> D >> K;
  vector<array<int, 3>> edges(M);
  for (int i = 0; i < M; ++i) {
    in >> edges[i][0] >> edges[i][1] >> edges[i][2];
    --edges[i][0];
    --edges[i][1];
  }
  for (int i = 0; i < N; ++i) {
    int x, y;
    in >> x >> y;
  }

  vector<int> day(M);
  ifstream fout;
  if (argc >= 3)
    fout.open(argv[2]);
  istream &out_in = fout.is_open() ? static_cast<istream &>(fout) : cin;
  for (int i = 0; i < M; ++i)
    out_in >> day[i];

  vector<int> count(D + 1);
  for (int d : day) {
    if (d < 1 || d > D) {
      cout << "invalid day\n";
      return 1;
    }
    ++count[d];
  }
  for (int k = 1; k <= D; ++k) {
    if (count[k] > K) {
      cout << "day " << k << " exceeds K (" << count[k] << " > " << K << ")\n";
      return 1;
    }
  }

  auto graph = [&](int closed_day) {
    vector<vector<pair<int, int>>> g(N);
    for (int i = 0; i < M; ++i) {
      if (day[i] == closed_day)
        continue;
      const int u = edges[i][0], v = edges[i][1], w = edges[i][2];
      g[u].emplace_back(v, w);
      g[v].emplace_back(u, w);
    }
    return g;
  };

  auto dist_from = [&](const vector<vector<pair<int, int>>> &g, int s) {
    vector<int> dist(N, INF);
    priority_queue<pair<int, int>> pq;
    dist[s] = 0;
    pq.emplace(0, s);
    while (!pq.empty()) {
      auto [d, u] = pq.top();
      pq.pop();
      d = -d;
      if (dist[u] != d)
        continue;
      for (const auto &[v, w] : g[u]) {
        const int nd = d + w;
        if (nd < dist[v]) {
          dist[v] = nd;
          pq.emplace(-nd, v);
        }
      }
    }
    return dist;
  };

  auto dist_matrix = [&](int closed_day) {
    const auto g = graph(closed_day);
    vector<vector<int>> mat(N);
    for (int s = 0; s < N; ++s)
      mat[s] = dist_from(g, s);
    return mat;
  };

  const auto base = dist_matrix(0);
  long long num = 0;
  for (int k = 1; k <= D; ++k) {
    const auto cur = dist_matrix(k);
    for (int i = 0; i < N; ++i) {
      for (int j = i + 1; j < N; ++j)
        num += cur[i][j] - base[i][j];
    }
  }
  const long long den = 1LL * D * N * (N - 1) / 2;
  const long long score = llround(num * 1000.0 / den);
  cout << "frustration=" << score << '\n';
  return 0;
}

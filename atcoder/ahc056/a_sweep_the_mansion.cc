// AHC056 A - Sweep the Mansion
// https://atcoder.jp/contests/ahc056/tasks/ahc056_a
//
// Encode cell id as color; BFS shortest-path transitions between checkpoints.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int kDr[4] = {-1, 1, 0, 0};
constexpr int kDc[4] = {0, 0, -1, 1};
constexpr char kCmd[4] = {'U', 'D', 'L', 'R'};

int N, K, T;
vector<vector<bool>> wall_v, wall_h;
vector<pair<int, int>> xy;

int cell_id(int r, int c) { return r * N + c; }

bool can_move(int r, int c, int d) {
  const int nr = r + kDr[d], nc = c + kDc[d];
  if (nr < 0 || nr >= N || nc < 0 || nc >= N)
    return false;
  if (kDr[d] == 0)
    return !wall_v[r][min(c, nc)];
  return !wall_h[min(r, nr)][c];
}

vector<pair<int, int>> bfs_path(pair<int, int> src, pair<int, int> dst) {
  vector<vector<pair<int, int>>> par(N, vector<pair<int, int>>(N, {-1, -1}));
  queue<pair<int, int>> q;
  q.push(src);
  par[src.first][src.second] = src;
  while (!q.empty()) {
    const auto [r, c] = q.front();
    q.pop();
    if (make_pair(r, c) == dst)
      break;
    for (int d = 0; d < 4; ++d) {
      if (!can_move(r, c, d))
        continue;
      const int nr = r + kDr[d], nc = c + kDc[d];
      if (par[nr][nc].first != -1)
        continue;
      par[nr][nc] = {r, c};
      q.push({nr, nc});
    }
  }
  if (par[dst.first][dst.second].first == -1)
    return {};
  vector<pair<int, int>> path;
  for (auto p = dst; p != src; p = par[p.first][p.second])
    path.push_back(p);
  path.push_back(src);
  reverse(path.begin(), path.end());
  return path;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> K >> T;
  wall_v.assign(N, vector<bool>(N - 1));
  wall_h.assign(N - 1, vector<bool>(N));
  for (int i = 0; i < N; ++i) {
    string s;
    cin >> s;
    for (int j = 0; j < N - 1; ++j)
      wall_v[i][j] = s[j] == '1';
  }
  for (int i = 0; i < N - 1; ++i) {
    string s;
    cin >> s;
    for (int j = 0; j < N; ++j)
      wall_h[i][j] = s[j] == '1';
  }
  xy.resize(K);
  for (int i = 0; i < K; ++i)
    cin >> xy[i].first >> xy[i].second;

  const int C = N * N;
  const int Q = 1;
  vector<array<int, 5>> rules;
  for (int seg = 0; seg + 1 < K; ++seg) {
    const auto path = bfs_path(xy[seg], xy[seg + 1]);
    for (size_t t = 0; t + 1 < path.size(); ++t) {
      const auto [r, c] = path[t];
      const auto [nr, nc] = path[t + 1];
      int d = -1;
      for (int dd = 0; dd < 4; ++dd) {
        if (r + kDr[dd] == nr && c + kDc[dd] == nc)
          d = dd;
      }
      rules.push_back({cell_id(r, c), 0, cell_id(nr, nc), 0, d});
    }
  }

  cout << C << ' ' << Q << ' ' << rules.size() << '\n';
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (j)
        cout << ' ';
      cout << cell_id(i, j);
    }
    cout << '\n';
  }
  for (const auto& rule : rules)
    cout << rule[0] << ' ' << rule[1] << ' ' << rule[2] << ' ' << rule[3] << ' '
         << kCmd[rule[4]] << '\n';
  return 0;
}

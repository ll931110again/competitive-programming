// AHC054 A - Treant's Forest
// https://atcoder.jp/contests/ahc054/tasks/ahc054_a
//
// Interactive: block shortest-path edges on the confirmed map (avoid entrance/goal/known).

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int N, ti, tj;
vector<string> grid;
vector<vector<char>> known, blocked;
int si, sj;

int id(int r, int c) { return r * N + c; }

void bfs_from(int sr, int sc, vector<int>& dist, const vector<vector<char>>& walk) {
  dist.assign(N * N, -1);
  queue<int> q;
  dist[id(sr, sc)] = 0;
  q.push(id(sr, sc));
  while (!q.empty()) {
    const int u = q.front();
    q.pop();
    const int r = u / N, c = u % N;
    constexpr int dr[4] = {-1, 1, 0, 0};
    constexpr int dc[4] = {0, 0, -1, 1};
    for (int d = 0; d < 4; ++d) {
      const int nr = r + dr[d], nc = c + dc[d];
      if (nr < 0 || nr >= N || nc < 0 || nc >= N)
        continue;
      if (!walk[nr][nc] || blocked[nr][nc])
        continue;
      const int v = id(nr, nc);
      if (dist[v] != -1)
        continue;
      dist[v] = dist[u] + 1;
      q.push(v);
    }
  }
}

vector<pair<int, int>> shortest_path(const vector<vector<char>>& walk) {
  vector<int> dist, par(N * N, -1);
  bfs_from(si, sj, dist, walk);
  const int goal = id(ti, tj);
  if (dist[goal] == -1)
    return {};
  vector<pair<int, int>> path;
  for (int v = goal; v != -1; v = par[v]) {
    path.push_back({v / N, v % N});
    if (v == id(si, sj))
      break;
  }
  reverse(path.begin(), path.end());
  return path;
}

vector<pair<int, int>> rebuild_path(const vector<vector<char>>& walk) {
  vector<int> dist(N * N, -1), par(N * N, -1);
  queue<int> q;
  dist[id(si, sj)] = 0;
  q.push(id(si, sj));
  while (!q.empty()) {
    const int u = q.front();
    q.pop();
    const int r = u / N, c = u % N;
    constexpr int dr[4] = {-1, 1, 0, 0};
    constexpr int dc[4] = {0, 0, -1, 1};
    for (int d = 0; d < 4; ++d) {
      const int nr = r + dr[d], nc = c + dc[d];
      if (nr < 0 || nr >= N || nc < 0 || nc >= N)
        continue;
      if (!walk[nr][nc] || blocked[nr][nc])
        continue;
      const int v = id(nr, nc);
      if (dist[v] != -1)
        continue;
      dist[v] = dist[u] + 1;
      par[v] = u;
      q.push(v);
    }
  }
  const int goal = id(ti, tj);
  if (dist[goal] == -1)
    return {};
  vector<pair<int, int>> path;
  for (int v = goal; ; v = par[v]) {
    path.push_back({v / N, v % N});
    if (v == id(si, sj))
      break;
  }
  reverse(path.begin(), path.end());
  return path;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> ti >> tj;
  grid.resize(N);
  for (int i = 0; i < N; ++i)
    cin >> grid[i];

  si = 0;
  sj = N / 2;
  known.assign(N, vector<char>(N, 0));
  blocked.assign(N, vector<char>(N, 0));
  known[si][sj] = 1;

  while (true) {
    int pi, pj, n;
    cin >> pi >> pj >> n;
    if (pi == ti && pj == tj)
      break;

    for (int k = 0; k < n; ++k) {
      int x, y;
      cin >> x >> y;
      known[x][y] = 1;
      if (grid[x][y] == 'T')
        blocked[x][y] = 1;
    }

    vector<vector<char>> walk(N, vector<char>(N, 0));
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        if (!known[i][j]) {
          walk[i][j] = 1;
        } else {
          walk[i][j] = (grid[i][j] == '.' && !blocked[i][j]);
        }
      }
    }
    walk[si][sj] = 1;
    walk[ti][tj] = 1;

    auto path = rebuild_path(walk);
    vector<pair<int, int>> place;
    if (path.size() >= 2) {
      const auto [r, c] = path[1];
      if (!known[r][c] && (r != si || c != sj) && (r != ti || c != tj))
        place.emplace_back(r, c);
    }

    cout << place.size();
    for (const auto [r, c] : place)
      cout << ' ' << r << ' ' << c;
    cout << '\n' << flush;
  }
  return 0;
}

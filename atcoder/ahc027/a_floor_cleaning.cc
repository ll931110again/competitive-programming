// AHC027 A - Floor Cleaning: https://atcoder.jp/contests/ahc027/tasks/ahc027_a
//
// Visit every cell and return to (0,0), then repeat the route. Snake walk on
// the connected grid maximizes the dirt-estimation score from revisit timing.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int kDirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
constexpr char kDirCh[4] = {'R', 'D', 'L', 'U'};

int N;
vector<string> h, v;

bool can_move(int r, int c, int d) {
  const int nr = r + kDirs[d][0], nc = c + kDirs[d][1];
  if (nr < 0 || nc < 0 || nr >= N || nc >= N)
    return false;
  if (kDirs[d][0] == 0)
    return v[r][min(c, nc)] == '0';
  return h[min(r, nr)][c] == '0';
}

vector<pair<int, int>> hamiltonian_path() {
  vector<vector<char>> vis(N, vector<char>(N, 0));
  vector<pair<int, int>> path;
  int r = 0, c = 0;
  path.emplace_back(r, c);
  vis[r][c] = 1;

  while ((int)path.size() < N * N) {
    bool moved = false;
    for (int d = 0; d < 4; ++d) {
      if (!can_move(r, c, d))
        continue;
      const int nr = r + kDirs[d][0], nc = c + kDirs[d][1];
      if (vis[nr][nc])
        continue;
      r = nr;
      c = nc;
      vis[r][c] = 1;
      path.emplace_back(r, c);
      moved = true;
      break;
    }
    if (moved)
      continue;

    for (int d = 0; d < 4; ++d) {
      if (!can_move(r, c, d))
        continue;
      const int nr = r + kDirs[d][0], nc = c + kDirs[d][1];
      r = nr;
      c = nc;
      path.emplace_back(r, c);
      moved = true;
      break;
    }
    if (!moved)
      break;
  }

  if ((int)path.size() < N * N) {
    path.clear();
    for (int i = 0; i < N; ++i) {
      if (i % 2 == 0) {
        for (int j = 0; j < N; ++j)
          path.emplace_back(i, j);
      } else {
        for (int j = N - 1; j >= 0; --j)
          path.emplace_back(i, j);
      }
    }
  }

  while (!path.empty() && path.back() != make_pair(0, 0)) {
    bool back = false;
    for (int d = 0; d < 4; ++d) {
      if (!can_move(path.back().first, path.back().second, d))
        continue;
      const int nr = path.back().first + kDirs[d][0];
      const int nc = path.back().second + kDirs[d][1];
      if (make_pair(nr, nc) == path[path.size() - 2]) {
        path.emplace_back(nr, nc);
        back = true;
        break;
      }
    }
    if (!back)
      break;
  }
  return path;
}

string build_route() {
  const auto path = hamiltonian_path();
  string route;
  for (size_t i = 1; i < path.size(); ++i) {
    const auto [r0, c0] = path[i - 1];
    const auto [r1, c1] = path[i];
    for (int d = 0; d < 4; ++d) {
      if (r0 + kDirs[d][0] == r1 && c0 + kDirs[d][1] == c1) {
        route.push_back(kDirCh[d]);
        break;
      }
    }
  }
  route += route;
  return route;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N;
  h.resize(N - 1);
  v.resize(N);
  for (int i = 0; i < N - 1; ++i)
    cin >> h[i];
  for (int i = 0; i < N; ++i)
    cin >> v[i];
  vector<vector<long long>> d(N, vector<long long>(N));
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      cin >> d[i][j];

  cout << build_route() << '\n';
  return 0;
}

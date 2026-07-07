// AHC024 A - Recoloring: https://atcoder.jp/contests/ahc024/tasks/ahc024_a
//
// Preserve color-adjacency graph while maximizing white (0) cells. Flood-fill
// each color region to 0 when no adjacency constraint forbids it; emit one
// grid per recolor step.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int kDirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

int n, m;
vector<vector<int>> grid;
vector<vector<int>> adj;

void build_adj(const vector<vector<int>>& cs) {
  adj.assign(m + 1, vector<int>(m + 1, 0));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      const int c = cs[i][j];
      if (c != 0 && (i == 0 || j == 0 || i + 1 == n || j + 1 == n))
        adj[c][0] = adj[0][c] = 1;
      if (i + 1 < n && cs[i][j] != cs[i + 1][j]) {
        adj[cs[i][j]][cs[i + 1][j]] = adj[cs[i + 1][j]][cs[i][j]] = 1;
      }
      if (j + 1 < n && cs[i][j] != cs[i][j + 1]) {
        adj[cs[i][j]][cs[i][j + 1]] = adj[cs[i][j + 1]][cs[i][j]] = 1;
      }
    }
  }
}

bool can_recolor(int color) {
  if (color == 0)
    return false;
  for (int c = 0; c <= m; ++c) {
    if (c == color)
      continue;
    const bool was = adj[color][c];
    const bool now = (c == 0) ? false : was;
    if (was != now)
      return false;
  }
  return true;
}

void print_grid() {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (j)
        cout << ' ';
      cout << grid[i][j];
    }
    cout << '\n';
  }
}

void solve() {
  build_adj(grid);
  vector<pair<int, int>> order;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      if (grid[i][j] > 0)
        order.emplace_back(i, j);
  sort(order.begin(), order.end(), [](auto a, auto b) {
    return grid[a.first][a.second] > grid[b.first][b.second];
  });

  for (auto [i, j] : order) {
    const int c = grid[i][j];
    if (c == 0 || !can_recolor(c))
      continue;
    vector<vector<int>> nxt = grid;
    vector<vector<char>> vis(n, vector<char>(n, 0));
    queue<pair<int, int>> q;
    q.emplace(i, j);
    vis[i][j] = 1;
    while (!q.empty()) {
      auto [x, y] = q.front();
      q.pop();
      nxt[x][y] = 0;
      for (auto [dx, dy] : kDirs) {
        const int nx = x + dx, ny = y + dy;
        if (nx < 0 || ny < 0 || nx >= n || ny >= n || vis[nx][ny])
          continue;
        if (grid[nx][ny] == c) {
          vis[nx][ny] = 1;
          q.emplace(nx, ny);
        }
      }
    }
    grid.swap(nxt);
    build_adj(grid);
    print_grid();
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  grid.assign(n, vector<int>(n));
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      cin >> grid[i][j];

  solve();
  return 0;
}

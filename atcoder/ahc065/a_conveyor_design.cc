// AHC065 A - Conveyor Design: https://atcoder.jp/contests/ahc065/tasks/ahc065_a
//
// Hamiltonian-path belts (length-2 loops on consecutive path cells, <=2 belts/cell).
// Target ~1.14e9 total needs the 1st-place "sushi lane" (long tour cycle + swaps);
// see https://qiita.com/tanaka-a/items/c286e1d03fd7d7b419ac

#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <algorithm>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>
#endif
using namespace std;

struct Belt {
  int r0, c0, r1, c1;
};

static int N, EXIT_C, NN, EXIT_IDX;
static vector<vector<int>> grid;
static vector<pair<int, int>> pos;
static vector<Belt> belts;
static vector<vector<int>> pidx;
static vector<pair<int, int>> path;
static vector<pair<int, int>> ops;

static int at_cell(int r, int c) { return grid[r][c]; }

static void apply_rotate(int m, int d) {
  const auto &b = belts[m];
  const int v0 = grid[b.r0][b.c0], v1 = grid[b.r1][b.c1];
  grid[b.r0][b.c0] = v1;
  grid[b.r1][b.c1] = v0;
  if (v0 >= 0) pos[v0] = {b.r1, b.c1};
  if (v1 >= 0) pos[v1] = {b.r0, b.c0};
}

static void try_remove(int &cur) {
  if (grid[0][EXIT_C] != cur) return;
  grid[0][EXIT_C] = -1;
  pos[cur] = {-1, -1};
  cur++;
}

static bool dfs_path(int r, int c, vector<vector<char>> &vis) {
  path.emplace_back(r, c);
  vis[r][c] = 1;
  if ((int)path.size() == NN) return true;
  static constexpr int dr[4] = {-1, 1, 0, 0};
  static constexpr int dc[4] = {0, 0, -1, 1};
  vector<tuple<int, int, int>> cand;
  for (int t = 0; t < 4; t++) {
    const int nr = r + dr[t], nc = c + dc[t];
    if (nr < 0 || nr >= N || nc < 0 || nc >= N || vis[nr][nc]) continue;
    int deg = 0;
    for (int u = 0; u < 4; u++) {
      const int ar = nr + dr[u], ac = nc + dc[u];
      if (ar >= 0 && ar < N && ac >= 0 && ac < N && !vis[ar][ac]) deg++;
    }
    cand.emplace_back(deg, nr, nc);
  }
  sort(cand.begin(), cand.end());
  for (const auto &e : cand) {
    if (dfs_path(get<1>(e), get<2>(e), vis)) return true;
  }
  vis[r][c] = 0;
  path.pop_back();
  return false;
}

static bool build_path_warn(int seed) {
  vector<vector<char>> vis(N, vector<char>(N, 0));
  static constexpr int dr[4] = {-1, 1, 0, 0};
  static constexpr int dc[4] = {0, 0, -1, 1};
  path.clear();
  int r = 0, c = EXIT_C;
  for (int step = 0; step < NN; step++) {
    path.emplace_back(r, c);
    vis[r][c] = 1;
    if (step + 1 == NN) break;
    vector<tuple<int, int, int>> cand;
    for (int t = 0; t < 4; t++) {
      const int nr = r + dr[t], nc = c + dc[t];
      if (nr < 0 || nr >= N || nc < 0 || nc >= N || vis[nr][nc]) continue;
      int deg = 0;
      for (int u = 0; u < 4; u++) {
        const int ar = nr + dr[u], ac = nc + dc[u];
        if (ar >= 0 && ar < N && ac >= 0 && ac < N && !vis[ar][ac]) deg++;
      }
      cand.emplace_back(deg, nr, nc);
    }
    if (cand.empty()) return false;
    sort(cand.begin(), cand.end());
    const int pick = (seed + step) % (int)cand.size();
    tie(r, c) = pair<int, int>{get<1>(cand[pick]), get<2>(cand[pick])};
  }
  reverse(path.begin(), path.end());
  return (int)path.size() == NN;
}

static long long estimate_cost() {
  vector<vector<int>> at(N, vector<int>(N, -1));
  for (int i = 0; i < NN; i++) at[path[i].first][path[i].second] = i;
  long long s = 0;
  for (int v = 0; v < NN; v++) {
    const auto [r, c] = pos[v];
    const int i = at[r][c];
    if (i >= 0) s += abs(i - EXIT_IDX);
  }
  return s;
}

static void build_belts() {
  vector<pair<int, int>> best;
  long long best_c = (1LL << 60);
  for (int t = 0; t < 48; t++) {
    if (!build_path_warn(t)) continue;
    const long long c = estimate_cost();
    if (c < best_c) {
      best_c = c;
      best = path;
    }
  }
  if (best.empty()) {
    path.clear();
    vector<vector<char>> vis(N, vector<char>(N, 0));
    dfs_path(0, EXIT_C, vis);
    reverse(path.begin(), path.end());
  } else {
    path = std::move(best);
  }

  belts.resize(NN - 1);
  pidx.assign(N, vector<int>(N, -1));
  for (int i = 0; i < NN; i++) pidx[path[i].first][path[i].second] = i;
  for (int i = 0; i + 1 < NN; i++) {
    belts[i] = {path[i].first, path[i].second, path[i + 1].first, path[i + 1].second};
  }
  EXIT_IDX = NN - 1;
}

static bool ok_swap_toward(int m, int cur) {
  const int dest = m + 1;
  const auto [r, c] = path[m];
  const int v = at_cell(r, c);
  if (dest == EXIT_IDX && v >= 0 && v < cur) return false;
  return true;
}

static bool ok_swap_away(int m, int cur) {
  const int src = m + 1;
  const auto [r, c] = path[src];
  const int v = at_cell(r, c);
  if (m == EXIT_IDX - 1 && v >= 0 && v < cur) return false;
  return true;
}

static bool slide_toward(int idx, int target, int cur) {
  if (idx == target) return false;
  if (idx < target) {
    if (!ok_swap_toward(idx, cur)) return false;
    ops.emplace_back(idx, 1);
    apply_rotate(idx, 1);
    return true;
  }
  if (!ok_swap_away(idx - 1, cur)) return false;
  ops.emplace_back(idx - 1, -1);
  apply_rotate(idx - 1, -1);
  return true;
}

static void solve(int &cur) {
  const int LIMIT = 100000;
  int stagnant = 0;

  while (cur < NN && (int)ops.size() < LIMIT) {
    if (grid[0][EXIT_C] == cur) {
      try_remove(cur);
      stagnant = 0;
      continue;
    }

    const int blocker = grid[0][EXIT_C];
    if (blocker >= 0 && blocker > cur) {
      const int bi = pidx[pos[blocker].first][pos[blocker].second];
      if (bi < 0) break;
      if (!slide_toward(bi, max(0, bi - 1), cur)) stagnant++;
      else stagnant = 0;
      if (stagnant > 5000) break;
      continue;
    }

    const int ci = pidx[pos[cur].first][pos[cur].second];
    if (ci < 0) break;
    if (!slide_toward(ci, EXIT_IDX, cur)) stagnant++;
    else stagnant = 0;
    if (stagnant > 5000) break;
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N;
  EXIT_C = N / 2;
  NN = N * N;
  EXIT_IDX = NN - 1;

  grid.assign(N, vector<int>(N));
  pos.assign(NN, {-1, -1});
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cin >> grid[i][j];
      pos[grid[i][j]] = {i, j};
    }
  }

  build_belts();

  int cur = 0;
  if (grid[0][EXIT_C] == 0) {
    grid[0][EXIT_C] = -1;
    pos[0] = {-1, -1};
    cur = 1;
  }

  solve(cur);

  cout << belts.size() << '\n';
  for (const auto &b : belts) {
    cout << 2 << ' ' << b.r0 << ' ' << b.c0 << ' ' << b.r1 << ' ' << b.c1 << '\n';
  }
  cout << ops.size() << '\n';
  for (const auto [m, d] : ops) cout << m << ' ' << d << '\n';
  return 0;
}

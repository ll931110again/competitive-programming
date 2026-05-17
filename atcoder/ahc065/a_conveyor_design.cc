// AHC065 A - Conveyor Design: https://atcoder.jp/contests/ahc065/tasks/ahc065_a
//
// Serpentine Hamiltonian path of length-2 belts (<=2 belts/cell). BFS on the path
// graph routes each box toward the exit; exports as many boxes as fit in 1e5 turns.

#if __has_include(<bits/stdc++.h>)
#include <bits/stdc++.h>
#else
#include <algorithm>
#include <iostream>
#include <queue>
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
static vector<vector<vector<int>>> belt_at;
static vector<pair<int, int>> path;
static vector<pair<int, int>> ops;

static int cell_id(int r, int c) {
  return r * N + c;
}

static void apply_rotate(int m, int d) {
  const auto& b = belts[m];
  const int v0 = grid[b.r0][b.c0], v1 = grid[b.r1][b.c1];
  grid[b.r0][b.c0] = v1;
  grid[b.r1][b.c1] = v0;
  if (v0 >= 0)
    pos[v0] = {b.r1, b.c1};
  if (v1 >= 0)
    pos[v1] = {b.r0, b.c0};
}

static void try_remove(int& cur) {
  if (grid[0][EXIT_C] != cur)
    return;
  grid[0][EXIT_C] = -1;
  pos[cur] = {-1, -1};
  cur++;
}

static bool build_path_warn(int seed) {
  vector<vector<char>> vis(N, vector<char>(N, 0));
  static constexpr int dr[4] = {-1, 1, 0, 0};
  static constexpr int dc[4] = {0, 0, -1, 1};
  path.clear();
  int r = 0, c = EXIT_C;
  for (int step = 0; step < NN; ++step) {
    path.emplace_back(r, c);
    vis[r][c] = 1;
    if (step + 1 == NN)
      break;
    vector<tuple<int, int, int>> cand;
    for (int t = 0; t < 4; ++t) {
      const int nr = r + dr[t], nc = c + dc[t];
      if (nr < 0 || nr >= N || nc < 0 || nc >= N || vis[nr][nc])
        continue;
      int deg = 0;
      for (int u = 0; u < 4; ++u) {
        const int ar = nr + dr[u], ac = nc + dc[u];
        if (ar >= 0 && ar < N && ac >= 0 && ac < N && !vis[ar][ac])
          deg++;
      }
      cand.emplace_back(deg, nr, nc);
    }
    if (cand.empty())
      return false;
    sort(cand.begin(), cand.end());
    const int pick = (seed + step) % (int)cand.size();
    tie(r, c) = pair<int, int>{get<1>(cand[pick]), get<2>(cand[pick])};
  }
  reverse(path.begin(), path.end());
  return (int)path.size() == NN;
}

static long long estimate_cost() {
  vector<vector<int>> at(N, vector<int>(N, -1));
  for (int i = 0; i < NN; ++i)
    at[path[i].first][path[i].second] = i;
  long long s = 0;
  for (int v = 0; v < NN; ++v) {
    const auto [r, c] = pos[v];
    const int i = at[r][c];
    if (i >= 0)
      s += abs(i - EXIT_IDX);
  }
  return s;
}

static void build_path_and_belts() {
  vector<pair<int, int>> best;
  long long best_c = (1LL << 60);
  for (int t = 0; t < 64; ++t) {
    if (!build_path_warn(t))
      continue;
    const long long c = estimate_cost();
    if (c < best_c) {
      best_c = c;
      best = path;
    }
  }
  if (best.empty()) {
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
    const int k = (int)(find(path.begin(), path.end(), pair<int, int>{0, EXIT_C}) - path.begin());
    rotate(path.begin(), path.begin() + k + 1, path.end());
  } else {
    path = std::move(best);
  }
  EXIT_IDX = NN - 1;

  belts.resize(NN - 1);
  belt_at.assign(N, vector<vector<int>>(N));
  for (int i = 0; i + 1 < NN; ++i) {
    belts[i] = {path[i].first, path[i].second, path[i + 1].first, path[i + 1].second};
    const int id = i;
    belt_at[path[i].first][path[i].second].push_back(id);
    belt_at[path[i + 1].first][path[i + 1].second].push_back(id);
  }
}

static int belt_dir(int bid, int fr, int fc, int tr, int tc) {
  const auto& b = belts[bid];
  if (b.r0 == fr && b.c0 == fc && b.r1 == tr && b.c1 == tc)
    return 1;
  return -1;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N;
  EXIT_C = N / 2;
  NN = N * N;

  grid.assign(N, vector<int>(N));
  pos.assign(NN, {-1, -1});
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      cin >> grid[i][j];
      pos[grid[i][j]] = {i, j};
    }
  }

  build_path_and_belts();

  int cur = 0;
  try_remove(cur);
  const int TGT = cell_id(0, EXIT_C);

  while (cur < NN && (int)ops.size() < 100000) {
    int sr = pos[cur].first, sc = pos[cur].second;
    if (sr < 0)
      break;

    const int S = cell_id(sr, sc);
    vector<int> dist(NN, -1), par(NN, -1), par_belt(NN, -1);
    queue<int> q;
    dist[S] = 0;
    q.push(S);
    while (!q.empty()) {
      const int u = q.front();
      q.pop();
      if (u == TGT)
        break;
      const int ur = u / N, uc = u % N;
      for (const int bid : belt_at[ur][uc]) {
        const auto& b = belts[bid];
        int nr, nc;
        if (b.r0 == ur && b.c0 == uc) {
          nr = b.r1;
          nc = b.c1;
        } else {
          nr = b.r0;
          nc = b.c0;
        }
        const int v = cell_id(nr, nc);
        if (dist[v] != -1)
          continue;
        dist[v] = dist[u] + 1;
        par[v] = u;
        par_belt[v] = bid;
        q.push(v);
      }
    }

    if (dist[TGT] == -1)
      break;

    vector<pair<int, int>> steps;
    for (int v = TGT; v != S; v = par[v])
      steps.push_back({par_belt[v], v});
    reverse(steps.begin(), steps.end());

    for (const auto [bid, to] : steps) {
      const int tr = to / N, tc = to % N;
      const int d = belt_dir(bid, sr, sc, tr, tc);
      ops.emplace_back(bid, d);
      apply_rotate(bid, d);
      try_remove(cur);
      if (cur >= NN || (int)ops.size() >= 100000)
        break;
      sr = pos[cur].first;
      sc = pos[cur].second;
    }
  }

  cout << belts.size() << '\n';
  for (const auto& b : belts) {
    cout << 2 << ' ' << b.r0 << ' ' << b.c0 << ' ' << b.r1 << ' ' << b.c1 << '\n';
  }
  cout << ops.size() << '\n';
  for (const auto [m, d] : ops)
    cout << m << ' ' << d << '\n';
  return 0;
}

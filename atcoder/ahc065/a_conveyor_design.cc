// AHC065 A - Conveyor Design: https://atcoder.jp/contests/ahc065/tasks/ahc065_a
//
// Serpentine Hamiltonian path of length-2 belts (<=2 belts/cell). BFS on the path
// graph routes each box toward the exit; exports as many boxes as fit in 1e5 turns.

#include <bits/stdc++.h>
#include <chrono>
using namespace std;

using i64 = long long;
namespace {

struct Belt {
  int r0, c0, r1, c1;
};

int N, EXIT_C, NN, EXIT_IDX;
vector<vector<int>> grid;
vector<pair<int, int>> pos;
vector<Belt> belts;
vector<vector<vector<int>>> belt_at;
vector<pair<int, int>> path;
vector<pair<int, int>> ops;

int cell_id(int r, int c) {
  return r * N + c;
}

void apply_rotate(int m, int /*d*/) {
  const auto& b = belts[m];
  const int v0 = grid[b.r0][b.c0], v1 = grid[b.r1][b.c1];
  grid[b.r0][b.c0] = v1;
  grid[b.r1][b.c1] = v0;
  if (v0 >= 0)
    pos[v0] = {b.r1, b.c1};
  if (v1 >= 0)
    pos[v1] = {b.r0, b.c0};
}

void try_remove(int& cur) {
  if (grid[0][EXIT_C] != cur)
    return;
  grid[0][EXIT_C] = -1;
  pos[cur] = {-1, -1};
  cur++;
}

int unvisited_degree(int r, int c, const vector<vector<char>>& vis) {
  constexpr int dr[4] = {-1, 1, 0, 0};
  constexpr int dc[4] = {0, 0, -1, 1};
  int deg = 0;
  for (int t = 0; t < 4; ++t) {
    const int nr = r + dr[t], nc = c + dc[t];
    if (nr >= 0 && nr < N && nc >= 0 && nc < N && !vis[nr][nc])
      deg++;
  }
  return deg;
}

bool build_path_warnsdorff(int seed) {
  constexpr int dr[4] = {-1, 1, 0, 0};
  constexpr int dc[4] = {0, 0, -1, 1};
  vector<vector<char>> vis(N, vector<char>(N, 0));
  path.clear();

  function<bool(int, int)> dfs = [&](int r, int c) -> bool {
    path.emplace_back(r, c);
    vis[r][c] = 1;
    if ((int)path.size() == NN)
      return true;

    vector<tuple<int, int, int, int>> cand;
    for (int t = 0; t < 4; ++t) {
      const int nr = r + dr[t], nc = c + dc[t];
      if (nr < 0 || nr >= N || nc < 0 || nc >= N || vis[nr][nc])
        continue;
      const int deg = unvisited_degree(nr, nc, vis);
      const int tie = (seed * 1315423911 + r * 997 + c * 37 + t * 17) & 7;
      cand.emplace_back(deg, tie, nr, nc);
    }
    if (cand.empty()) {
      path.pop_back();
      vis[r][c] = 0;
      return false;
    }
    sort(cand.begin(), cand.end());
    for (const auto& item : cand) {
      if (dfs(get<2>(item), get<3>(item)))
        return true;
    }
    path.pop_back();
    vis[r][c] = 0;
    return false;
  };

  if (!dfs(0, EXIT_C))
    return false;
  reverse(path.begin(), path.end());
  return (int)path.size() == NN;
}

bool path_is_valid(const vector<pair<int, int>>& p) {
  if ((int)p.size() != NN)
    return false;
  for (int i = 0; i + 1 < NN; ++i) {
    const int dr = abs(p[i].first - p[i + 1].first);
    const int dc = abs(p[i].second - p[i + 1].second);
    if (dr + dc != 1)
      return false;
  }
  return p.back() == pair<int, int>{0, EXIT_C};
}

i64 estimate_cost() {
  vector<vector<int>> at(N, vector<int>(N, -1));
  for (int i = 0; i < NN; ++i)
    at[path[i].first][path[i].second] = i;
  i64 s = 0;
  for (int v = 0; v < NN; ++v) {
    const auto [r, c] = pos[v];
    const int i = at[r][c];
    if (i >= 0)
      s += abs(i - EXIT_IDX);
  }
  return s;
}

void build_path_and_belts() {
  vector<pair<int, int>> best;
  i64 best_c = (1LL << 60);
  const auto deadline =
      chrono::steady_clock::now() + chrono::milliseconds(800);
  for (int t = 0; chrono::steady_clock::now() < deadline; ++t) {
    if (!build_path_warnsdorff(t))
      continue;
    if (!path_is_valid(path))
      continue;
    const i64 c = estimate_cost();
    if (c < best_c) {
      best_c = c;
      best = path;
    }
  }
  if (best.empty()) {
    for (int t = 0; t < 8192; ++t) {
      if (!build_path_warnsdorff(t))
        continue;
      if (!path_is_valid(path))
        continue;
      best = path;
      break;
    }
  }
  if (best.empty())
    throw runtime_error("failed to build Hamiltonian path");
  path = std::move(best);
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

int belt_dir(int bid, int fr, int fc, int tr, int tc) {
  const auto& b = belts[bid];
  if (b.r0 == fr && b.c0 == fc && b.r1 == tr && b.c1 == tc)
    return 1;
  return -1;
}

} // namespace

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

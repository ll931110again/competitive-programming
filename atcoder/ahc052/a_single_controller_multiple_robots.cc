// AHC052 A - Single Controller Multiple Robots
// https://atcoder.jp/contests/ahc052/tasks/ahc052_a
//
// Predefine K sweep programs; greedily pick the button that waxes the most new cells.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int kDirs = 4;
constexpr int kStay = 4;
constexpr int kDr[5] = {-1, 1, 0, 0, 0};
constexpr int kDc[5] = {0, 0, -1, 1, 0};
constexpr char kCmd[5] = {'U', 'D', 'L', 'R', 'S'};

int N, M, K;
vector<pair<int, int>> robots;
vector<vector<bool>> wall_v, wall_h;
vector<vector<int>> owner;
vector<vector<char>> waxed;

bool can_move(int r, int c, int d) {
  if (d == kStay)
    return true;
  const int nr = r + kDr[d], nc = c + kDc[d];
  if (nr < 0 || nr >= N || nc < 0 || nc >= N)
    return false;
  if (kDr[d] == 0)
    return !wall_v[r][min(c, nc)];
  return !wall_h[min(r, nr)][c];
}

int apply_program(const vector<vector<int>>& prog, int& newly) {
  newly = 0;
  vector<pair<int, int>> nxt = robots;
  for (int i = 0; i < M; ++i) {
    const int d = prog[i][0];
    const auto [r, c] = robots[i];
    if (can_move(r, c, d)) {
      nxt[i] = {r + kDr[d], c + kDc[d]};
    }
  }
  for (int i = 0; i < M; ++i) {
    const auto [r, c] = nxt[i];
    if (owner[r][c] == -1) {
      owner[r][c] = i;
      waxed[r][c] = 1;
      newly++;
    }
  }
  robots = std::move(nxt);
  return newly;
}

int remaining_cells() {
  int rem = 0;
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      if (owner[i][j] == -1)
        rem++;
  return rem;
}

vector<vector<int>> nearest_dir_program() {
  vector<vector<int>> prog(M, vector<int>(1, kStay));
  for (int i = 0; i < M; ++i) {
    const auto [r, c] = robots[i];
    int best_d = kStay;
    int best = INT_MAX;
    for (int d = 0; d < kDirs; ++d) {
      if (!can_move(r, c, d))
        continue;
      const int nr = r + kDr[d], nc = c + kDc[d];
      if (owner[nr][nc] != -1)
        continue;
      int dist = INT_MAX;
      for (int ti = 0; ti < N; ++ti) {
        for (int tj = 0; tj < N; ++tj) {
          if (owner[ti][tj] != -1)
            continue;
          dist = min(dist, abs(ti - nr) + abs(tj - nc));
        }
      }
      if (dist < best) {
        best = dist;
        best_d = d;
      }
    }
    prog[i][0] = best_d;
  }
  return prog;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> M >> K;
  robots.resize(M);
  for (int i = 0; i < M; ++i)
    cin >> robots[i].first >> robots[i].second;

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

  owner.assign(N, vector<int>(N, -1));
  waxed.assign(N, vector<char>(N, 0));
  for (int i = 0; i < M; ++i) {
    owner[robots[i].first][robots[i].second] = i;
    waxed[robots[i].first][robots[i].second] = 1;
  }

  vector<vector<vector<int>>> programs(K);
  for (int k = 0; k < K; ++k) {
    programs[k].assign(M, vector<int>(1, k % 5));
    for (int i = 0; i < M; ++i)
      programs[k][i][0] = (k + i) % kDirs;
  }

  vector<int> actions;
  const int limit = 2 * N * N;
  while (remaining_cells() > 0 && (int)actions.size() < limit) {
    int best_k = 0;
    int best_gain = -1;
    auto saved = robots;
    auto saved_owner = owner;
    for (int k = 0; k < K; ++k) {
      robots = saved;
      owner = saved_owner;
      int gain = 0;
      apply_program(programs[k], gain);
      if (gain > best_gain) {
        best_gain = gain;
        best_k = k;
      }
    }
    robots = saved;
    owner = saved_owner;
    if (best_gain <= 0) {
      programs[best_k] = nearest_dir_program();
      apply_program(programs[best_k], best_gain);
    } else {
      apply_program(programs[best_k], best_gain);
    }
    actions.push_back(best_k);
  }

  for (int k = 0; k < K; ++k) {
    for (int i = 0; i < M; ++i)
      cout << kCmd[programs[k][i][0]];
    cout << '\n';
  }
  for (int a : actions)
    cout << a << '\n';
  return 0;
}

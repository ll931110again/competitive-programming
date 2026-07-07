// AHC062 A - King's Tour: https://atcoder.jp/contests/ahc062/tasks/ahc062_a
//
// Hamiltonian king-move path on N×N. Serpentine on consecutive rows (king-valid
// joins); multi-start row/direction flips; time-budgeted SA with validated
// segment reversals and random swaps.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

int N;
vector<vector<i64>> A;
vector<pair<int, int>> path;
vector<vector<int>> pos;

bool king_adj(int r0, int c0, int r1, int c1) {
  return max(abs(r0 - r1), abs(c0 - c1)) == 1;
}

bool path_valid(const vector<pair<int, int>>& p) {
  if ((int)p.size() != N * N)
    return false;
  vector<vector<char>> seen(N, vector<char>(N, 0));
  for (int k = 0; k < (int)p.size(); ++k) {
    const auto [r, c] = p[k];
    if (r < 0 || r >= N || c < 0 || c >= N || seen[r][c])
      return false;
    seen[r][c] = 1;
    if (k > 0 && !king_adj(p[k - 1].first, p[k - 1].second, r, c))
      return false;
  }
  return true;
}

i64 score_path(const vector<pair<int, int>>& p) {
  i64 s = 0;
  for (int k = 0; k < (int)p.size(); ++k)
    s += i64(k) * A[p[k].first][p[k].second];
  return s;
}

void build_serpentine(bool flip_rows, bool flip_start) {
  path.clear();
  vector<int> rows(N);
  iota(rows.begin(), rows.end(), 0);
  if (flip_rows)
    reverse(rows.begin(), rows.end());
  for (int ii = 0; ii < N; ++ii) {
    const int i = rows[ii];
    const bool left_to_right = ((ii % 2) == 0) ^ flip_start;
    if (left_to_right) {
      for (int j = 0; j < N; ++j)
        path.emplace_back(i, j);
    } else {
      for (int j = N - 1; j >= 0; --j)
        path.emplace_back(i, j);
    }
  }
}

void rebuild_pos() {
  pos.assign(N, vector<int>(N, -1));
  for (int k = 0; k < (int)path.size(); ++k)
    pos[path[k].first][path[k].second] = k;
}

bool try_reverse_segment(int l, int r) {
  if (l <= 0 || r >= (int)path.size() - 1 || l >= r)
    return false;
  reverse(path.begin() + l, path.begin() + r + 1);
  if (!path_valid(path)) {
    reverse(path.begin() + l, path.begin() + r + 1);
    return false;
  }
  rebuild_pos();
  return true;
}

bool try_swap_positions(int k1, int k2) {
  if (k1 > k2)
    swap(k1, k2);
  if (k1 == k2)
    return true;
  auto trial = path;
  swap(trial[k1], trial[k2]);
  if (!path_valid(trial))
    return false;
  path = std::move(trial);
  rebuild_pos();
  return true;
}

void improve(mt19937& rng, chrono::steady_clock::time_point deadline) {
  i64 best = score_path(path);
  double temp = 1.0;

  while (chrono::steady_clock::now() < deadline) {
    const int kind = uniform_int_distribution<int>(0, 1)(rng);
    auto saved = path;
    const i64 old = best;
    bool ok = false;

    if (kind == 0) {
      const int l = uniform_int_distribution<int>(1, N * N - 3)(rng);
      const int r = uniform_int_distribution<int>(l + 1, min(l + 40, N * N - 2))(rng);
      ok = try_reverse_segment(l, r);
    } else {
      const int k1 = uniform_int_distribution<int>(0, N * N - 1)(rng);
      const int k2 = uniform_int_distribution<int>(0, N * N - 1)(rng);
      ok = try_swap_positions(k1, k2);
    }

    if (!ok) {
      path = std::move(saved);
      continue;
    }

    const i64 nw = score_path(path);
    const bool accept =
        nw >= old || bernoulli_distribution(exp((nw - old) / max(1.0, temp * 5e4)))(rng);
    if (accept) {
      best = max(best, nw);
    } else {
      path = std::move(saved);
      rebuild_pos();
    }
    temp *= 0.9995;
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N;
  A.assign(N, vector<i64>(N));
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      cin >> A[i][j];

  const auto deadline =
      chrono::steady_clock::now() + chrono::milliseconds(1900);
  mt19937 rng(712367);

  i64 best_score = LLONG_MIN;
  vector<pair<int, int>> best_path;

  for (int trial = 0; trial < 4 && chrono::steady_clock::now() < deadline;
       ++trial) {
    build_serpentine(trial & 1, (trial >> 1) & 1);
    if (!path_valid(path))
      continue;
    rebuild_pos();
    const auto ms_left = chrono::duration_cast<chrono::milliseconds>(
        deadline - chrono::steady_clock::now());
    const auto trial_end = chrono::steady_clock::now() +
                           ms_left / max(1, 4 - trial);
    improve(rng, trial_end);
    const i64 sc = score_path(path);
    if (sc > best_score) {
      best_score = sc;
      best_path = path;
    }
  }

  path = std::move(best_path);
  for (const auto [i, j] : path)
    cout << i << ' ' << j << '\n';
  return 0;
}

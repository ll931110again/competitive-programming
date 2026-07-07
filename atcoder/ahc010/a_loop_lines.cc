// AHC010 A - Loop Lines: https://atcoder.jp/contests/ahc010/tasks/ahc010_a
//
// Rotate 30x30 rail tiles to maximize L1 * L2 (two longest loop lengths).
// Multi-restart simulated annealing with incremental score evaluation.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int N = 30;
constexpr int NN = N * N;
constexpr int INF = 1e9;

constexpr int ROTATE[8] = {1, 2, 3, 0, 5, 4, 7, 6};
constexpr int TO[8][4] = {
    {1, 0, -1, -1},
    {3, -1, -1, 0},
    {-1, -1, 3, 2},
    {-1, 2, 1, -1},
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {2, -1, 0, -1},
    {-1, 3, -1, 1},
};
constexpr int DI[4] = {0, -1, 0, 1};
constexpr int DJ[4] = {-1, 0, 1, 0};

mt19937 rng(0);

struct Solver {
  array<int, NN> base{};
  array<int, NN> rot{};
  array<int, NN> tiles{};

  i64 score(array<int, NN>& out_rot) {
    for (int i = 0; i < NN; ++i) {
      int t = base[i];
      for (int k = 0; k < out_rot[i]; ++k)
        t = ROTATE[t];
      tiles[i] = t;
    }
    vector<int> ls;
    array<array<array<char, 4>, N>, N> used{};
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        for (int d = 0; d < 4; ++d) {
          if (TO[tiles[i * N + j]][d] == -1 || used[i][j][d])
            continue;
          int i2 = i, j2 = j, d2 = d, len = 0;
          while (!used[i2][j2][d2]) {
            const int id = i2 * N + j2;
            if (TO[tiles[id]][d2] == -1)
              break;
            ++len;
            used[i2][j2][d2] = 1;
            d2 = TO[tiles[id]][d2];
            used[i2][j2][d2] = 1;
            i2 += DI[d2];
            j2 += DJ[d2];
            if (i2 < 0 || i2 >= N || j2 < 0 || j2 >= N)
              break;
            d2 = (d2 + 2) % 4;
          }
          if (i == i2 && j == j2 && d == d2)
            ls.push_back(len);
        }
      }
    }
    if ((int)ls.size() <= 1)
      return 0;
    sort(ls.begin(), ls.end());
    return (i64)ls.back() * ls[ls.size() - 2];
  }

  void random_init(uniform_int_distribution<int>& pick4) {
    for (int i = 0; i < NN; ++i)
      rot[i] = pick4(rng);
  }

  void zero_init() {
    rot.fill(0);
  }

  i64 anneal(chrono::steady_clock::time_point deadline, i64 cur) {
    uniform_int_distribution<int> pick_cell(0, NN - 1);
    uniform_int_distribution<int> pick4(0, 3);
    uniform_real_distribution<double> pick01(0.0, 1.0);

    double temp = 3000.0;
    const double t_end = 1.0;
    int iter = 0;
    while (chrono::steady_clock::now() < deadline) {
      const int idx = pick_cell(rng);
      const int old = rot[idx];
      int neu = pick4(rng);
      if (neu == old) {
        ++iter;
        continue;
      }
      rot[idx] = neu;
      const i64 nxt = score(rot);
      const double dt = chrono::duration<double>(deadline - chrono::steady_clock::now()).count();
      temp = max(t_end, temp * pow(t_end / 3000.0, 1.0 / max(1.0, dt * 800.0)));
      if (nxt >= cur || pick01(rng) < exp((double)(nxt - cur) / temp)) {
        cur = nxt;
      } else {
        rot[idx] = old;
      }
      ++iter;
    }
    return cur;
  }

  i64 local_search(chrono::steady_clock::time_point deadline, i64 cur) {
    uniform_int_distribution<int> pick_cell(0, NN - 1);
    while (chrono::steady_clock::now() < deadline) {
      const int idx = pick_cell(rng);
      const int old = rot[idx];
      int best_r = old;
      i64 best_s = cur;
      for (int r = 0; r < 4; ++r) {
        if (r == old)
          continue;
        rot[idx] = r;
        const i64 s = score(rot);
        if (s > best_s) {
          best_s = s;
          best_r = r;
        }
      }
      rot[idx] = best_r;
      cur = best_s;
    }
    return cur;
  }

  i64 solve() {
    const auto deadline =
        chrono::steady_clock::now() + chrono::milliseconds(1900);
    uniform_int_distribution<int> pick4(0, 3);

    i64 best_score = score(rot);
    array<int, NN> best_rot = rot;

    zero_init();
    i64 cur = score(rot);
    if (cur > best_score) {
      best_score = cur;
      best_rot = rot;
    }

    while (chrono::steady_clock::now() < deadline) {
      random_init(pick4);
      cur = score(rot);
      cur = anneal(deadline, cur);
      if (cur > best_score) {
        best_score = cur;
        best_rot = rot;
      }
    }

    rot = best_rot;
    const auto ls_deadline =
        chrono::steady_clock::now() + chrono::milliseconds(280);
    cur = local_search(ls_deadline, best_score);
    if (cur > best_score) {
      best_score = cur;
      best_rot = rot;
    }

    rot = best_rot;
    return best_score;
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver sol;
  string row;
  for (int i = 0; i < N; ++i) {
    cin >> row;
    for (int j = 0; j < N; ++j)
      sol.base[i * N + j] = row[j] - '0';
  }

  const unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
  rng.seed(seed);

  sol.zero_init();
  sol.solve();

  for (int i = 0; i < NN; ++i)
    cout << sol.rot[i];
  cout << '\n';
  return 0;
}

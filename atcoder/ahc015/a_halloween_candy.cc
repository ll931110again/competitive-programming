// AHC015 A - Halloween Candy: https://atcoder.jp/contests/ahc015/tasks/ahc015_a
//
// Rule-based clustering (major flavor right, others top/bottom-left) with Monte
// Carlo playouts to pick the tilt each turn. Exact 4-way search near the end.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int N = 10;
constexpr int TURNS = N * N;
constexpr int FLAVORS = 3;
constexpr char DIRS[4] = {'F', 'B', 'L', 'R'};

using Board = array<array<int, N>, N>;

struct Input {
  array<int, TURNS> f{};
  array<int, TURNS> p{};
};

Board empty_board() {
  Board b{};
  for (auto& row : b)
    row.fill(0);
  return b;
}

void tilt(Board& b, char d) {
  if (d == 'L') {
    for (int i = 0; i < N; ++i) {
      int k = 0;
      for (int j = 0; j < N; ++j) {
        if (b[i][j] != 0) {
          b[i][k] = b[i][j];
          if (k != j)
            b[i][j] = 0;
          ++k;
        }
      }
    }
  } else if (d == 'R') {
    for (int i = 0; i < N; ++i) {
      int k = N - 1;
      for (int j = N - 1; j >= 0; --j) {
        if (b[i][j] != 0) {
          b[i][k] = b[i][j];
          if (k != j)
            b[i][j] = 0;
          --k;
        }
      }
    }
  } else if (d == 'F') {
    for (int j = 0; j < N; ++j) {
      int k = 0;
      for (int i = 0; i < N; ++i) {
        if (b[i][j] != 0) {
          b[k][j] = b[i][j];
          if (k != i)
            b[i][j] = 0;
          ++k;
        }
      }
    }
  } else if (d == 'B') {
    for (int j = 0; j < N; ++j) {
      int k = N - 1;
      for (int i = N - 1; i >= 0; --i) {
        if (b[i][j] != 0) {
          b[k][j] = b[i][j];
          if (k != i)
            b[i][j] = 0;
          --k;
        }
      }
    }
  }
}

void place(Board& b, int p, int flavor) {
  int q = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (b[i][j] == 0) {
        ++q;
        if (q == p) {
          b[i][j] = flavor;
          return;
        }
      }
    }
  }
}

void apply_turn(Board& b, char d, int flavor, int p) {
  tilt(b, d);
  place(b, p, flavor);
}

i64 component_sum_sq(const Board& b) {
  array<array<char, N>, N> vis{};
  for (auto& row : vis)
    row.fill(0);
  i64 num = 0;
  constexpr int dr[4] = {1, 0, -1, 0};
  constexpr int dc[4] = {0, 1, 0, -1};
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (vis[i][j] || b[i][j] == 0)
        continue;
      const int c = b[i][j];
      vis[i][j] = 1;
      int size = 1;
      vector<pair<int, int>> st = {{i, j}};
      while (!st.empty()) {
        const auto [r, col] = st.back();
        st.pop_back();
        for (int k = 0; k < 4; ++k) {
          const int r2 = r + dr[k], c2 = col + dc[k];
          if (r2 < 0 || r2 >= N || c2 < 0 || c2 >= N || vis[r2][c2] ||
              b[r2][c2] != c)
            continue;
          vis[r2][c2] = 1;
          ++size;
          st.emplace_back(r2, c2);
        }
      }
      num += i64(size) * size;
    }
  }
  return num;
}

i64 denom_sq(const array<int, TURNS>& f) {
  array<int, FLAVORS + 1> d{};
  for (int x : f)
    ++d[x];
  return i64(d[1]) * d[1] + i64(d[2]) * d[2] + i64(d[3]) * d[3];
}

double eval_board(const Board& b, i64 denom) {
  return 1e6 * (double)component_sum_sq(b) / (double)denom;
}

struct FlavorRanks {
  int major = 1;
  int minor[2]{2, 3};
};

FlavorRanks rank_remaining(const array<int, TURNS>& f, int next_turn) {
  array<int, FLAVORS + 1> rem{};
  for (int i = next_turn; i < TURNS; ++i)
    ++rem[f[i]];
  vector<pair<int, int>> v;
  for (int c = 1; c <= FLAVORS; ++c)
    v.emplace_back(rem[c], c);
  sort(v.begin(), v.end(), greater<>());
  FlavorRanks r;
  r.major = v[0].second;
  r.minor[0] = v[1].second;
  r.minor[1] = v[2].second;
  return r;
}

char rule_dir(int next_turn, char last_h, const array<int, TURNS>& f) {
  if (next_turn >= TURNS)
    return 'R';
  const int next_f = f[next_turn];
  const auto rk = rank_remaining(f, next_turn);
  const int A = rk.major;
  const int B = rk.minor[0];
  const int C = rk.minor[1];

  if (next_f == A)
    return 'L';
  if (last_h == 'R') {
    if (next_f == B)
      return 'B';
    if (next_f == C)
      return 'F';
  }
  return 'R';
}

char update_last_h(char last_h, char d) {
  if (d == 'L' || d == 'R')
    return d;
  return last_h;
}

int empty_count(const Board& b) {
  int s = 0;
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      s += b[i][j] == 0;
  return s;
}

int random_place(int empty, mt19937& rng) {
  return uniform_int_distribution<int>(1, empty)(rng);
}

double playout(Board b, int next_turn, char last_h, const Input& in, i64 denom,
               mt19937& rng) {
  while (next_turn < TURNS) {
    const char d = rule_dir(next_turn, last_h, in.f);
    const int empty = empty_count(b);
    const int p = random_place(empty, rng);
    apply_turn(b, d, in.f[next_turn], p);
    last_h = update_last_h(last_h, d);
    ++next_turn;
  }
  return eval_board(b, denom);
}

double search_endgame(const Board& start, int out_idx, char last_h,
                      const Input& in, i64 denom, int depth, int known_out,
                      mt19937& rng) {
  if (depth == 0)
    return eval_board(start, denom);

  const int next_turn = out_idx + 1;
  double best = -1.0;
  for (char d : DIRS) {
    Board b = start;
    const int empty = empty_count(b);
    int p;
    if (out_idx < known_out)
      p = in.p[next_turn];
    else
      p = random_place(empty, rng);
    apply_turn(b, d, in.f[next_turn], p);
    const double sc = search_endgame(b, out_idx + 1, update_last_h(last_h, d),
                                     in, denom, depth - 1, known_out, rng);
    best = max(best, sc);
  }
  return best;
}

struct Solver {
  Input in;
  i64 denom = 1;
  Board board = empty_board();
  char last_h = 'R';
  chrono::steady_clock::time_point deadline;
  mt19937 rng{42};

  explicit Solver(chrono::milliseconds budget = chrono::milliseconds(1900))
      : deadline(chrono::steady_clock::now() + budget) {}

  char choose(int out_idx) {
    const int rem_out = (TURNS - 1) - out_idx;
    if (rem_out <= 0)
      return 'R';

    if (rem_out <= 6) {
      double best = -1.0;
      char best_d = 'R';
      for (char d : DIRS) {
        const double sc =
            search_endgame(board, out_idx, last_h, in, denom, rem_out, out_idx, rng);
        if (sc > best) {
          best = sc;
          best_d = d;
        }
      }
      return best_d;
    }

    const int next_turn = out_idx + 1;
    int trials = max(8, 220 - out_idx * 2);
    double best = -1.0;
    char best_d = 'R';
    for (char d : DIRS) {
      Board nb = board;
      apply_turn(nb, d, in.f[next_turn], in.p[next_turn]);
      double sum = eval_board(nb, denom);
      int used = 0;
      for (int k = 0; k < trials; ++k) {
        if (chrono::steady_clock::now() > deadline)
          break;
        sum += playout(nb, next_turn + 1, update_last_h(last_h, d), in, denom, rng);
        ++used;
      }
      const double avg = sum / (used + 1);
      if (avg > best) {
        best = avg;
        best_d = d;
      }
    }
    return best_d;
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver solver;
  for (int i = 0; i < TURNS; ++i)
    cin >> solver.in.f[i];
  cin >> solver.in.p[0];
  place(solver.board, solver.in.p[0], solver.in.f[0]);

  for (int out_idx = 0; out_idx < TURNS - 1; ++out_idx) {
    cin >> solver.in.p[out_idx + 1];
    const char d = solver.choose(out_idx);
    apply_turn(solver.board, d, solver.in.f[out_idx + 1], solver.in.p[out_idx + 1]);
    solver.last_h = update_last_h(solver.last_h, d);
    cout << d << '\n' << flush;
  }
  return 0;
}

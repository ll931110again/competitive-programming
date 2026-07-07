// AHC004 A - Alien's Genome Assembly: https://atcoder.jp/contests/ahc004/tasks/ahc004_a
//
// Top-scoring pipeline (terry_u16 / iwashi31 / inani_waon):
//   1. Drop hints contained in another; greedily merge suffix/prefix overlaps.
//   2. L <= 6: adaptive greedy — each step place the unplaced hint with best overlap.
//   3. L >= 7: length-descending order with within-length shuffles.
//   4. Hill climb on placement order and fixed orientations (swap / flip neighbors).
// Score is always measured against the original input hints.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int N = 20;
constexpr int NN = N * N;
constexpr int MIN_MERGE_OVERLAP = 4;
constexpr double TIME_LIMIT = 2.85;

vector<string> original_hints;
vector<string> work_hints;
chrono::steady_clock::time_point t0;

int wrap(int x) {
  if (x >= N)
    return x - N;
  if (x < 0)
    return x + N;
  return x;
}

bool timed_out() {
  return chrono::duration<double>(chrono::steady_clock::now() - t0).count() >
         TIME_LIMIT;
}

struct TorusGrid {
  array<array<char, N>, N> cells{};

  TorusGrid() {
    for (auto& row : cells)
      row.fill('.');
  }

  char at(int r, int c) const { return cells[r][c]; }

  bool can_place(const string& s, int r, int c, int dir) const {
    for (int k = 0; k < (int)s.size(); ++k) {
      const int i = wrap(r + dir * k);
      const int j = wrap(c + (1 - dir) * k);
      const char ch = cells[i][j];
      if (ch != '.' && ch != s[k])
        return false;
    }
    return true;
  }

  void place(const string& s, int r, int c, int dir) {
    for (int k = 0; k < (int)s.size(); ++k) {
      const int i = wrap(r + dir * k);
      const int j = wrap(c + (1 - dir) * k);
      cells[i][j] = s[k];
    }
  }

  bool contains(const string& s) const {
    for (int r = 0; r < N; ++r) {
      for (int c = 0; c < N; ++c) {
        for (int dir = 0; dir < 2; ++dir) {
          bool ok = true;
          for (int k = 0; k < (int)s.size(); ++k) {
            const int i = wrap(r + dir * k);
            const int j = wrap(c + (1 - dir) * k);
            if (cells[i][j] != s[k]) {
              ok = false;
              break;
            }
          }
          if (ok)
            return true;
        }
      }
    }
    return false;
  }
};

i64 score_grid(const TorusGrid& grid) {
  int matched = 0;
  for (const auto& s : original_hints) {
    if (grid.contains(s))
      ++matched;
  }
  if (matched < (int)original_hints.size())
    return llround(1e8 * (long double)matched / (int)original_hints.size());

  int blanks = 0;
  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c) {
      if (grid.at(r, c) == '.')
        ++blanks;
    }
  }
  return llround(1e8 * (2LL * NN) / (2LL * NN - blanks));
}

struct Placement {
  int row = 0;
  int col = 0;
  int dir = 0;
  int overlap = 0;
  int new_cells = 0;
};

bool better_placement(const Placement& a, const Placement& b, int horiz_bias) {
  if (a.overlap != b.overlap)
    return a.overlap > b.overlap;
  if (a.new_cells != b.new_cells)
    return a.new_cells < b.new_cells;
  if (horiz_bias && a.dir != b.dir)
    return a.dir == 0;
  return false;
}

optional<Placement> best_placement(const TorusGrid& grid, const string& s,
                                   optional<int> fixed_dir, int horiz_bias) {
  optional<Placement> best;
  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c) {
      const int d0 = fixed_dir ? *fixed_dir : 0;
      const int d1 = fixed_dir ? *fixed_dir + 1 : 2;
      for (int dir = d0; dir < d1; ++dir) {
        if (!grid.can_place(s, r, c, dir))
          continue;
        Placement cur;
        cur.row = r;
        cur.col = c;
        cur.dir = dir;
        for (int k = 0; k < (int)s.size(); ++k) {
          const int i = wrap(r + dir * k);
          const int j = wrap(c + (1 - dir) * k);
          if (grid.at(i, j) == '.')
            ++cur.new_cells;
          else
            ++cur.overlap;
        }
        if (!best || better_placement(cur, *best, horiz_bias))
          best = cur;
      }
    }
  }
  return best;
}

void shuffle_length_groups(vector<string>& hints, mt19937& rng) {
  for (int i = 0; i < (int)hints.size();) {
    int j = i + 1;
    while (j < (int)hints.size() && (int)hints[j].size() == (int)hints[i].size())
      ++j;
    if (j - i > 1)
      shuffle(hints.begin() + i, hints.begin() + j, rng);
    i = j;
  }
}

void remove_contained_hints(vector<string>& hints) {
  sort(hints.begin(), hints.end(), [](const string& a, const string& b) {
    if (a.size() != b.size())
      return a.size() > b.size();
    return a < b;
  });
  vector<string> kept;
  for (const string& s : hints) {
    bool contained = false;
    for (const string& t : kept) {
      if (t.find(s) != string::npos) {
        contained = true;
        break;
      }
    }
    if (!contained)
      kept.push_back(s);
  }
  hints = std::move(kept);
}

int suffix_prefix_overlap(const string& left, const string& right) {
  const int limit = min((int)left.size(), (int)right.size());
  for (int k = limit; k >= MIN_MERGE_OVERLAP; --k) {
    if (left.compare(left.size() - k, k, right, 0, k) == 0)
      return k;
  }
  return 0;
}

void merge_overlapping_hints(vector<string>& hints) {
  for (;;) {
    int best_i = -1;
    int best_j = -1;
    int best_overlap = MIN_MERGE_OVERLAP - 1;
    for (int i = 0; i < (int)hints.size(); ++i) {
      for (int j = 0; j < (int)hints.size(); ++j) {
        if (i == j)
          continue;
        const int ov = suffix_prefix_overlap(hints[i], hints[j]);
        if (ov > best_overlap) {
          best_overlap = ov;
          best_i = i;
          best_j = j;
        }
      }
    }
    if (best_overlap < MIN_MERGE_OVERLAP)
      break;
    hints[best_i] += hints[best_j].substr(best_overlap);
    hints.erase(hints.begin() + best_j);
  }
}

void preprocess_hints() {
  work_hints = original_hints;
  sort(work_hints.begin(), work_hints.end());
  work_hints.erase(unique(work_hints.begin(), work_hints.end()), work_hints.end());
  remove_contained_hints(work_hints);
  merge_overlapping_hints(work_hints);
}

TorusGrid greedy_fixed_order(const vector<string>& order,
                             const vector<int>& orient, int horiz_bias) {
  TorusGrid grid;
  for (int idx = 0; idx < (int)order.size(); ++idx) {
    const string& s = order[idx];
    if (grid.contains(s))
      continue;
    const auto place = best_placement(grid, s, orient[idx], horiz_bias);
    if (place)
      grid.place(s, place->row, place->col, place->dir);
  }
  for (int pass = 0; pass < 2; ++pass) {
    for (const string& s : order) {
      if (grid.contains(s))
        continue;
      const auto place = best_placement(grid, s, nullopt, horiz_bias);
      if (place)
        grid.place(s, place->row, place->col, place->dir);
    }
  }
  return grid;
}

TorusGrid adaptive_greedy(vector<string> hints, int horiz_bias) {
  TorusGrid grid;
  vector<char> used(hints.size(), 0);
  int placed = 0;
  while (placed < (int)hints.size()) {
    int best_idx = -1;
    optional<Placement> best_place;
    for (int i = 0; i < (int)hints.size(); ++i) {
      if (used[i])
        continue;
      if (grid.contains(hints[i])) {
        used[i] = 1;
        ++placed;
        continue;
      }
      const auto cand = best_placement(grid, hints[i], nullopt, horiz_bias);
      if (!cand)
        continue;
      if (best_idx < 0 ||
          better_placement(*cand, *best_place, horiz_bias)) {
        best_idx = i;
        best_place = cand;
      }
    }
    if (best_idx < 0)
      break;
    grid.place(hints[best_idx], best_place->row, best_place->col, best_place->dir);
    used[best_idx] = 1;
    ++placed;
  }
  for (const string& s : hints) {
    if (grid.contains(s))
      continue;
    const auto place = best_placement(grid, s, nullopt, horiz_bias);
    if (place)
      grid.place(s, place->row, place->col, place->dir);
  }
  return grid;
}

TorusGrid ordered_greedy(vector<string> order, int horiz_bias) {
  sort(order.begin(), order.end(), [](const string& a, const string& b) {
    if (a.size() != b.size())
      return a.size() > b.size();
    return a < b;
  });
  vector<int> orient(order.size(), 0);
  return greedy_fixed_order(order, orient, horiz_bias);
}

struct SolverState {
  TorusGrid best_grid;
  i64 best_score = -1;

  void consider(TorusGrid grid) {
    const i64 sc = score_grid(grid);
    if (sc > best_score) {
      best_score = sc;
      best_grid = std::move(grid);
    }
  }
};

void hill_climb(SolverState& state, mt19937& rng) {
  vector<string> order = work_hints;
  sort(order.begin(), order.end(), [](const string& a, const string& b) {
    if (a.size() != b.size())
      return a.size() > b.size();
    return a < b;
  });
  vector<int> orient(order.size(), 0);

  TorusGrid cur = greedy_fixed_order(order, orient, 1);
  i64 cur_score = score_grid(cur);
  state.consider(cur);

  uniform_int_distribution<int> pick(0, (int)order.size() - 1);
  while (!timed_out()) {
    const int kind = rng() % 3;
    vector<string> next_order = order;
    vector<int> next_orient = orient;

    if (kind == 0) {
      const int i = pick(rng);
      const int j = pick(rng);
      swap(next_order[i], next_order[j]);
    } else if (kind == 1) {
      next_orient[pick(rng)] ^= 1;
    } else {
      shuffle_length_groups(next_order, rng);
    }

    TorusGrid grid = greedy_fixed_order(next_order, next_orient, 1);
    const i64 sc = score_grid(grid);
    if (sc >= cur_score) {
      cur_score = sc;
      order = std::move(next_order);
      orient = std::move(next_orient);
      cur = std::move(grid);
      state.consider(cur);
    }
  }
}

void solve() {
  preprocess_hints();
  const int max_len =
      work_hints.empty() ? 0 : (int)max_element(work_hints.begin(), work_hints.end(),
                                                [](const string& a, const string& b) {
                                                  return a.size() < b.size();
                                                })->size();

  SolverState state;
  mt19937 rng(20210626);

  if (max_len <= 6) {
    while (!timed_out()) {
      vector<string> order = work_hints;
      shuffle(order.begin(), order.end(), rng);
      state.consider(adaptive_greedy(std::move(order), rng() & 1));
    }
  } else {
    state.consider(ordered_greedy(work_hints, 1));
    state.consider(ordered_greedy(work_hints, 0));
    hill_climb(state, rng);
    while (!timed_out()) {
      vector<string> order = work_hints;
      shuffle_length_groups(order, rng);
      state.consider(ordered_greedy(std::move(order), rng() & 1));
    }
  }
  state.consider(adaptive_greedy(work_hints, 1));

  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c)
      cout << state.best_grid.at(r, c);
    cout << '\n';
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n_in, m_in;
  cin >> n_in >> m_in;
  (void)n_in;
  original_hints.resize(m_in);
  for (int i = 0; i < m_in; ++i)
    cin >> original_hints[i];

  t0 = chrono::steady_clock::now();
  solve();
  return 0;
}

// IOI 2010 Maze — simulated annealing (maomao90 / CF blog entry 118853).
//
// Neighbour: pick a random corn cell (non-corner, non-X); toggle # <-> . on the
// interior, or relocate the unique perimeter entrance on the boundary.
// Energy: max BFS distance in cells from the entrance ('.' on the border).
// Cooling: t0 = 2.5, alpha = 0.999999999, Boltzmann acceptance.
//
// Usage:  maze_sa <input> <output> [max_iters] [--seed N] [--warm path]
//   max_iters omitted => run until killed (blog style; saves best every 2^23 steps).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 205;
constexpr int k_max_cells = k_max_n * k_max_n;
constexpr double k_t0 = 2.5;
constexpr double k_alpha = 0.999999999;
constexpr int k_save_mask = (1 << 23) - 1;

const int k_di[4] = {1, 0, -1, 0};
const int k_dj[4] = {0, 1, 0, -1};

int n, m;
char g[k_max_n][k_max_n];
char bestg[k_max_n][k_max_n];
char input_mask[k_max_n][k_max_n];

int dist[k_max_n][k_max_n];
unsigned short visit_stamp[k_max_n][k_max_n];
unsigned short visit_gen = 1;

int bfs_q_r[k_max_cells];
int bfs_q_c[k_max_cells];

vector<pair<int, int>> sides;
vector<pair<int, int>> all;
pair<int, int> src = {-1, -1};
pair<int, int> best_src = {-1, -1};

mt19937 rng(0);
uniform_int_distribution<int> pick_all;
uniform_int_distribution<int> pick_side;
uniform_real_distribution<double> pick_real;

bool is_corner(int i, int j) {
  return (i == 0 && j == 0) || (i == 0 && j == m - 1) || (i == n - 1 && j == 0) ||
         (i == n - 1 && j == m - 1);
}

bool is_perimeter(int i, int j) {
  return i == 0 || i == n - 1 || j == 0 || j == m - 1;
}

void build_masks() {
  sides.clear();
  all.clear();

  for (int i = 1; i < n - 1; ++i) {
    for (int j : {0, m - 1}) {
      if (input_mask[i][j] == '#') {
        sides.emplace_back(i, j);
      }
    }
  }
  for (int j = 1; j < m - 1; ++j) {
    for (int i : {0, n - 1}) {
      if (input_mask[i][j] == '#') {
        sides.emplace_back(i, j);
      }
    }
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      if (is_corner(i, j)) {
        continue;
      }
      if (input_mask[i][j] != '#') {
        continue;
      }
      all.emplace_back(i, j);
    }
  }

  pick_all = uniform_int_distribution<int>(0, static_cast<int>(all.size()) - 1);
  pick_side = uniform_int_distribution<int>(0, static_cast<int>(sides.size()) - 1);
}

int calc_energy() {
  ++visit_gen;
  if (visit_gen == 0) {
    memset(visit_stamp, 0, sizeof visit_stamp);
    visit_gen = 1;
  }

  const int si = src.first;
  const int sj = src.second;
  if (g[si][sj] != '.') {
    return 0;
  }

  int head = 0;
  int tail = 0;
  bfs_q_r[tail] = si;
  bfs_q_c[tail] = sj;
  ++tail;
  visit_stamp[si][sj] = visit_gen;
  dist[si][sj] = 1;
  int res = 1;

  while (head < tail) {
    const int i = bfs_q_r[head];
    const int j = bfs_q_c[head];
    ++head;
    const int d0 = dist[i][j];
    res = max(res, d0);
    for (int k = 0; k < 4; ++k) {
      const int ni = i + k_di[k];
      const int nj = j + k_dj[k];
      if (ni < 0 || ni >= n || nj < 0 || nj >= m) {
        continue;
      }
      if (g[ni][nj] != '.') {
        continue;
      }
      if (visit_stamp[ni][nj] == visit_gen) {
        continue;
      }
      visit_stamp[ni][nj] = visit_gen;
      dist[ni][nj] = d0 + 1;
      bfs_q_r[tail] = ni;
      bfs_q_c[tail] = nj;
      ++tail;
    }
  }
  return res;
}

bool accept(int new_energy, int old_energy, double t) {
  if (new_energy >= old_energy) {
    return true;
  }
  return pick_real(rng) < exp((new_energy - old_energy) / t);
}

void write_grid(const char grid[k_max_n][k_max_n], const pair<int, int>& entrance,
                const char* path) {
  ofstream fout(path);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      if (input_mask[i][j] == 'X') {
        fout << 'X';
        continue;
      }
      if (input_mask[i][j] != '#') {
        continue;
      }
      char ch = grid[i][j];
      if (is_perimeter(i, j)) {
        const bool is_ent = (i == entrance.first && j == entrance.second);
        ch = is_ent ? '.' : '#';
      }
      fout << ch;
    }
    fout << '\n';
  }
}

void random_initial_state() {
  if (sides.empty()) {
    fprintf(stderr, "no valid entrance cells\n");
    exit(1);
  }

  src = sides[pick_side(rng)];
  g[src.first][src.second] = '.';

  uniform_int_distribution<int> coin(0, 9);
  for (int i = 1; i < n - 1; ++i) {
    for (int j = 1; j < m - 1; ++j) {
      if (input_mask[i][j] != '#') {
        continue;
      }
      g[i][j] = (coin(rng) < 7) ? '.' : '#';
    }
  }
}

void load_warm(const char* path) {
  ifstream fin(path);
  if (!fin) {
    return;
  }
  string line;
  int row = 0;
  while (getline(fin, line) && row < n) {
    for (int j = 0; j < m && j < static_cast<int>(line.size()); ++j) {
      g[row][j] = line[j];
      if (g[row][j] == '.' && is_perimeter(row, j)) {
        src = {row, j};
      }
    }
    ++row;
  }
}

bool valid_warm() {
  if (src.first < 0) {
    return false;
  }
  int perimeter_dots = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      if (input_mask[i][j] == 'X' && g[i][j] != 'X') {
        return false;
      }
      if (g[i][j] == '.' && is_perimeter(i, j)) {
        ++perimeter_dots;
      }
    }
  }
  return perimeter_dots == 1;
}

void read_input(const char* path) {
  ifstream fin(path);
  if (!fin) {
    perror(path);
    exit(1);
  }
  string line;
  while (getline(fin, line)) {
    while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
      line.pop_back();
    }
    if (line.empty()) {
      continue;
    }
    if (n >= k_max_n) {
      break;
    }
    m = static_cast<int>(line.size());
    for (int j = 0; j < m; ++j) {
      input_mask[n][j] = line[j];
      g[n][j] = line[j];
    }
    ++n;
  }
}

void save_best() {
  for (int i = 0; i < n; ++i) {
    memcpy(bestg[i], g[i], m);
  }
}

} // namespace

int main(int argc, char** argv) {
  if (argc < 3) {
    fprintf(stderr, "usage: %s <input> <output> [max_iters] [--seed N] [--warm path]\n", argv[0]);
    return 1;
  }

  const char* in_path = argv[1];
  const char* out_path = argv[2];
  long long max_iters = -1;
  const char* warm_path = nullptr;
  unsigned seed =
      static_cast<unsigned>(chrono::high_resolution_clock::now().time_since_epoch().count());

  for (int i = 3; i < argc; ++i) {
    if (string(argv[i]) == "--warm" && i + 1 < argc) {
      warm_path = argv[++i];
    } else if (string(argv[i]) == "--seed" && i + 1 < argc) {
      seed = static_cast<unsigned>(strtoul(argv[++i], nullptr, 10));
    } else {
      max_iters = atoll(argv[i]);
    }
  }

  rng.seed(seed);
  read_input(in_path);
  build_masks();

  bool warmed = false;
  if (warm_path) {
    load_warm(warm_path);
    warmed = valid_warm();
  }
  if (!warmed) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        g[i][j] = input_mask[i][j];
      }
    }
    src = {-1, -1};
    random_initial_state();
  }

  double t = k_t0;
  int cur_energy = calc_energy();
  int best_energy = cur_energy;
  best_src = src;
  save_best();

  unsigned int cnt = 0;
  const auto t_start = chrono::steady_clock::now();

  while (max_iters < 0 || static_cast<long long>(cnt) < max_iters) {
    if ((cnt & k_save_mask) == 0) {
      fprintf(stderr, "%.12g %d\n", t, cur_energy);
      write_grid(bestg, best_src, out_path);
    }
    ++cnt;

    const int id = pick_all(rng);
    const int ci = all[id].first;
    const int cj = all[id].second;
    const auto old_src = src;

    if (is_perimeter(ci, cj)) {
      g[old_src.first][old_src.second] = '#';
      if (old_src.first == ci && old_src.second == cj) {
        src = sides[pick_side(rng)];
      } else {
        src = {ci, cj};
      }
      g[src.first][src.second] = '.';
    } else {
      g[ci][cj] = (g[ci][cj] == '#') ? '.' : '#';
    }

    const int new_energy = calc_energy();
    if (new_energy >= best_energy) {
      best_energy = new_energy;
      best_src = src;
      save_best();
    }

    if (accept(new_energy, cur_energy, t)) {
      cur_energy = new_energy;
    } else {
      if (is_perimeter(ci, cj)) {
        g[src.first][src.second] = '#';
        g[old_src.first][old_src.second] = '.';
        src = old_src;
      } else {
        g[ci][cj] = (g[ci][cj] == '#') ? '.' : '#';
      }
    }

    t *= k_alpha;
  }

  write_grid(bestg, best_src, out_path);
  const auto elapsed =
      chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - t_start);
  fprintf(stderr, "best_energy=%d  iters=%u  elapsed_ms=%lld  seed=%u  -> %s\n", best_energy, cnt,
          static_cast<long long>(elapsed.count()), seed, out_path);
  return 0;
}

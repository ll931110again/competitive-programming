// IOI 2010 Maze — simulated annealing (maomao90 / CF blog entry 118853).
//
// Neighbour: pick a random corn cell (non-corner, non-X); toggle # <-> . on the
// interior, or relocate the unique perimeter entrance on the boundary.
// Energy: max BFS distance in cells from the entrance ('.' on the border).
// Cooling: t0 = 2.5, alpha = 0.999999999, Boltzmann acceptance.
//
// Usage:  maze_sa <input> <output> [max_iters]
//   max_iters omitted => run until killed (blog style; saves best every 2^23 steps).

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <random>
#include <string>
#include <vector>

namespace {

constexpr int kMaxN = 205;
constexpr double kT0 = 2.5;
constexpr double kAlpha = 0.999999999;
constexpr int kSaveMask = (1 << 23) - 1;

const int kDi[4] = {1, 0, -1, 0};
const int kDj[4] = {0, 1, 0, -1};

int n, m;
char g[kMaxN][kMaxN];
char bestg[kMaxN][kMaxN];
char input_mask[kMaxN][kMaxN]; // '#', 'X', or 0

int dist[kMaxN][kMaxN];
std::vector<std::pair<int, int>> sides;
std::vector<std::pair<int, int>> all;
std::pair<int, int> src = {-1, -1};
std::pair<int, int> best_src = {-1, -1};

std::mt19937 rng(
    static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

bool is_corner(int i, int j) {
  return (i == 0 && j == 0) || (i == 0 && j == m - 1) || (i == n - 1 && j == 0) ||
         (i == n - 1 && j == m - 1);
}

bool is_perimeter(int i, int j) {
  return i == 0 || i == n - 1 || j == 0 || j == m - 1;
}

bool is_strict_interior(int i, int j) {
  return i >= 1 && i <= n - 2 && j >= 1 && j <= m - 2;
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
}

int calc_energy() {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      dist[i][j] = -1;
    }
  }

  const int si = src.first;
  const int sj = src.second;
  if (g[si][sj] != '.') {
    return 0;
  }

  std::queue<std::pair<int, int>> q;
  dist[si][sj] = 1;
  q.emplace(si, sj);
  int res = 1;

  while (!q.empty()) {
    const auto [i, j] = q.front();
    q.pop();
    const int d0 = dist[i][j];
    res = std::max(res, d0);
    for (int k = 0; k < 4; ++k) {
      const int ni = i + kDi[k];
      const int nj = j + kDj[k];
      if (ni < 0 || ni >= n || nj < 0 || nj >= m) {
        continue;
      }
      if (g[ni][nj] != '.') {
        continue;
      }
      if (dist[ni][nj] != -1) {
        continue;
      }
      dist[ni][nj] = d0 + 1;
      q.emplace(ni, nj);
    }
  }
  return res;
}

bool accept(int new_energy, int old_energy, double t) {
  if (new_energy >= old_energy) {
    return true;
  }
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  return dist(rng) < std::exp((new_energy - old_energy) / t);
}

void write_grid(const char grid[kMaxN][kMaxN], const std::pair<int, int>& entrance,
                const char* path) {
  std::ofstream fout(path);
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
    std::fprintf(stderr, "no valid entrance cells\n");
    std::exit(1);
  }

  std::uniform_int_distribution<int> side_dist(0, static_cast<int>(sides.size()) - 1);
  src = sides[side_dist(rng)];
  g[src.first][src.second] = '.';

  std::uniform_int_distribution<int> coin(0, 9);
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
  std::ifstream fin(path);
  if (!fin) {
    return;
  }
  std::string line;
  int row = 0;
  while (std::getline(fin, line) && row < n) {
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
  std::ifstream fin(path);
  if (!fin) {
    std::perror(path);
    std::exit(1);
  }
  std::string line;
  while (std::getline(fin, line)) {
    while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
      line.pop_back();
    }
    if (line.empty()) {
      continue;
    }
    if (n >= kMaxN) {
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

} // namespace

int main(int argc, char** argv) {
  if (argc < 3) {
    std::fprintf(stderr, "usage: %s <input> <output> [max_iters] [--warm path]\n", argv[0]);
    return 1;
  }

  const char* in_path = argv[1];
  const char* out_path = argv[2];
  long long max_iters = -1;
  const char* warm_path = nullptr;

  for (int i = 3; i < argc; ++i) {
    if (std::string(argv[i]) == "--warm" && i + 1 < argc) {
      warm_path = argv[++i];
    } else {
      max_iters = std::atoll(argv[i]);
    }
  }

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

  double t = kT0;
  int cur_energy = calc_energy();
  int best_energy = cur_energy;
  best_src = src;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      bestg[i][j] = g[i][j];
    }
  }

  unsigned int cnt = 0;
  const auto t_start = std::chrono::steady_clock::now();

  while (max_iters < 0 || static_cast<long long>(cnt) < max_iters) {
    if ((cnt & kSaveMask) == 0) {
      std::fprintf(stderr, "%.12g %d\n", t, cur_energy);
      write_grid(bestg, best_src, out_path);
    }
    ++cnt;

    std::uniform_int_distribution<int> pick(0, static_cast<int>(all.size()) - 1);
    const int id = pick(rng);
    const int ci = all[id].first;
    const int cj = all[id].second;
    const auto old_src = src;

    if (is_perimeter(ci, cj)) {
      g[old_src.first][old_src.second] = '#';
      if (old_src.first == ci && old_src.second == cj) {
        std::uniform_int_distribution<int> side_pick(0, static_cast<int>(sides.size()) - 1);
        src = sides[side_pick(rng)];
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
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
          bestg[i][j] = g[i][j];
        }
      }
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

    t *= kAlpha;
  }

  write_grid(bestg, best_src, out_path);
  const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::steady_clock::now() - t_start);
  std::fprintf(stderr, "best_energy=%d  iters=%u  elapsed_ms=%lld  -> %s\n", best_energy, cnt,
               static_cast<long long>(elapsed.count()), out_path);
  return 0;
}

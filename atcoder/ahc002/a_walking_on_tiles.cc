// AHC002 A - Walking on Tiles: https://atcoder.jp/contests/ahc002/tasks/ahc002_a
//
// Tile-visit path on 50×50. Phase 1: time-limited DFS for a strong initial path.
// Phase 2: simulated annealing with splice-and-reconnect neighborhood (kmyk / tourist
// style): cut at a random prefix, random-walk extension, merge back when hitting the
// old suffix.

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int N = 50;
constexpr int NN = N * N;
constexpr int DR[4] = {-1, 1, 0, 0};
constexpr int DC[4] = {0, 0, -1, 1};
constexpr char MV[4] = {'U', 'D', 'L', 'R'};
constexpr auto kTimeLimit = chrono::milliseconds(1950);

struct XorShift128 {
  uint32_t a, b, c, d;
  explicit XorShift128(uint32_t seed = 42) {
    a = seed = 1812433253u * (seed ^ (seed >> 30));
    b = seed = 1812433253u * (seed ^ (seed >> 30)) + 1;
    c = seed = 1812433253u * (seed ^ (seed >> 30)) + 2;
    d = seed = 1812433253u * (seed ^ (seed >> 30)) + 3;
  }
  uint32_t next() {
    uint32_t t = a ^ (a << 11);
    a = b;
    b = c;
    c = d;
    return d = (d ^ (d >> 19)) ^ (t ^ (t >> 8));
  }
  int randint(int lo, int hi) {
    return lo + (int)(next() % (uint32_t)(hi - lo + 1));
  }
  bool bernoulli(double p) { return (next() >> 8) < (uint32_t)(p * (1 << 24)); }
};

int si, sj, M;
int tile_id[N][N];
int score_at[N][N];
vector<int> adj[NN];

inline int enc(int r, int c) { return r * N + c; }
inline pair<int, int> dec(int v) { return {v / N, v % N}; }

void read_input() {
  cin >> si >> sj;
  M = 0;
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j) {
      cin >> tile_id[i][j];
      M = max(M, tile_id[i][j] + 1);
    }
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      cin >> score_at[i][j];

  for (int r = 0; r < N; ++r)
    for (int c = 0; c < N; ++c) {
      const int u = enc(r, c);
      adj[u].clear();
      for (int d = 0; d < 4; ++d) {
        const int nr = r + DR[d], nc = c + DC[d];
        if (nr < 0 || nr >= N || nc < 0 || nc >= N)
          continue;
        if (tile_id[nr][nc] != tile_id[r][c])
          adj[u].push_back(enc(nr, nc));
      }
    }
}

string path_to_moves(const vector<uint16_t>& path) {
  string out;
  out.reserve(path.size());
  for (int i = 1; i < (int)path.size(); ++i) {
    auto [r0, c0] = dec(path[i - 1]);
    auto [r1, c1] = dec(path[i]);
    if (r1 == r0 - 1 && c1 == c0)
      out.push_back('U');
    else if (r1 == r0 + 1 && c1 == c0)
      out.push_back('D');
    else if (r1 == r0 && c1 == c0 + 1)
      out.push_back('R');
    else if (r1 == r0 && c1 == c0 - 1)
      out.push_back('L');
    else
      return {};
  }
  return out;
}

vector<uint16_t> dfs_init(XorShift128& rng, chrono::steady_clock::time_point deadline) {
  const int start = enc(si, sj);
  vector<uint16_t> best = {static_cast<uint16_t>(start)};
  int best_sc = score_at[si][sj];

  vector<int> used_tile(M, 0);
  vector<uint16_t> cur = {static_cast<uint16_t>(start)};
  used_tile[tile_id[si][sj]] = 1;

  function<void(int, int)> dfs = [&](int pos, int sc) {
    if (chrono::steady_clock::now() >= deadline)
      return;
    if (sc > best_sc) {
      best_sc = sc;
      best = cur;
    }
    vector<int> nbr = adj[pos];
    for (int i = (int)nbr.size() - 1; i > 0; --i) {
      const int j = rng.randint(0, i);
      swap(nbr[i], nbr[j]);
    }
    for (int np : nbr) {
      auto [r, c] = dec(np);
      const int tid = tile_id[r][c];
      if (used_tile[tid])
        continue;
      used_tile[tid] = 1;
      cur.push_back(static_cast<uint16_t>(np));
      dfs(np, sc + score_at[r][c]);
      cur.pop_back();
      used_tile[tid] = 0;
    }
  };
  dfs(start, score_at[si][sj]);
  return best;
}

vector<uint16_t> sa_splice(XorShift128& rng, chrono::steady_clock::time_point t_begin,
                           chrono::steady_clock::time_point t_end,
                           vector<uint16_t> path_prev) {
  vector<int16_t> used_tile_prev(M, INT16_MAX);
  array<bool, NN> used_pos_prev{};
  vector<int> score_prev;
  score_prev.push_back(0);
  for (int i = 0; i < (int)path_prev.size(); ++i) {
    auto [r, c] = dec(path_prev[i]);
    used_tile_prev[tile_id[r][c]] = i;
    used_pos_prev[enc(r, c)] = true;
    score_prev.push_back(score_prev.back() + score_at[r][c]);
  }

  vector<uint16_t> result = path_prev;
  int highscore = score_prev.back();

  int64_t iteration = 0;
  double temperature = 1.0;
  while (true) {
    if ((iteration & 63) == 0) {
      const double rem = chrono::duration<double>(t_end - chrono::steady_clock::now()).count();
      const double total = chrono::duration<double>(t_end - t_begin).count();
      if (rem <= 0)
        break;
      temperature = rem / total;
    }

    const int start = rng.randint(1, (int)path_prev.size());
    vector<char> used_tile_next(M, 0);
    auto tile_used = [&](int r, int c) {
      const int tid = tile_id[r][c];
      return used_tile_prev[tid] < start || used_tile_next[tid];
    };

    vector<uint16_t> diff;
    int score_next = score_prev[start];
    auto [y, x] = dec(path_prev[start - 1]);

    while (true) {
      array<int, 4> dirs = {0, 1, 2, 3};
      for (int i = 3; i > 0; --i)
        swap(dirs[i], dirs[rng.randint(0, i)]);
      bool found = false;
      for (int d : dirs) {
        const int ny = y + DR[d], nx = x + DC[d];
        if (ny < 0 || ny >= N || nx < 0 || nx >= N)
          continue;
        if (diff.empty() && start < (int)path_prev.size() &&
            path_prev[start] == enc(ny, nx))
          continue;
        if (!tile_used(ny, nx)) {
          found = true;
          diff.push_back(static_cast<uint16_t>(enc(ny, nx)));
          y = ny;
          x = nx;
          used_tile_next[tile_id[y][x]] = 1;
          score_next += score_at[y][x];
          break;
        }
      }
      if (!found)
        break;
      if (used_pos_prev[enc(y, x)])
        break;
    }

    if (!diff.empty()) {
      int tail_first = (int)path_prev.size();
      int tail_last = (int)path_prev.size();
      if (used_pos_prev[enc(y, x)]) {
        tail_first = start;
        while (tail_first < (int)path_prev.size() && path_prev[tail_first] != enc(y, x))
          ++tail_first;
        if (tail_first < (int)path_prev.size()) {
          ++tail_first;
          for (int i = tail_first; i < (int)path_prev.size(); ++i) {
            auto [yr, xr] = dec(path_prev[i]);
            if (tile_used(yr, xr)) {
              tail_last = i;
              break;
            }
            used_tile_next[tile_id[yr][xr]] = 1;
          }
        }
      }
      score_next += score_prev[tail_last] - score_prev[tail_first];

      const int delta = score_next - score_prev.back();
      const double accept_p = exp(0.01 * delta / max(temperature, 1e-9));
      if (delta >= 0 || rng.bernoulli(accept_p)) {
        diff.insert(diff.end(), path_prev.begin() + tail_first, path_prev.begin() + tail_last);
        path_prev.resize(start);
        path_prev.insert(path_prev.end(), diff.begin(), diff.end());

        used_tile_prev.assign(M, INT16_MAX);
        used_pos_prev = {};
        score_prev.clear();
        score_prev.push_back(0);
        for (int i = 0; i < (int)path_prev.size(); ++i) {
          auto [r, c] = dec(path_prev[i]);
          used_tile_prev[tile_id[r][c]] = i;
          used_pos_prev[enc(r, c)] = true;
          score_prev.push_back(score_prev.back() + score_at[r][c]);
        }
        if (score_prev.back() > highscore) {
          highscore = score_prev.back();
          result = path_prev;
        }
      }
    }
    ++iteration;
  }
  return result;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  read_input();

  XorShift128 rng(20210425);
  const auto t_begin = chrono::steady_clock::now();
  const auto t_end = t_begin + kTimeLimit;
  const auto dfs_deadline = t_begin + chrono::milliseconds(400);

  vector<uint16_t> path = dfs_init(rng, dfs_deadline);
  path = sa_splice(rng, t_begin + chrono::milliseconds(400), t_end, std::move(path));

  cout << path_to_moves(path) << '\n';
  return 0;
}

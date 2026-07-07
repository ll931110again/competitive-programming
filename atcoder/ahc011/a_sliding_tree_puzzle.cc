// AHC011 A - Sliding Tree Puzzle: https://atcoder.jp/contests/ahc011/tasks/ahc011_a
//
// DFS for a grid spanning tree matching input tile masks, then beam search and
// distance-greedy slides toward that assignment.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int MAXN = 10;
constexpr int DR[4] = {-1, 1, 0, 0};
constexpr int DC[4] = {0, 0, -1, 1};
constexpr char DIR[4] = {'U', 'D', 'L', 'R'};
constexpr int OPP[4] = {1, 0, 3, 2};
constexpr int BIT[4] = {2, 8, 1, 4};

int N, T, NN;
int init_tile[MAXN][MAXN];
int cur_mask[MAXN][MAXN];
bool in_tree[MAXN][MAXN];
int target_pos[MAXN * MAXN];
int cell_tile[MAXN * MAXN];
int target_er, target_ec;
int tile_mask[MAXN * MAXN];
int have_cnt[16];
int rem_cnt[16];
bool used_tile[MAXN * MAXN];

struct Timer {
  chrono::steady_clock::time_point st =
      chrono::steady_clock::now();
  double elapsed() const {
    return chrono::duration<double>(chrono::steady_clock::now() - st)
        .count();
  }
} timer;

mt19937 rng(712367);

int id(int r, int c) {
  return r * N + c;
}

pair<int, int> rc(int p) {
  return {p / N, p % N};
}

int tree_size(const int g[MAXN][MAXN]) {
  int uf[MAXN * MAXN], sz[MAXN * MAXN];
  bool is_root[MAXN * MAXN], ok[MAXN * MAXN];
  iota(uf, uf + NN, 0);
  fill(sz, sz + NN, 1);
  fill(is_root, is_root + NN, true);
  fill(ok, ok + NN, true);
  auto find = [&](auto &&self, int x) -> int {
    if (uf[x] == x)
      return x;
    return uf[x] = self(self, uf[x]);
  };
  auto unite = [&](int a, int b) {
    a = find(find, a);
    b = find(find, b);
    if (a == b) {
      ok[a] = false;
      return;
    }
    if (sz[a] < sz[b])
      swap(a, b);
    sz[a] += sz[b];
    uf[b] = a;
    is_root[b] = false;
    ok[a] = ok[a] && ok[b];
  };
  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c) {
      if (g[r][c] == 0)
        continue;
      if (r + 1 < N && g[r + 1][c] && (g[r][c] & 8) && (g[r + 1][c] & 2))
        unite(id(r, c), id(r + 1, c));
      if (c + 1 < N && g[r][c + 1] && (g[r][c] & 4) && (g[r][c + 1] & 1))
        unite(id(r, c), id(r, c + 1));
    }
  }
  int best = 0;
  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c) {
      if (g[r][c] == 0)
        continue;
      const int p = id(r, c);
      const int rt = find(find, p);
      if (is_root[rt] && ok[rt])
        best = max(best, sz[rt]);
    }
  }
  return best;
}

bool dfs_grow(int er0, int ec0, int placed) {
  if (placed == NN - 1) {
    for (int r = 0; r < N; ++r) {
      for (int c = 0; c < N; ++c) {
        if (!in_tree[r][c] || (r == er0 && c == ec0))
          continue;
        const int p = id(r, c);
        if (cur_mask[r][c] != tile_mask[cell_tile[p]])
          return false;
      }
    }
    for (int m = 0; m < 16; ++m) {
      if (rem_cnt[m] != 0)
        return false;
    }
    return true;
  }
  if (timer.elapsed() > 1.15)
    return false;

  vector<pair<int, int>> frontier;
  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c) {
      if (!in_tree[r][c] || (r == er0 && c == ec0))
        continue;
      for (int d = 0; d < 4; ++d) {
        const int nr = r + DR[d], nc = c + DC[d];
        if (nr < 0 || nr >= N || nc < 0 || nc >= N)
          continue;
        if (in_tree[nr][nc] || (nr == er0 && nc == ec0))
          continue;
        frontier.emplace_back(nr, nc);
      }
    }
  }
  sort(frontier.begin(), frontier.end());
  frontier.erase(unique(frontier.begin(), frontier.end()), frontier.end());
  shuffle(frontier.begin(), frontier.end(), rng);

  for (const auto [cr, cc] : frontier) {
    vector<pair<int, int>> nbs;
    for (int d = 0; d < 4; ++d) {
      const int nr = cr + DR[d], nc = cc + DC[d];
      if (nr < 0 || nr >= N || nc < 0 || nc >= N)
        continue;
      if (in_tree[nr][nc] && !(nr == er0 && nc == ec0))
        nbs.emplace_back(nr, nc);
    }
    shuffle(nbs.begin(), nbs.end(), rng);
    for (const auto [pr, pc] : nbs) {
      int pd = -1;
      for (int d = 0; d < 4; ++d) {
        if (cr + DR[d] == pr && cc + DC[d] == pc) {
          pd = d;
          break;
        }
      }
      if (pd < 0)
        continue;
      const int need_bit = BIT[pd];
      for (int tid = 0; tid < NN; ++tid) {
        if (tid == id(er0, ec0))
          continue;
        const int m = tile_mask[tid];
        if (used_tile[tid] || rem_cnt[m] <= 0 || ((m & need_bit) == 0))
          continue;
        if ((cur_mask[cr][cc] | need_bit) & ~m)
          continue;
        if ((cur_mask[pr][pc] | BIT[OPP[pd]]) & ~tile_mask[cell_tile[id(pr, pc)]])
          continue;

        in_tree[cr][cc] = true;
        cell_tile[id(cr, cc)] = tid;
        target_pos[tid] = id(cr, cc);
        used_tile[tid] = true;
        rem_cnt[m]--;
        const int old_c = cur_mask[cr][cc];
        const int old_p = cur_mask[pr][pc];
        cur_mask[cr][cc] = old_c | need_bit;
        cur_mask[pr][pc] = old_p | BIT[OPP[pd]];

        if (dfs_grow(er0, ec0, placed + 1))
          return true;

        cur_mask[pr][pc] = old_p;
        cur_mask[cr][cc] = old_c;
        rem_cnt[m]++;
        used_tile[tid] = false;
        in_tree[cr][cc] = false;
      }
    }
  }
  return false;
}

bool find_tree_layout(int er0, int ec0) {
  memset(cur_mask, 0, sizeof(cur_mask));
  memset(in_tree, 0, sizeof(in_tree));
  memset(cell_tile, -1, sizeof(cell_tile));
  memset(used_tile, 0, sizeof(used_tile));
  target_er = er0;
  target_ec = ec0;
  in_tree[er0][ec0] = true;
  memcpy(rem_cnt, have_cnt, sizeof(have_cnt));

  vector<pair<int, int>> cells;
  for (int r = 0; r < N; ++r)
    for (int c = 0; c < N; ++c)
      if (!(r == er0 && c == ec0))
        cells.emplace_back(r, c);
  shuffle(cells.begin(), cells.end(), rng);

  for (const auto [sr, sc] : cells) {
    for (int tid = 0; tid < NN; ++tid) {
      if (tid == id(er0, ec0))
        continue;
      const int m = tile_mask[tid];
      if (rem_cnt[m] <= 0)
        continue;
      if (__builtin_popcount(m) == 0)
        continue;

      memset(cur_mask, 0, sizeof(cur_mask));
      memset(in_tree, 0, sizeof(in_tree));
      memset(cell_tile, -1, sizeof(cell_tile));
  memset(used_tile, 0, sizeof(used_tile));
      in_tree[er0][ec0] = true;
      memcpy(rem_cnt, have_cnt, sizeof(have_cnt));

      in_tree[sr][sc] = true;
      cell_tile[id(sr, sc)] = tid;
      target_pos[tid] = id(sr, sc);
      used_tile[tid] = true;
      rem_cnt[m]--;
      if (dfs_grow(er0, ec0, 1))
        return true;
      rem_cnt[m]++;
    }
  }
  return false;
}

i64 dist_cost(const int at[MAXN * MAXN]) {
  i64 h = 0;
  for (int p = 0; p < NN; ++p) {
    const int t = at[p];
    if (t < 0)
      continue;
    const auto [r, c] = rc(p);
    const auto [tr, tc] = rc(target_pos[t]);
    const int dr = r - tr, dc = c - tc;
    h += 1LL * dr * dr + 1LL * dc * dc;
  }
  return h;
}

bool solved(const int at[MAXN * MAXN], int er0, int ec0) {
  if (er0 != target_er || ec0 != target_ec)
    return false;
  for (int p = 0; p < NN; ++p) {
    const int t = at[p];
    if (t < 0)
      continue;
    if (target_pos[t] != p)
      return false;
  }
  return true;
}

struct Node {
  int at[MAXN * MAXN];
  int er, ec;
  i64 h;
  int prev;
  char mv;
};

string rebuild_ops(const vector<Node> &nodes, int idx) {
  string ops;
  for (int v = idx; nodes[v].prev >= 0; v = nodes[v].prev)
    ops.push_back(nodes[v].mv);
  reverse(ops.begin(), ops.end());
  return ops;
}

string greedy_distance(const int start_at[MAXN * MAXN], int ser, int sec,
                       double tl) {
  int at[MAXN * MAXN];
  memcpy(at, start_at, sizeof(at));
  int er = ser, ec = sec;
  string ops;
  int last = -1;
  while ((int)ops.size() < T && timer.elapsed() < tl) {
    if (solved(at, er, ec))
      break;
    int best_d = -1;
    i64 best_h = dist_cost(at);
    for (int d = 0; d < 4; ++d) {
      if (last >= 0 && d == (last ^ 1))
        continue;
      const int nr = er + DR[d], nc = ec + DC[d];
      if (nr < 0 || nr >= N || nc < 0 || nc >= N)
        continue;
      const int np = id(nr, nc);
      const int tile = at[np];
      at[id(er, ec)] = tile;
      at[np] = -1;
      const int oer = er, oec = ec;
      er = nr;
      ec = nc;
      const i64 h = dist_cost(at);
      if (h < best_h) {
        best_h = h;
        best_d = d;
      }
      at[id(oer, oec)] = -1;
      at[np] = tile;
      er = oer;
      ec = oec;
    }
    if (best_d < 0) {
      for (int d = 0; d < 4; ++d) {
        const int nr = er + DR[d], nc = ec + DC[d];
        if (nr < 0 || nr >= N || nc < 0 || nc >= N)
          continue;
        best_d = d;
        break;
      }
    }
    if (best_d < 0)
      break;
    const int nr = er + DR[best_d], nc = ec + DC[best_d];
    at[id(er, ec)] = at[id(nr, nc)];
    at[id(nr, nc)] = -1;
    er = nr;
    ec = nc;
    ops.push_back(DIR[best_d]);
    last = best_d;
  }
  return ops;
}

string beam_solve(const int start_at[MAXN * MAXN], int ser, int sec,
                  double tl) {
  const int W = N <= 7 ? 1000 : (N <= 8 ? 700 : 450);
  vector<Node> nodes;
  nodes.reserve(W * 512 + 8);
  Node st{};
  memcpy(st.at, start_at, sizeof(st.at));
  st.er = ser;
  st.ec = sec;
  st.h = dist_cost(st.at);
  st.prev = -1;
  st.mv = 0;
  nodes.push_back(st);
  vector<int> layer{0};
  vector<int> nlayer;
  string best_ops;
  i64 best_h = st.h;
  for (int depth = 0; depth < T && timer.elapsed() < tl; ++depth) {
    nlayer.clear();
    for (const int idx : layer) {
      const Node &nd = nodes[idx];
      if (solved(nd.at, nd.er, nd.ec))
        return rebuild_ops(nodes, idx);
      if (nd.h < best_h) {
        best_h = nd.h;
        best_ops = rebuild_ops(nodes, idx);
      }
      for (int d = 0; d < 4; ++d) {
        const int nr = nd.er + DR[d], nc = nd.ec + DC[d];
        if (nr < 0 || nr >= N || nc < 0 || nc >= N)
          continue;
        Node nx = nd;
        const int np = id(nr, nc);
        nx.at[id(nd.er, nd.ec)] = nx.at[np];
        nx.at[np] = -1;
        nx.er = nr;
        nx.ec = nc;
        nx.h = dist_cost(nx.at);
        nx.prev = idx;
        nx.mv = DIR[d];
        nlayer.push_back((int)nodes.size());
        nodes.push_back(nx);
      }
    }
    if (nlayer.empty())
      break;
    sort(nlayer.begin(), nlayer.end(),
         [&](int a, int b) { return nodes[a].h < nodes[b].h; });
    if ((int)nlayer.size() > W)
      nlayer.resize(W);
    layer.swap(nlayer);
  }
  if (!best_ops.empty())
    return best_ops;
  if (!layer.empty())
    return rebuild_ops(nodes, layer[0]);
  return string{};
}

void apply_ops_to_grid(const string &ops, int g[MAXN][MAXN], int &e0, int &e1) {
  for (const char ch : ops) {
    const int d = (ch == 'U' ? 0 : ch == 'D' ? 1 : ch == 'L' ? 2 : 3);
    const int nr = e0 + DR[d], nc = e1 + DC[d];
    g[e0][e1] = g[nr][nc];
    g[nr][nc] = 0;
    e0 = nr;
    e1 = nc;
  }
}

int eval_ops(const string &ops) {
  int g[MAXN][MAXN];
  memcpy(g, init_tile, sizeof(g));
  int e0 = 0, e1 = 0;
  for (int r = 0; r < N; ++r)
    for (int c = 0; c < N; ++c)
      if (g[r][c] == 0)
        e0 = r, e1 = c;
  apply_ops_to_grid(ops, g, e0, e1);
  const int S = tree_size(g);
  if (S == NN - 1)
    return (int)llround(500000.0 *
                        (1.0 + (T - (int)ops.size()) * 1.0 / T));
  return (int)llround(500000.0 * S / (NN - 1.0));
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> T;
  NN = N * N;
  memset(have_cnt, 0, sizeof(have_cnt));
  for (int r = 0; r < N; ++r) {
    string s;
    cin >> s;
    for (int c = 0; c < N; ++c) {
      const int v =
          s[c] <= '9' ? s[c] - '0' : s[c] - 'a' + 10;
      init_tile[r][c] = v;
      tile_mask[id(r, c)] = v;
      if (v)
        have_cnt[v]++;
    }
  }

  int start_at[MAXN * MAXN];
  int ser = 0, sec = 0;
  for (int p = 0; p < NN; ++p) {
    const auto [r, c] = rc(p);
    if (init_tile[r][c] == 0) {
      start_at[p] = -1;
      ser = r;
      sec = c;
    } else {
      start_at[p] = p;
    }
  }

  string best_ops;
  int best_score = -1;
  bool found = false;
  vector<pair<int, int>> empties{{N - 1, N - 1}};
  for (int r = 0; r < N; ++r)
    for (int c = 0; c < N; ++c)
      if (!(r == N - 1 && c == N - 1))
        empties.emplace_back(r, c);

  for (const auto [er0, ec0] : empties) {
    if (timer.elapsed() > 1.2)
      break;
    for (int t = 0; t < 8; ++t) {
      if (find_tree_layout(er0, ec0)) {
        found = true;
        break;
      }
    }
    if (found)
      break;
  }

  if (found) {
    const string beam = beam_solve(start_at, ser, sec, 2.55);
    const int sc = eval_ops(beam);
    if (sc > best_score) {
      best_score = sc;
      best_ops = beam;
    }
    const string dist = greedy_distance(start_at, ser, sec, 2.85);
    const int sc2 = eval_ops(dist);
    if (sc2 > best_score) {
      best_score = sc2;
      best_ops = dist;
    }
  }

  if (best_score < 0) {
    int g[MAXN][MAXN];
    memcpy(g, init_tile, sizeof(g));
    int e0 = ser, e1 = sec;
    while ((int)best_ops.size() < T) {
      int best = 0, best_s = -1;
      for (int d = 0; d < 4; ++d) {
        const int nr = e0 + DR[d], nc = e1 + DC[d];
        if (nr < 0 || nr >= N || nc < 0 || nc >= N)
          continue;
        const int tmp = g[e0][e1];
        g[e0][e1] = g[nr][nc];
        g[nr][nc] = tmp;
        const int s = tree_size(g);
        if (s > best_s)
          best_s = s, best = d;
        g[nr][nc] = g[e0][e1];
        g[e0][e1] = tmp;
      }
      const int nr = e0 + DR[best], nc = e1 + DC[best];
      g[e0][e1] = g[nr][nc];
      g[nr][nc] = 0;
      e0 = nr;
      e1 = nc;
      best_ops.push_back(DIR[best]);
      if (best_s == NN - 1)
        break;
    }
  }

  if ((int)best_ops.size() > T)
    best_ops.resize(T);
  cout << best_ops << '\n';
  return 0;
}

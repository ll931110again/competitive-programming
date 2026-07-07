// AHC067 A - Castle Renovation with Linked Doors
// https://atcoder.jp/contests/ahc067/tasks/ahc067_a
//
// Primary: exponential tree on throne-rooted BFS spanning tree — switches 0..8
// on distinct tree branches (0 near start, 8..1 toward throne), complementary
// split + ring gates. Switch 9 unused → door 19; seal all non-tree shortcuts.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int MAXN = 20;
constexpr int MAXK = 10;
constexpr int WALL_G = 2 * 9 + 1;
constexpr int dr[4] = {-1, 1, 0, 0};
constexpr int dc[4] = {0, 0, -1, 1};

struct Door {
  int orient;
  int i, j, g;
};

struct Switch {
  int i, j, k;
};

struct Plan {
  vector<Door> doors;
  vector<Switch> switches;
};

struct BranchAlcove {
  int path_index = 0;
  vector<pair<int, int>> cells;
};

int N, M, K;
vector<string> grid;

inline int enc(int r, int c) {
  return r * N + c;
}

bool walkable(int r, int c) {
  return r >= 0 && r < N && c >= 0 && c < N && grid[r][c] == '.';
}

bool door_open(int g, int mask) {
  if (g < 0)
    return true;
  return ((mask >> (g / 2)) & 1) == (g & 1);
}

struct HeroSimulator {
  int door_h[MAXN - 1][MAXN]{};
  int door_v[MAXN][MAXN - 1]{};
  int sw[MAXN][MAXN]{};

  explicit HeroSimulator(const Plan& plan) {
    for (int i = 0; i < N - 1; ++i)
      fill(door_h[i], door_h[i] + N, -1);
    for (int i = 0; i < N; ++i)
      fill(door_v[i], door_v[i] + N - 1, -1);
    for (int i = 0; i < N; ++i)
      fill(sw[i], sw[i] + N, -1);

    for (const Door& d : plan.doors) {
      if (d.orient == 0)
        door_h[d.i][d.j] = d.g;
      else
        door_v[d.i][d.j] = d.g;
    }
    for (const Switch& s : plan.switches)
      sw[s.i][s.j] = s.k;
  }

  int min_steps() const {
    const int states = 1 << K;
    static int dist[MAXN][MAXN][1 << MAXK];
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        fill(dist[i][j], dist[i][j] + states, -1);

    queue<tuple<int, int, int>> q;
    dist[0][0][0] = 0;
    q.emplace(0, 0, 0);

    while (!q.empty()) {
      auto [i, j, mask] = q.front();
      q.pop();
      const int d = dist[i][j][mask];
      if (i == N - 1 && j == N - 1)
        return d;

      const int sk = sw[i][j];
      if (sk >= 0) {
        const int nmask = mask ^ (1 << sk);
        if (dist[i][j][nmask] < 0) {
          dist[i][j][nmask] = d + 1;
          q.emplace(i, j, nmask);
        }
      }

      for (int t = 0; t < 4; ++t) {
        const int ni = i + dr[t], nj = j + dc[t];
        if (!walkable(ni, nj))
          continue;
        const int g = (i == ni) ? door_v[i][min(j, nj)] : door_h[min(i, ni)][j];
        if (!door_open(g, mask))
          continue;
        if (dist[ni][nj][mask] < 0) {
          dist[ni][nj][mask] = d + 1;
          q.emplace(ni, nj, mask);
        }
      }
    }
    return 0;
  }
};

i64 score_plan(const Plan& plan) {
  const int steps = HeroSimulator(plan).min_steps();
  if (steps <= 0)
    return 1;
  return llround(1e6 * log2((double)steps / N));
}

struct Edge {
  int r0, c0, r1, c1;

  bool operator<(const Edge& o) const {
    return tie(r0, c0, r1, c1) < tie(o.r0, o.c0, o.r1, o.c1);
  }
};

Edge normalize_edge(int r0, int c0, int r1, int c1) {
  if (tie(r0, c0) > tie(r1, c1))
    swap(r0, r1), swap(c0, c1);
  return {r0, c0, r1, c1};
}

void place_door(Plan& plan, int r0, int c0, int r1, int c1, int g, set<Edge>& used) {
  if ((int)plan.doors.size() >= M)
    return;
  const Edge e = normalize_edge(r0, c0, r1, c1);
  if (!used.insert(e).second)
    return;
  if (r0 == r1)
    plan.doors.push_back({1, r0, min(c0, c1), g});
  else
    plan.doors.push_back({0, min(r0, r1), c0, g});
}

vector<pair<int, int>> shortest_path(int sr, int sc, int tr, int tc) {
  static int parent[MAXN * MAXN];
  fill(parent, parent + N * N, -1);
  queue<int> q;
  const int s = enc(sr, sc), t = enc(tr, tc);
  parent[s] = s;
  q.push(s);
  while (!q.empty()) {
    const int u = q.front();
    q.pop();
    if (u == t)
      break;
    const int r = u / N, c = u % N;
    for (int k = 0; k < 4; ++k) {
      const int nr = r + dr[k], nc = c + dc[k];
      if (!walkable(nr, nc))
        continue;
      const int v = enc(nr, nc);
      if (parent[v] >= 0)
        continue;
      parent[v] = u;
      q.push(v);
    }
  }
  if (parent[t] < 0)
    return {};
  vector<pair<int, int>> path;
  for (int v = t; v != s; v = parent[v])
    path.emplace_back(v / N, v % N);
  path.emplace_back(sr, sc);
  reverse(path.begin(), path.end());
  return path;
}

vector<pair<int, int>> dfs_long_path(mt19937& rng, int ms_budget) {
  vector<pair<int, int>> best;
  const auto deadline = chrono::steady_clock::now() + chrono::milliseconds(ms_budget);

  while (chrono::steady_clock::now() < deadline) {
    vector<vector<char>> vis(N, vector<char>(N, 0));
    vector<pair<int, int>> path;
    function<void(int, int)> dfs = [&](int r, int c) {
      path.emplace_back(r, c);
      vis[r][c] = 1;
      if (r == N - 1 && c == N - 1) {
        if (path.size() > best.size())
          best = path;
      } else {
        array<int, 4> order = {0, 1, 2, 3};
        shuffle(order.begin(), order.end(), rng);
        for (int t : order) {
          const int nr = r + dr[t], nc = c + dc[t];
          if (walkable(nr, nc) && !vis[nr][nc])
            dfs(nr, nc);
        }
      }
      vis[r][c] = 0;
      path.pop_back();
    };
    dfs(0, 0);
  }
  return best;
}

vector<pair<int, int>> greedy_long_path(mt19937& rng) {
  vector<vector<char>> vis(N, vector<char>(N, 0));
  vector<pair<int, int>> path;
  int r = 0, c = 0;
  vis[r][c] = 1;
  path.emplace_back(r, c);
  while (!(r == N - 1 && c == N - 1)) {
    vector<tuple<int, int, int, int>> cand;
    for (int t = 0; t < 4; ++t) {
      const int nr = r + dr[t], nc = c + dc[t];
      if (!walkable(nr, nc) || vis[nr][nc])
        continue;
      int deg = 0;
      for (int u = 0; u < 4; ++u) {
        const int ar = nr + dr[u], ac = nc + dc[u];
        if (walkable(ar, ac) && !vis[ar][ac])
          deg++;
      }
      cand.emplace_back(-deg, abs(nr - (N - 1)) + abs(nc - (N - 1)), nr, nc);
    }
    if (cand.empty())
      break;
    sort(cand.begin(), cand.end());
    const int pick = rng() % min(3, (int)cand.size());
    tie(r, c) = pair{get<2>(cand[pick]), get<3>(cand[pick])};
    vis[r][c] = 1;
    path.emplace_back(r, c);
  }
  return (r == N - 1 && c == N - 1) ? path : vector<pair<int, int>>{};
}

vector<int> ring_switch_indices(int path_len, int num_sw, int spread_num, int spread_den) {
  vector<int> idx(num_sw);
  for (int k = 0; k < num_sw; ++k) {
    const i64 base = i64(k + 1) * (path_len - 2) / (num_sw + 1);
    idx[k] = 1 + (int)base + (int)(i64(k) * spread_num / spread_den);
  }
  for (int k = 1; k < num_sw; ++k)
    idx[k] = max(idx[k], idx[k - 1] + 2);
  for (int k = num_sw - 2; k >= 0; --k)
    idx[k] = min(idx[k], idx[k + 1] - 2);
  idx[0] = max(idx[0], 1);
  if (num_sw > 0)
    idx[num_sw - 1] = min(idx[num_sw - 1], path_len - 2);
  return idx;
}

bool valid_ring_indices(const vector<int>& idx, int path_len) {
  if (idx.empty() || idx.front() < 1 || idx.back() >= path_len - 1)
    return false;
  for (int k = 1; k < (int)idx.size(); ++k) {
    if (idx[k] <= idx[k - 1] + 1)
      return false;
  }
  return true;
}

void add_ring_doors(Plan& plan, const vector<pair<int, int>>& route,
                    const vector<int>& sw_idx, set<Edge>& used) {
  set<pair<int, int>> seen_sw;
  for (int k = 0; k < (int)sw_idx.size(); ++k) {
    const auto [sr, sc] = route[sw_idx[k]];
    if (!seen_sw.insert({sr, sc}).second)
      return;
    plan.switches.push_back({sr, sc, k});
    if (k >= 1) {
      const auto [r0, c0] = route[sw_idx[k] - 1];
      const auto [r1, c1] = route[sw_idx[k]];
      place_door(plan, r0, c0, r1, c1, 2 * (k - 1) + 1, used);
    }
    if (k >= 2) {
      const auto [r0, c0] = route[sw_idx[k] - 2];
      const auto [r1, c1] = route[sw_idx[k] - 1];
      place_door(plan, r0, c0, r1, c1, 2 * (k - 2), used);
    }
  }
}

void add_wall_doors(Plan& plan, const set<Edge>& path_edges, set<Edge>& used) {
  vector<pair<int, Edge>> cand;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (grid[i][j] != '.')
        continue;
      for (int t = 0; t < 4; ++t) {
        const int ni = i + dr[t], nj = j + dc[t];
        if (!walkable(ni, nj))
          continue;
        const Edge e = normalize_edge(i, j, ni, nj);
        if (path_edges.count(e))
          continue;
        const int pri = abs(i - (N - 1)) + abs(j - (N - 1)) + abs(ni - (N - 1)) + abs(nj - (N - 1));
        cand.emplace_back(-pri, e);
      }
    }
  }
  sort(cand.begin(), cand.end());
  for (const auto& [_, e] : cand) {
    if ((int)plan.doors.size() >= M)
      break;
    set<Edge> trial_used = used;
    Plan trial = plan;
    place_door(trial, e.r0, e.c0, e.r1, e.c1, WALL_G, trial_used);
    if (HeroSimulator(trial).min_steps() > 0) {
      plan = std::move(trial);
      used = std::move(trial_used);
    }
  }
}

set<Edge> collect_tree_edges(const vector<int>& parent) {
  set<Edge> edges;
  for (int v = 0; v < N * N; ++v) {
    if (parent[v] < 0 || parent[v] == v)
      continue;
    const int u = parent[v];
    edges.insert(normalize_edge(u / N, u % N, v / N, v % N));
  }
  return edges;
}

void seal_shortcuts(Plan& plan, const set<Edge>& reserved, set<Edge>& used,
                    int max_trials = 48) {
  static int dist0[MAXN * MAXN];
  fill(dist0, dist0 + N * N, -1);
  queue<int> q;
  dist0[0] = 0;
  q.push(0);
  while (!q.empty()) {
    const int u = q.front();
    q.pop();
    const int r = u / N, c = u % N;
    for (int t = 0; t < 4; ++t) {
      const int nr = r + dr[t], nc = c + dc[t];
      if (!walkable(nr, nc))
        continue;
      const int v = enc(nr, nc);
      if (dist0[v] >= 0)
        continue;
      dist0[v] = dist0[u] + 1;
      q.push(v);
    }
  }

  vector<pair<int, Edge>> cand;
  cand.reserve(N * N * 2);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (grid[i][j] != '.')
        continue;
      for (int t = 0; t < 4; ++t) {
        const int ni = i + dr[t], nj = j + dc[t];
        if (!walkable(ni, nj))
          continue;
        const Edge e = normalize_edge(i, j, ni, nj);
        if (reserved.count(e))
          continue;
        const int u = enc(i, j), v = enc(ni, nj);
        if (dist0[u] < 0 || dist0[v] < 0)
          continue;
        const int pri = dist0[u] + dist0[v];
        cand.emplace_back(pri, e);
      }
    }
  }
  sort(cand.begin(), cand.end());
  int trials = 0;
  for (const auto& [_, e] : cand) {
    if ((int)plan.doors.size() >= M || trials >= max_trials)
      break;
    ++trials;
    set<Edge> trial_used = used;
    Plan trial = plan;
    place_door(trial, e.r0, e.c0, e.r1, e.c1, WALL_G, trial_used);
    if (HeroSimulator(trial).min_steps() > 0) {
      plan = std::move(trial);
      used = std::move(trial_used);
    }
  }
}

Plan build_ring_plan(const vector<pair<int, int>>& route, int num_sw,
                     const vector<int>& sw_idx) {
  Plan plan;
  if (route.size() < 4 || num_sw < 1 || !valid_ring_indices(sw_idx, (int)route.size()))
    return plan;

  set<Edge> used, path_edges;
  for (int i = 0; i + 1 < (int)route.size(); ++i) {
    const auto [r0, c0] = route[i];
    const auto [r1, c1] = route[i + 1];
    path_edges.insert(normalize_edge(r0, c0, r1, c1));
  }

  add_ring_doors(plan, route, sw_idx, used);
  if (HeroSimulator(plan).min_steps() <= 0)
    return plan;
  add_wall_doors(plan, path_edges, used);
  return plan;
}

Plan best_ring_plan(const vector<pair<int, int>>& route) {
  Plan best;
  i64 best_score = 0;
  const int max_sw =
      min(min(K - 1, 9), (int)route.size() >= 50 ? 9 : 3);
  for (int num_sw = max_sw; num_sw >= 1; --num_sw) {
    for (int spread = 0; spread < 12; ++spread) {
      const vector<int> idx =
          ring_switch_indices((int)route.size(), num_sw, spread, 12);
      if (!valid_ring_indices(idx, (int)route.size()))
        continue;
      Plan plan = build_ring_plan(route, num_sw, idx);
      const i64 sc = score_plan(plan);
      if (sc > best_score) {
        best_score = sc;
        best = std::move(plan);
      }
    }
  }
  return best;
}

bool plan_is_strong(const Plan& plan) {
  if (HeroSimulator(plan).min_steps() <= 0)
    return false;
  const int sw = (int)plan.switches.size();
  if (sw >= 4) {
    const int steps = HeroSimulator(plan).min_steps();
    if (steps < N * sw * 2)
      return false;
  }
  return true;
}

vector<BranchAlcove> find_branch_alcoves(const vector<pair<int, int>>& route,
                                         int max_branches) {
  vector<vector<char>> on_path(N, vector<char>(N, 0));
  for (const auto [r, c] : route)
    on_path[r][c] = 1;

  vector<BranchAlcove> candidates;
  for (int i = 1; i + 1 < (int)route.size(); ++i) {
    const auto [pr, pc] = route[i];
    const auto [prev_r, prev_c] = route[i - 1];
    const auto [next_r, next_c] = route[i + 1];
    for (int t = 0; t < 4; ++t) {
      const int br = pr + dr[t], bc = pc + dc[t];
      if (!walkable(br, bc) || on_path[br][bc])
        continue;
      if ((br == prev_r && bc == prev_c) || (br == next_r && bc == next_c))
        continue;

      vector<pair<int, int>> branch;
      vector<vector<char>> vis(N, vector<char>(N, 0));
      int cr = br, cc = bc;
      vis[pr][pc] = 1;
      while ((int)branch.size() < 14) {
        branch.emplace_back(cr, cc);
        vis[cr][cc] = 1;
        vector<pair<int, int>> nbr;
        for (int u = 0; u < 4; ++u) {
          const int nr = cr + dr[u], nc = cc + dc[u];
          if (walkable(nr, nc) && !vis[nr][nc] && !on_path[nr][nc])
            nbr.emplace_back(nr, nc);
        }
        if (nbr.empty())
          break;
        int best_deg = 99;
        pair<int, int> pick = nbr[0];
        for (const auto [nr, nc] : nbr) {
          int deg = 0;
          for (int u = 0; u < 4; ++u) {
            const int ar = nr + dr[u], ac = nc + dc[u];
            if (walkable(ar, ac) && !vis[ar][ac] && !on_path[ar][ac])
              deg++;
          }
          if (deg < best_deg) {
            best_deg = deg;
            pick = {nr, nc};
          }
        }
        tie(cr, cc) = pick;
      }
      if ((int)branch.size() >= 2)
        candidates.push_back({i, branch});
    }
  }

  sort(candidates.begin(), candidates.end(),
       [](const BranchAlcove& a, const BranchAlcove& b) {
         return a.cells.size() > b.cells.size();
       });

  vector<vector<char>> used(N, vector<char>(N, 0));
  vector<BranchAlcove> chosen;
  for (const auto& cand : candidates) {
    if ((int)chosen.size() >= max_branches)
      break;
    bool clash = false;
    for (const auto [r, c] : cand.cells) {
      if (used[r][c]) {
        clash = true;
        break;
      }
    }
    if (clash)
      continue;
    for (const auto [r, c] : cand.cells)
      used[r][c] = 1;
    chosen.push_back(cand);
  }
  return chosen;
}

Plan build_branch_plan(const vector<pair<int, int>>& route) {
  Plan plan;
  if (route.size() < 4)
    return plan;

  const int max_sw = min(K - 1, 9);
  const vector<BranchAlcove> alcoves = find_branch_alcoves(route, max_sw);
  if (alcoves.empty())
    return plan;

  set<Edge> used, path_edges;
  for (int i = 0; i + 1 < (int)route.size(); ++i) {
    const auto [r0, c0] = route[i];
    const auto [r1, c1] = route[i + 1];
    path_edges.insert(normalize_edge(r0, c0, r1, c1));
  }

  vector<BranchAlcove> ordered = alcoves;
  sort(ordered.begin(), ordered.end(),
       [](const BranchAlcove& a, const BranchAlcove& b) {
         return a.path_index < b.path_index;
       });

  for (int k = 0; k < (int)ordered.size(); ++k) {
    const auto& alc = ordered[k];
    if (alc.path_index + 1 >= (int)route.size())
      continue;
    const auto [jr, jc] = route[alc.path_index];
    const auto [nr, nc] = route[alc.path_index + 1];
    const auto [br, bc] = alc.cells.front();
    const auto [sr, sc] = alc.cells.back();

    place_door(plan, jr, jc, nr, nc, 2 * k + 1, used);
    place_door(plan, jr, jc, br, bc, 2 * k, used);
    for (int i = 0; i + 1 < (int)alc.cells.size(); ++i) {
      const auto [a0, b0] = alc.cells[i];
      const auto [a1, b1] = alc.cells[i + 1];
      place_door(plan, a0, b0, a1, b1, 2 * k, used);
    }
    plan.switches.push_back({sr, sc, k});
  }

  if (HeroSimulator(plan).min_steps() <= 0)
    return Plan{};
  add_wall_doors(plan, path_edges, used);
  return plan;
}

vector<int> build_spanning_parent_from(int sr, int sc, mt19937& rng) {
  vector<int> parent(N * N, -1);
  vector<char> vis(N * N, 0);
  const int start = enc(sr, sc);
  if (!walkable(sr, sc))
    return parent;
  queue<int> q;
  parent[start] = start;
  vis[start] = 1;
  q.push(start);
  while (!q.empty()) {
    const int u = q.front();
    q.pop();
    const int r = u / N, c = u % N;
    array<int, 4> order = {0, 1, 2, 3};
    shuffle(order.begin(), order.end(), rng);
    for (int t : order) {
      const int nr = r + dr[t], nc = c + dc[t];
      if (!walkable(nr, nc))
        continue;
      const int v = enc(nr, nc);
      if (vis[v])
        continue;
      vis[v] = 1;
      parent[v] = u;
      q.push(v);
    }
  }
  return parent;
}

vector<int> build_spanning_parent(mt19937& rng) {
  return build_spanning_parent_from(0, 0, rng);
}

vector<pair<int, int>> spine_from_parent(const vector<int>& parent) {
  const int goal = enc(N - 1, N - 1);
  if (!walkable(0, 0) || !walkable(N - 1, N - 1) || parent[goal] < 0)
    return shortest_path(0, 0, N - 1, N - 1);

  vector<int> verts;
  for (int v = goal; v != parent[v]; v = parent[v])
    verts.push_back(v);
  verts.push_back(parent[goal]);
  reverse(verts.begin(), verts.end());

  vector<pair<int, int>> spine;
  spine.reserve(verts.size());
  for (int v : verts)
    spine.emplace_back(v / N, v % N);
  return spine;
}

vector<BranchAlcove> tree_branches(const vector<pair<int, int>>& spine,
                                   const vector<int>& parent, int max_branches) {
  vector<vector<char>> on_spine(N, vector<char>(N, 0));
  for (const auto [r, c] : spine)
    on_spine[r][c] = 1;

  map<int, int> spine_pos;
  for (int i = 0; i < (int)spine.size(); ++i)
    spine_pos[enc(spine[i].first, spine[i].second)] = i;

  vector<BranchAlcove> candidates;
  for (int i = 0; i < (int)spine.size(); ++i) {
    const auto [pr, pc] = spine[i];
    const int u = enc(pr, pc);
    for (int t = 0; t < 4; ++t) {
      const int nr = pr + dr[t], nc = pc + dc[t];
      if (!walkable(nr, nc) || on_spine[nr][nc])
        continue;
      const int v = enc(nr, nc);
      if (parent[v] != u && parent[u] != v)
        continue;

      vector<pair<int, int>> branch;
      vector<vector<char>> vis(N, vector<char>(N, 0));
      int cr = nr, cc = nc;
      vis[pr][pc] = 1;
      while ((int)branch.size() < 16) {
        branch.emplace_back(cr, cc);
        vis[cr][cc] = 1;
        int next = -1;
        for (int u2 = 0; u2 < 4; ++u2) {
          const int ar = cr + dr[u2], ac = cc + dc[u2];
          if (!walkable(ar, ac) || vis[ar][ac] || on_spine[ar][ac])
            continue;
          const int w = enc(ar, ac);
          if (parent[w] == enc(cr, cc) || parent[enc(cr, cc)] == w) {
            next = w;
            break;
          }
        }
        if (next < 0)
          break;
        cr = next / N;
        cc = next % N;
      }
      if ((int)branch.size() >= 2)
        candidates.push_back({i, branch});
    }
  }

  sort(candidates.begin(), candidates.end(),
       [](const BranchAlcove& a, const BranchAlcove& b) {
         if (a.cells.size() != b.cells.size())
           return a.cells.size() > b.cells.size();
         return a.path_index < b.path_index;
       });

  vector<vector<char>> used(N, vector<char>(N, 0));
  vector<BranchAlcove> chosen;
  set<int> used_junctions;
  for (const auto& cand : candidates) {
    if ((int)chosen.size() >= max_branches)
      break;
    if (used_junctions.count(cand.path_index))
      continue;
    bool clash = false;
    for (const auto [r, c] : cand.cells) {
      if (used[r][c]) {
        clash = true;
        break;
      }
    }
    if (clash)
      continue;
    for (const auto [r, c] : cand.cells)
      used[r][c] = 1;
    used_junctions.insert(cand.path_index);
    chosen.push_back(cand);
  }
  sort(chosen.begin(), chosen.end(),
       [](const BranchAlcove& a, const BranchAlcove& b) {
         return a.path_index < b.path_index;
       });
  return chosen;
}

Plan build_tree_plan(const vector<pair<int, int>>& spine, const vector<int>& parent) {
  Plan plan;
  if (spine.size() < 4)
    return plan;

  const int max_sw = min(K - 1, 9);
  const vector<BranchAlcove> branches = tree_branches(spine, parent, max_sw);
  if (branches.empty())
    return plan;

  set<Edge> used, spine_edges;
  for (int i = 0; i + 1 < (int)spine.size(); ++i) {
    const auto [r0, c0] = spine[i];
    const auto [r1, c1] = spine[i + 1];
    spine_edges.insert(normalize_edge(r0, c0, r1, c1));
  }

  for (int k = 0; k < (int)branches.size(); ++k) {
    const auto& br = branches[k];
    if (br.path_index + 1 >= (int)spine.size())
      continue;
    const auto [jr, jc] = spine[br.path_index];
    const auto [nr, nc] = spine[br.path_index + 1];
    const auto [br0, bc0] = br.cells.front();
    const auto [sr, sc] = br.cells.back();

    place_door(plan, jr, jc, nr, nc, 2 * k + 1, used);
    place_door(plan, jr, jc, br0, bc0, 2 * k, used);
    for (int i = 0; i + 1 < (int)br.cells.size(); ++i) {
      const auto [a0, b0] = br.cells[i];
      const auto [a1, b1] = br.cells[i + 1];
      place_door(plan, a0, b0, a1, b1, 2 * k, used);
    }
    plan.switches.push_back({sr, sc, k});
  }

  if (HeroSimulator(plan).min_steps() <= 0)
    return Plan{};
  add_wall_doors(plan, spine_edges, used);
  return plan;
}

void place_switch_widget(Plan& plan, const vector<pair<int, int>>& spine, int k,
                         int si, const vector<pair<int, int>>& branch,
                         set<Edge>& used) {
  const auto [jr, jc] = spine[si];
  const auto [nr, nc] = spine[si + 1];
  const auto [br, bc] = branch.front();
  const auto [sr, sc] = branch.back();

  place_door(plan, jr, jc, nr, nc, 2 * k + 1, used);
  place_door(plan, jr, jc, br, bc, 2 * k, used);
  if (k >= 1) {
    const auto [pr, pc] = spine[si - 1];
    place_door(plan, pr, pc, jr, jc, 2 * (k - 1) + 1, used);
  }
  if (k >= 2) {
    const auto [pr2, pc2] = spine[si - 2];
    const auto [pr1, pc1] = spine[si - 1];
    place_door(plan, pr2, pc2, pr1, pc1, 2 * (k - 2), used);
  }
  for (int i = 0; i + 1 < (int)branch.size(); ++i) {
    const auto [a0, b0] = branch[i];
    const auto [a1, b1] = branch[i + 1];
    place_door(plan, a0, b0, a1, b1, 2 * k, used);
  }
  plan.switches.push_back({sr, sc, k});
}

set<Edge> reserved_from_layout(const vector<pair<int, int>>& spine,
                               const vector<BranchAlcove>& branches) {
  set<Edge> reserved;
  for (int i = 0; i + 1 < (int)spine.size(); ++i) {
    const auto [r0, c0] = spine[i];
    const auto [r1, c1] = spine[i + 1];
    reserved.insert(normalize_edge(r0, c0, r1, c1));
  }
  for (const auto& br : branches) {
    const auto [pr, pc] = spine[br.path_index];
    const auto [br0, bc0] = br.cells.front();
    reserved.insert(normalize_edge(pr, pc, br0, bc0));
    for (int i = 0; i + 1 < (int)br.cells.size(); ++i) {
      const auto [a0, b0] = br.cells[i];
      const auto [a1, b1] = br.cells[i + 1];
      reserved.insert(normalize_edge(a0, b0, a1, b1));
    }
  }
  return reserved;
}

vector<pair<int, int>> spine_path(const vector<int>& parent);

using Clock = chrono::steady_clock;
using TimePoint = Clock::time_point;

Plan build_exponential_tree_plan(const vector<int>& parent,
                                 TimePoint deadline = TimePoint::max()) {
  const vector<pair<int, int>> spine = spine_path(parent);
  if (spine.size() < 12)
    return Plan{};

  const int L = (int)spine.size();
  const int gap = 3;
  vector<BranchAlcove> pool = tree_branches(spine, parent, 36);
  if (pool.size() < 3)
    return Plan{};

  Plan best;
  i64 best_score = 0;

  for (int try_sw = min(9, K - 1); try_sw >= 3; --try_sw) {
    if (Clock::now() >= deadline)
      break;
    if (L < try_sw * gap + 2)
      continue;

    vector<vector<char>> branch_used(N, vector<char>(N, 0));
    set<int> used_junction;
    vector<BranchAlcove> picked(try_sw);
    bool ok = true;

    // Switch 0: nearest start, prefer path_index == 1
    {
      int best_rank = -1;
      BranchAlcove pick{};
      for (const auto& alc : pool) {
        if (alc.path_index < 1 || alc.path_index > max(2, L / 4))
          continue;
        if (used_junction.count(alc.path_index))
          continue;
        bool clash = false;
        for (const auto [r, c] : alc.cells)
          if (branch_used[r][c]) {
            clash = true;
            break;
          }
        if (clash)
          continue;
        const int rank =
            (int)alc.cells.size() * 100 - alc.path_index * 10 + (alc.path_index == 1 ? 500 : 0);
        if (rank > best_rank) {
          best_rank = rank;
          pick = alc;
        }
      }
      if (best_rank < 0) {
        ok = false;
      } else {
        for (const auto [r, c] : pick.cells)
          branch_used[r][c] = 1;
        used_junction.insert(pick.path_index);
        picked[0] = pick;
      }
    }
    if (!ok)
      continue;

    for (int k = try_sw - 1; k >= 1; --k) {
      const int target = min(L - 2, 1 + k * max(gap, (L - 2) / try_sw));
      const int lo = max(1 + (k - 1) * gap, target - gap * 2);
      const int hi = min(L - 2, target + gap);
      int best_rank = -1;
      BranchAlcove pick{};
      for (const auto& alc : pool) {
        if (alc.path_index < lo || alc.path_index > hi)
          continue;
        if (used_junction.count(alc.path_index))
          continue;
        bool clash = false;
        for (const auto [r, c] : alc.cells)
          if (branch_used[r][c]) {
            clash = true;
            break;
          }
        if (clash)
          continue;
        const int rank =
            (int)alc.cells.size() * 100 - abs(alc.path_index - target) * 5;
        if (rank > best_rank) {
          best_rank = rank;
          pick = alc;
        }
      }
      if (best_rank < 0) {
        ok = false;
        break;
      }
      for (const auto [r, c] : pick.cells)
        branch_used[r][c] = 1;
      used_junction.insert(pick.path_index);
      picked[k] = pick;
    }
    if (!ok)
      continue;

    Plan plan;
    set<Edge> used;
    for (int k = 0; k < try_sw; ++k) {
      const int si = picked[k].path_index;
      if (si < 1 || si + 1 >= L) {
        ok = false;
        break;
      }
      place_switch_widget(plan, spine, k, si, picked[k].cells, used);
    }
    if (!ok)
      continue;

    const int steps = HeroSimulator(plan).min_steps();
    if (steps <= 0 || steps < N * try_sw * 2)
      continue;

    set<Edge> reserved = collect_tree_edges(parent);
    const set<Edge> layout = reserved_from_layout(spine, picked);
    reserved.insert(layout.begin(), layout.end());
    seal_shortcuts(plan, reserved, used);
    add_wall_doors(plan, layout, used);

    const i64 sc = score_plan(plan);
    if (sc > best_score) {
      best_score = sc;
      best = std::move(plan);
    }
  }
  return best;
}

Plan build_hybrid_plan(const vector<pair<int, int>>& route) {
  Plan plan;
  if (route.size() < 6)
    return plan;

  const int ring_sw = min(3, min(K - 1, (int)route.size() - 1));
  const vector<int> sw_idx = ring_switch_indices((int)route.size(), ring_sw, 0, 1);
  if (!valid_ring_indices(sw_idx, (int)route.size()))
    return plan;

  set<Edge> used, path_edges;
  for (int i = 0; i + 1 < (int)route.size(); ++i) {
    const auto [r0, c0] = route[i];
    const auto [r1, c1] = route[i + 1];
    path_edges.insert(normalize_edge(r0, c0, r1, c1));
  }

  add_ring_doors(plan, route, sw_idx, used);
  if (HeroSimulator(plan).min_steps() <= 0)
    return Plan{};

  const int branch_base = ring_sw;
  const vector<BranchAlcove> alcoves =
      find_branch_alcoves(route, min(K - 1, 9) - branch_base);
  vector<BranchAlcove> ordered = alcoves;
  sort(ordered.begin(), ordered.end(),
       [](const BranchAlcove& a, const BranchAlcove& b) {
         return a.path_index < b.path_index;
       });

  set<int> ring_cells;
  for (int idx : sw_idx)
    ring_cells.insert(enc(route[idx].first, route[idx].second));

  for (int k = 0; k < (int)ordered.size(); ++k) {
    const int sk = branch_base + k;
    if (sk >= K - 1)
      break;
    const auto& alc = ordered[k];
    if (alc.path_index + 1 >= (int)route.size())
      continue;
    if (ring_cells.count(enc(route[alc.path_index].first, route[alc.path_index].second)))
      continue;

    const auto [jr, jc] = route[alc.path_index];
    const auto [nr, nc] = route[alc.path_index + 1];
    const auto [br, bc] = alc.cells.front();
    const auto [sr, sc] = alc.cells.back();

    place_door(plan, jr, jc, nr, nc, 2 * sk + 1, used);
    place_door(plan, jr, jc, br, bc, 2 * sk, used);
    for (int i = 0; i + 1 < (int)alc.cells.size(); ++i) {
      const auto [a0, b0] = alc.cells[i];
      const auto [a1, b1] = alc.cells[i + 1];
      place_door(plan, a0, b0, a1, b1, 2 * sk, used);
    }
    plan.switches.push_back({sr, sc, sk});
  }

  if (HeroSimulator(plan).min_steps() <= 0)
    return Plan{};
  add_wall_doors(plan, path_edges, used);
  return plan;
}

bool plan_is_strong(const Plan& plan);

Plan build_gray_tree_plan(const vector<pair<int, int>>& spine, TimePoint deadline);

Plan build_from_route(const vector<pair<int, int>>& route,
                      const vector<int>* parent = nullptr, bool deep = false) {
  Plan best = best_ring_plan(route);
  i64 best_score = score_plan(best);

  auto consider = [&](Plan plan) {
    if (!plan_is_strong(plan))
      return;
    const i64 sc = score_plan(plan);
    if (sc > best_score) {
      best_score = sc;
      best = std::move(plan);
    }
  };

  consider(build_branch_plan(route));
  consider(build_hybrid_plan(route));
  if (deep) {
    consider(build_gray_tree_plan(route, TimePoint::max()));
    if (parent != nullptr) {
      const vector<pair<int, int>> spine = spine_path(*parent);
      if (!spine.empty()) {
        consider(build_tree_plan(spine, *parent));
        consider(build_exponential_tree_plan(*parent));
      }
    }
  }

  if (HeroSimulator(best).min_steps() > 0)
    return best;

  Plan fallback;
  if (route.size() < 3)
    return fallback;
  set<Edge> used, path_edges;
  for (int i = 0; i + 1 < (int)route.size(); ++i) {
    const auto [r0, c0] = route[i];
    const auto [r1, c1] = route[i + 1];
    path_edges.insert(normalize_edge(r0, c0, r1, c1));
  }
  const int mid = (int)route.size() / 2;
  const auto [sr, sc] = route[mid];
  fallback.switches.push_back({sr, sc, 0});
  const auto [r0, c0] = route[mid - 1];
  const auto [r1, c1] = route[mid];
  place_door(fallback, r0, c0, r1, c1, 1, used);
  if (HeroSimulator(fallback).min_steps() > 0)
    add_wall_doors(fallback, path_edges, used);
  return fallback;
}

int subtree_depth(int u, const vector<vector<int>>& children,
                  const vector<vector<char>>& on_spine) {
  int best = 0;
  for (const int v : children[u]) {
    if (on_spine[v / N][v % N])
      continue;
    best = max(best, 1 + subtree_depth(v, children, on_spine));
  }
  return best;
}

vector<pair<int, int>> deepest_branch(int u, const vector<vector<int>>& children,
                                      const vector<vector<char>>& on_spine) {
  vector<pair<int, int>> path;
  int cur = u;
  while (cur >= 0) {
    path.emplace_back(cur / N, cur % N);
    int nxt = -1, best = -1;
    for (const int v : children[cur]) {
      if (on_spine[v / N][v % N])
        continue;
      const int d = subtree_depth(v, children, on_spine);
      if (d > best) {
        best = d;
        nxt = v;
      }
    }
    if (nxt < 0)
      break;
    cur = nxt;
  }
  return path;
}

vector<int> path_to_root(int v, const vector<int>& parent) {
  vector<int> path;
  while (true) {
    path.push_back(v);
    if (parent[v] < 0 || parent[v] == v)
      break;
    v = parent[v];
  }
  return path;
}

vector<pair<int, int>> spine_path(const vector<int>& parent) {
  const int start = enc(0, 0), goal = enc(N - 1, N - 1);
  if (parent[goal] < 0 || parent[start] < 0)
    return shortest_path(0, 0, N - 1, N - 1);

  const vector<int> from_start = path_to_root(start, parent);
  const vector<int> from_goal = path_to_root(goal, parent);
  set<int> on_start_path(from_start.begin(), from_start.end());

  int lca = -1;
  for (int v : from_goal) {
    if (on_start_path.count(v)) {
      lca = v;
      break;
    }
  }
  if (lca < 0)
    return shortest_path(0, 0, N - 1, N - 1);

  vector<int> verts;
  for (int v : from_start) {
    verts.push_back(v);
    if (v == lca)
      break;
  }
  vector<int> goal_tail;
  for (int v : from_goal) {
    if (v == lca)
      break;
    goal_tail.push_back(v);
  }
  reverse(goal_tail.begin(), goal_tail.end());
  for (int v : goal_tail)
    verts.push_back(v);

  vector<pair<int, int>> spine;
  spine.reserve(verts.size());
  for (int v : verts)
    spine.emplace_back(v / N, v % N);
  return spine;
}

Plan build_gray_tree_plan(const vector<pair<int, int>>& spine,
                          TimePoint deadline = TimePoint::max()) {
  Plan best;
  i64 best_score = 0;
  for (int try_sw = min(9, K - 1); try_sw >= 3; --try_sw) {
    if (Clock::now() >= deadline)
      break;
    Plan plan;
    if (spine.size() < (size_t)try_sw * 3 + 2)
      continue;

    vector<vector<char>> on_spine(N, vector<char>(N, 0));
    for (const auto [r, c] : spine)
      on_spine[r][c] = 1;

    const int num_sw = try_sw;
    const int gap = 3;
    const int L = (int)spine.size();

    vector<BranchAlcove> alcoves;
    for (int i = 1; i + 1 < L; ++i) {
      const auto [pr, pc] = spine[i];
      const auto [prev_r, prev_c] = spine[i - 1];
      const auto [next_r, next_c] = spine[i + 1];
      for (int t = 0; t < 4; ++t) {
        const int br = pr + dr[t], bc = pc + dc[t];
        if (!walkable(br, bc) || on_spine[br][bc])
          continue;
        if ((br == prev_r && bc == prev_c) || (br == next_r && bc == next_c))
          continue;

        vector<pair<int, int>> branch;
        vector<vector<char>> vis(N, vector<char>(N, 0));
        int cr = br, cc = bc;
        vis[pr][pc] = 1;
        while ((int)branch.size() < 18) {
          branch.emplace_back(cr, cc);
          vis[cr][cc] = 1;
          vector<pair<int, int>> nbr;
          for (int u = 0; u < 4; ++u) {
            const int nr = cr + dr[u], nc = cc + dc[u];
            if (walkable(nr, nc) && !vis[nr][nc] && !on_spine[nr][nc])
              nbr.emplace_back(nr, nc);
          }
          if (nbr.empty())
            break;
          int best_deg = 99;
          pair<int, int> pick = nbr[0];
          for (const auto [nr, nc] : nbr) {
            int deg = 0;
            for (int u = 0; u < 4; ++u) {
              const int ar = nr + dr[u], ac = nc + dc[u];
              if (walkable(ar, ac) && !vis[ar][ac] && !on_spine[ar][ac])
                deg++;
            }
            if (deg < best_deg) {
              best_deg = deg;
              pick = {nr, nc};
            }
          }
          tie(cr, cc) = pick;
        }
        if ((int)branch.size() >= 2)
          alcoves.push_back({i, branch});
      }
    }
    if (alcoves.empty())
      continue;

    struct BranchSlot {
      int spine_idx = -1;
      vector<pair<int, int>> cells;
    };
    vector<BranchSlot> slots(num_sw);
    vector<vector<char>> branch_used(N, vector<char>(N, 0));
    bool ok = true;
    for (int k = num_sw - 1; k >= 0; --k) {
      const int si_hi =
          (k + 1 < num_sw && slots[k + 1].spine_idx >= 0)
              ? slots[k + 1].spine_idx - gap
              : L - 2;
      const int si_lo = max(1, si_hi - gap * 6);
      bool found = false;
      int best_rank = -1;
      BranchAlcove pick{};
      for (const auto& alc : alcoves) {
        if (alc.path_index < si_lo || alc.path_index > si_hi)
          continue;
        bool clash = false;
        for (const auto [r, c] : alc.cells) {
          if (branch_used[r][c]) {
            clash = true;
            break;
          }
        }
        if (clash)
          continue;
        const int rank = (int)alc.cells.size() * 100 - abs(alc.path_index - si_hi);
        if (rank > best_rank) {
          best_rank = rank;
          pick = alc;
          found = true;
        }
      }
      if (!found) {
        ok = false;
        break;
      }
      for (const auto [r, c] : pick.cells)
        branch_used[r][c] = 1;
      slots[k] = {pick.path_index, pick.cells};
    }
    if (!ok)
      continue;

    set<Edge> used, spine_edges;
    for (int i = 0; i + 1 < L; ++i) {
      const auto [r0, c0] = spine[i];
      const auto [r1, c1] = spine[i + 1];
      spine_edges.insert(normalize_edge(r0, c0, r1, c1));
    }

    for (int k = 0; k < num_sw; ++k) {
      const int si = slots[k].spine_idx;
      if (si < 1 || si + 1 >= L) {
        ok = false;
        break;
      }
      const auto [jr, jc] = spine[si];
      const auto [nr, nc] = spine[si + 1];
      const auto [br, bc] = slots[k].cells.front();
      const auto [sr, sc] = slots[k].cells.back();

      place_door(plan, jr, jc, nr, nc, 2 * k + 1, used);
      place_door(plan, jr, jc, br, bc, 2 * k, used);
      if (k >= 1) {
        const auto [pr, pc] = spine[si - 1];
        place_door(plan, pr, pc, jr, jc, 2 * (k - 1) + 1, used);
      }
      if (k >= 2) {
        const auto [pr2, pc2] = spine[si - 2];
        const auto [pr1, pc1] = spine[si - 1];
        place_door(plan, pr2, pc2, pr1, pc1, 2 * (k - 2), used);
      }
      for (int i = 0; i + 1 < (int)slots[k].cells.size(); ++i) {
        const auto [a0, b0] = slots[k].cells[i];
        const auto [a1, b1] = slots[k].cells[i + 1];
        place_door(plan, a0, b0, a1, b1, 2 * k, used);
      }
      plan.switches.push_back({sr, sc, k});
    }
    if (!ok)
      continue;

    const int steps = HeroSimulator(plan).min_steps();
    if (steps <= 0 || steps < N * num_sw * 2)
      continue;

    set<Edge> reserved;
    for (int i = 0; i + 1 < L; ++i) {
      const auto [r0, c0] = spine[i];
      const auto [r1, c1] = spine[i + 1];
      reserved.insert(normalize_edge(r0, c0, r1, c1));
    }
    for (int k = 0; k < num_sw; ++k) {
      const int si = slots[k].spine_idx;
      const auto [jr, jc] = spine[si];
      reserved.insert(normalize_edge(jr, jc, slots[k].cells.front().first,
                                     slots[k].cells.front().second));
      for (int i = 0; i + 1 < (int)slots[k].cells.size(); ++i) {
        const auto [a0, b0] = slots[k].cells[i];
        const auto [a1, b1] = slots[k].cells[i + 1];
        reserved.insert(normalize_edge(a0, b0, a1, b1));
      }
    }
    seal_shortcuts(plan, reserved, used);
    add_wall_doors(plan, reserved, used);
    const i64 sc = score_plan(plan);
    if (sc > best_score) {
      best_score = sc;
      best = std::move(plan);
    }
  }
  return best;
}

Plan build_gray_tree_plan(const vector<int>& parent) {
  return build_gray_tree_plan(spine_path(parent));
}

void hill_climb_ring(Plan& best, i64& best_score, const vector<pair<int, int>>& route,
                     int num_sw, mt19937& rng,
                     chrono::steady_clock::time_point deadline) {
  num_sw = min(num_sw, min(3, min(K - 1, (int)route.size() - 1)));
  if (route.size() < 4 || num_sw < 2)
    return;

  vector<int> offsets = ring_switch_indices((int)route.size(), num_sw, 0, 1);
  while (chrono::steady_clock::now() < deadline) {
    const int k = uniform_int_distribution<int>(0, num_sw - 1)(rng);
    const int delta = bernoulli_distribution(0.5)(rng) ? 1 : -1;
    vector<int> trial = offsets;
    trial[k] += delta;
    sort(trial.begin(), trial.end());
    if (!valid_ring_indices(trial, (int)route.size()))
      continue;
    Plan plan = build_ring_plan(route, num_sw, trial);
    const i64 sc = score_plan(plan);
    if (sc > best_score) {
      best_score = sc;
      best = std::move(plan);
      offsets = trial;
    }
  }
}

void write_plan(const Plan& plan) {
  cout << plan.doors.size() << '\n';
  for (const Door& d : plan.doors)
    cout << d.orient << ' ' << d.i << ' ' << d.j << ' ' << d.g << '\n';
  cout << plan.switches.size() << '\n';
  for (const Switch& s : plan.switches)
    cout << s.i << ' ' << s.j << ' ' << s.k << '\n';
}

void solve() {
  mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());
  const auto deadline =
      chrono::steady_clock::now() + chrono::milliseconds(1950);

  Plan best;
  i64 best_score = -1;
  vector<pair<int, int>> best_route;
  bool best_is_ring = false;

  auto adopt = [&](Plan plan, const vector<pair<int, int>>& route, bool ring) {
    if (!plan_is_strong(plan))
      return;
    const i64 sc = score_plan(plan);
    if (sc > best_score) {
      best_score = sc;
      best = std::move(plan);
      best_route = route;
      best_is_ring = ring;
    }
  };

  const auto tree_end = deadline - chrono::milliseconds(900);
  while (chrono::steady_clock::now() < tree_end) {
    const bool from_throne = bernoulli_distribution(0.65)(rng);
    const vector<int> parent =
        from_throne ? build_spanning_parent_from(N - 1, N - 1, rng)
                    : build_spanning_parent(rng);
    const vector<pair<int, int>> spine = spine_path(parent);
    if (spine.size() < 8)
      continue;

    Plan exp = build_exponential_tree_plan(parent, tree_end);
    if (!exp.switches.empty())
      adopt(exp, spine, false);

    if (chrono::steady_clock::now() >= tree_end)
      break;

    Plan gray = build_gray_tree_plan(spine, tree_end);
    if (!gray.switches.empty())
      adopt(gray, spine, false);
  }

  const auto gray_end = deadline - chrono::milliseconds(500);
  while (chrono::steady_clock::now() < gray_end) {
    vector<pair<int, int>> route =
        dfs_long_path(rng, uniform_int_distribution<int>(20, 45)(rng));
    if (route.empty()) {
      const vector<int> parent = build_spanning_parent_from(N - 1, N - 1, rng);
      route = spine_path(parent);
    }
    if (route.empty())
      route = shortest_path(0, 0, N - 1, N - 1);
    if (route.size() < 8)
      continue;

    adopt(build_from_route(route, nullptr, false), route, false);
  }

  const vector<int> parent = build_spanning_parent(rng);
  const vector<pair<int, int>> tree_spine = spine_path(parent);
  const vector<pair<int, int>> shortest =
      shortest_path(0, 0, N - 1, N - 1);

  if (chrono::steady_clock::now() < deadline) {
    const int ms_left = (int)chrono::duration_cast<chrono::milliseconds>(
                            deadline - Clock::now())
                            .count();
    vector<pair<int, int>> longest =
        dfs_long_path(rng, min(400, max(0, ms_left - 300)));
    if (longest.size() < shortest.size())
      longest = tree_spine;
    if (!longest.empty()) {
      for (const int roots : {0, 1}) {
        if (Clock::now() >= deadline)
          break;
        const vector<int> p =
            roots ? build_spanning_parent_from(N - 1, N - 1, rng)
                  : build_spanning_parent(rng);
        adopt(build_exponential_tree_plan(p, deadline), spine_path(p), false);
      }
    }
  }

  const vector<pair<int, int>>* seeds[] = {&tree_spine, &shortest};
  for (const auto* seed_route : seeds) {
    if (Clock::now() >= deadline || seed_route->empty())
      continue;
    Plan plan = build_from_route(*seed_route, nullptr, false);
    const i64 sc = score_plan(plan);
    if (sc > best_score) {
      best_score = sc;
      best = std::move(plan);
      best_route = *seed_route;
      best_is_ring = ((int)best.switches.size() <= 3);
    }
  }

  const auto search_end = deadline - chrono::milliseconds(400);
  while (Clock::now() < search_end) {
    vector<pair<int, int>> route =
        dfs_long_path(rng, uniform_int_distribution<int>(20, 45)(rng));
    if (route.empty())
      route = greedy_long_path(rng);
    if (route.empty())
      route = tree_spine.empty() ? shortest : tree_spine;
    if (route.empty())
      continue;

    Plan plan = build_from_route(route, nullptr, false);
    const i64 sc = score_plan(plan);
    if (sc > best_score) {
      best_score = sc;
      best = std::move(plan);
      best_route = route;
      best_is_ring = ((int)best.switches.size() <= 3);
    }
  }

  if (!best_route.empty() && best_is_ring && !best.switches.empty())
    hill_climb_ring(best, best_score, best_route, (int)best.switches.size(), rng,
                    deadline);

  if (best.switches.empty() || HeroSimulator(best).min_steps() <= 0) {
    const vector<pair<int, int>> route =
        shortest_path(0, 0, N - 1, N - 1);
    Plan fallback = build_from_route(route, nullptr);
    if (!fallback.switches.empty() &&
        HeroSimulator(fallback).min_steps() > 0) {
      best = std::move(fallback);
    } else if (!route.empty()) {
      best = best_ring_plan(route);
    }
  }

  write_plan(best);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> M >> K;
  grid.resize(N);
  for (int i = 0; i < N; ++i)
    cin >> grid[i];

  solve();
  return 0;
}

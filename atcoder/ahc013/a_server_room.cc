// AHC013 A - Server Room: https://atcoder.jp/contests/ahc013/tasks/ahc013_a
//
// Greedy same-type clustering: relocate blockers, BFS-move toward clear mates,
// then repeatedly connect the best-scoring valid homogeneous edge.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
using Clock = chrono::steady_clock;

namespace {

constexpr int CABLE = -1;
constexpr int dr[4] = {-1, 1, 0, 0};
constexpr int dc[4] = {0, 0, -1, 1};

struct MoveOp {
  int r0, c0, r1, c1;
};

struct ConnectOp {
  int r0, c0, r1, c1;
};

struct Sim {
  int n, k, budget;
  vector<vector<int>> grid;
  vector<ConnectOp> cables;
  set<uint64_t> cable_set;
  vector<MoveOp> moves;
  vector<ConnectOp> connects;

  Sim(int n_, int k_, vector<vector<int>> g)
      : n(n_), k(k_), budget(100 * k_), grid(std::move(g)) {}

  static uint64_t edge_key(int r0, int c0, int r1, int c1) {
    if (tie(r0, c0) > tie(r1, c1))
      swap(r0, c0), swap(r1, c1);
    return (uint64_t(r0) << 24) | (uint64_t(c0) << 16) | (uint64_t(r1) << 8) | uint64_t(c1);
  }

  bool inside(int r, int c) const { return 0 <= r && r < n && 0 <= c && c < n; }

  bool is_computer(int r, int c) const {
    return inside(r, c) && grid[r][c] > 0;
  }

  bool is_empty(int r, int c) const {
    return inside(r, c) && grid[r][c] == 0;
  }

  bool can_move(int r0, int c0, int r1, int c1) const {
    return inside(r0, c0) && inside(r1, c1) && abs(r0 - r1) + abs(c0 - c1) == 1 &&
           is_computer(r0, c0) && is_empty(r1, c1);
  }

  bool path_clear(int r0, int c0, int r1, int c1) const {
    if (r0 != r1 && c0 != c1)
      return false;
    if (r0 == r1) {
      for (int c = min(c0, c1) + 1; c < max(c0, c1); ++c)
        if (grid[r0][c] != 0)
          return false;
    } else {
      for (int r = min(r0, r1) + 1; r < max(r0, r1); ++r)
        if (grid[r][c0] != 0)
          return false;
    }
    return true;
  }

  bool cable_crosses(int r0, int c0, int r1, int c1) const {
    const int cr0 = (r0 == r1) ? r0 : min(r0, r1) + 1;
    const int cr1 = (r0 == r1) ? r0 : max(r0, r1) - 1;
    const int cc0 = (c0 == c1) ? c0 : min(c0, c1) + 1;
    const int cc1 = (c0 == c1) ? c0 : max(c0, c1) - 1;
    for (int r = cr0; r <= cr1; ++r)
      for (int c = cc0; c <= cc1; ++c)
        if (grid[r][c] == CABLE)
          return false;
    return true;
  }

  bool can_connect(int r0, int c0, int r1, int c1) const {
    if (!is_computer(r0, c0) || !is_computer(r1, c1))
      return false;
    if (r0 == r1 && c0 == c1)
      return false;
    if (r0 != r1 && c0 != c1)
      return false;
    if (cable_set.count(edge_key(r0, c0, r1, c1)))
      return false;
    if (!path_clear(r0, c0, r1, c1))
      return false;
    if (!cable_crosses(r0, c0, r1, c1))
      return false;
    return true;
  }

  void apply_move(int r0, int c0, int r1, int c1) {
    grid[r1][c1] = grid[r0][c0];
    grid[r0][c0] = 0;
    moves.push_back({r0, c0, r1, c1});
  }

  void apply_connect(int r0, int c0, int r1, int c1) {
    const int cr0 = (r0 == r1) ? r0 : min(r0, r1) + 1;
    const int cr1 = (r0 == r1) ? r0 : max(r0, r1) - 1;
    const int cc0 = (c0 == c1) ? c0 : min(c0, c1) + 1;
    const int cc1 = (c0 == c1) ? c0 : max(c0, c1) - 1;
    for (int r = cr0; r <= cr1; ++r)
      for (int c = cc0; c <= cc1; ++c)
        grid[r][c] = CABLE;
    cables.push_back({r0, c0, r1, c1});
    cable_set.insert(edge_key(r0, c0, r1, c1));
    connects.push_back({r0, c0, r1, c1});
  }

  int id(int r, int c) const { return r * n + c; }

  int score() const {
    vector<int> par(n * n), sz(n * n, 1);
    iota(par.begin(), par.end(), 0);
    function<int(int)> find = [&](int x) -> int {
      return par[x] == x ? x : par[x] = find(par[x]);
    };
    auto unite = [&](int a, int b) {
      a = find(a);
      b = find(b);
      if (a == b)
        return;
      if (sz[a] < sz[b])
        swap(a, b);
      par[b] = a;
      sz[a] += sz[b];
    };
    for (const auto& e : cables)
      unite(id(e.r0, e.c0), id(e.r1, e.c1));

    vector<tuple<int, int, int>> comps;
    for (int r = 0; r < n; ++r)
      for (int c = 0; c < n; ++c)
        if (is_computer(r, c))
          comps.push_back({find(id(r, c)), r, c});
    sort(comps.begin(), comps.end());

    int ans = 0;
    for (int i = 0; i < (int)comps.size(); ++i) {
      const int ri = get<1>(comps[i]);
      const int ci = get<2>(comps[i]);
      const int ti = grid[ri][ci];
      for (int j = i + 1; j < (int)comps.size(); ++j) {
        if (get<0>(comps[i]) != get<0>(comps[j]))
          break;
        const int rj = get<1>(comps[j]);
        const int cj = get<2>(comps[j]);
        ans += (grid[rj][cj] == ti) ? 1 : -1;
      }
    }
    return ans;
  }

  struct ClusterInfo {
    int root;
    array<int, 6> cnt{};
    vector<pair<int, int>> cells;
  };

  vector<ClusterInfo> clusters() const {
    vector<int> par(n * n);
    iota(par.begin(), par.end(), 0);
    function<int(int)> find = [&](int x) -> int {
      return par[x] == x ? x : par[x] = find(par[x]);
    };
    auto unite = [&](int a, int b) {
      a = find(a);
      b = find(b);
      if (a != b)
        par[b] = a;
    };
    for (const auto& e : cables)
      unite(id(e.r0, e.c0), id(e.r1, e.c1));

    map<int, ClusterInfo> mp;
    for (int r = 0; r < n; ++r) {
      for (int c = 0; c < n; ++c) {
        if (!is_computer(r, c))
          continue;
        const int root = find(id(r, c));
        auto& cl = mp[root];
        cl.root = root;
        cl.cnt[grid[r][c]]++;
        cl.cells.push_back({r, c});
      }
    }
    vector<ClusterInfo> out;
    for (auto& [_, cl] : mp)
      out.push_back(std::move(cl));
    return out;
  }

  int connect_gain(int r0, int c0, int r1, int c1) const {
    if (grid[r0][c0] != grid[r1][c1])
      return -1000000;
    auto cls = clusters();
    map<int, ClusterInfo> by_root;
    for (const auto& cl : cls)
      by_root[cl.root] = cl;

    vector<int> par(n * n);
    iota(par.begin(), par.end(), 0);
    function<int(int)> find = [&](int x) -> int {
      return par[x] == x ? x : par[x] = find(par[x]);
    };
    auto unite = [&](int a, int b) {
      a = find(a);
      b = find(b);
      if (a != b)
        par[b] = a;
    };
    for (const auto& e : cables)
      unite(id(e.r0, e.c0), id(e.r1, e.c1));

    const int ra = find(id(r0, c0)), rb = find(id(r1, c1));
    if (ra == rb)
      return 0;
    const auto& ca = by_root.at(ra);
    const auto& cb = by_root.at(rb);
    array<int, 6> cnt{};
    for (int t = 1; t <= k; ++t)
      cnt[t] = ca.cnt[t] + cb.cnt[t];
    int after = 0;
    for (int t = 1; t <= k; ++t)
      after += cnt[t] * (cnt[t] - 1) / 2;
    for (int t1 = 1; t1 <= k; ++t1)
      for (int t2 = t1 + 1; t2 <= k; ++t2)
        after -= cnt[t1] * cnt[t2];
    int before = 0;
    for (const auto* cl : {&ca, &cb}) {
      for (int t = 1; t <= k; ++t)
        before += cl->cnt[t] * (cl->cnt[t] - 1) / 2;
      for (int t1 = 1; t1 <= k; ++t1)
        for (int t2 = t1 + 1; t2 <= k; ++t2)
          before -= cl->cnt[t1] * cl->cnt[t2];
    }
    return after - before;
  }

  int ops_left() const {
    return budget - (int)moves.size() - (int)connects.size();
  }

  bool try_connect_best() {
    struct Cand {
      int gain, r0, c0, r1, c1;
      bool operator<(const Cand& o) const { return gain < o.gain; }
    };
    vector<Cand> cand;
    for (int r = 0; r < n; ++r) {
      for (int c = 0; c < n; ++c) {
        if (!is_computer(r, c))
          continue;
        for (int c2 = c + 1; c2 < n; ++c2) {
          if (!is_computer(r, c2) || grid[r][c] != grid[r][c2])
            continue;
          if (!can_connect(r, c, r, c2))
            continue;
          cand.push_back({connect_gain(r, c, r, c2), r, c, r, c2});
        }
        for (int r2 = r + 1; r2 < n; ++r2) {
          if (!is_computer(r2, c) || grid[r][c] != grid[r2][c])
            continue;
          if (!can_connect(r, c, r2, c))
            continue;
          cand.push_back({connect_gain(r, c, r2, c), r, c, r2, c});
        }
      }
    }
    if (cand.empty())
      return false;
    sort(cand.rbegin(), cand.rend());
    for (const auto& x : cand) {
      if (x.gain <= 0)
        break;
      if (!can_connect(x.r0, x.c0, x.r1, x.c1))
        continue;
      apply_connect(x.r0, x.c0, x.r1, x.c1);
      return true;
    }
    return false;
  }

  bool bfs_move(int sr, int sc, int tr, int tc, int max_steps) {
    if (sr == tr && sc == tc)
      return false;
    queue<pair<int, int>> q;
    vector<vector<pair<int, int>>> prev(n, vector<pair<int, int>>(n, {-1, -1}));
    q.push({sr, sc});
    prev[sr][sc] = {sr, sc};
    while (!q.empty()) {
      auto [r, c] = q.front();
      q.pop();
      if (r == tr && c == tc)
        break;
      for (int d = 0; d < 4; ++d) {
        const int nr = r + dr[d], nc = c + dc[d];
        if (!inside(nr, nc) || prev[nr][nc].first != -1)
          continue;
        if (!(is_empty(nr, nc) || (nr == tr && nc == tc)))
          continue;
        prev[nr][nc] = {r, c};
        q.push({nr, nc});
      }
    }
    if (prev[tr][tc].first == -1)
      return false;

    vector<pair<int, int>> path;
    for (int r = tr, c = tc; !(r == sr && c == sc);) {
      path.push_back({r, c});
      tie(r, c) = prev[r][c];
    }
    reverse(path.begin(), path.end());
    const int steps = min((int)path.size(), max_steps);
    if (steps == 0)
      return false;
    int cr = sr, cc = sc;
    for (int i = 0; i < steps; ++i) {
      const auto [nr, nc] = path[i];
      if (ops_left() <= 0 || !can_move(cr, cc, nr, nc))
        return false;
      apply_move(cr, cc, nr, nc);
      cr = nr;
      cc = nc;
    }
    return true;
  }

  bool move_blocker_once(int r0, int c0, int r1, int c1) {
    if (path_clear(r0, c0, r1, c1))
      return false;
    vector<pair<int, int>> blockers;
    if (r0 == r1) {
      for (int c = min(c0, c1) + 1; c < max(c0, c1); ++c)
        if (grid[r0][c] > 0)
          blockers.push_back({r0, c});
    } else {
      for (int r = min(r0, r1) + 1; r < max(r0, r1); ++r)
        if (grid[r][c0] > 0)
          blockers.push_back({r, c0});
    }
    if (blockers.size() != 1 || ops_left() <= 0)
      return false;
    const auto [br, bc] = blockers[0];
    for (int d = 0; d < 4; ++d) {
      const int nr = br + dr[d], nc = bc + dc[d];
      if (!can_move(br, bc, nr, nc))
        continue;
      apply_move(br, bc, nr, nc);
      return true;
    }
    return false;
  }

  void clear_and_connect(int r0, int c0, int r1, int c1) {
    while (ops_left() > 0 && !can_connect(r0, c0, r1, c1)) {
      if (move_blocker_once(r0, c0, r1, c1))
        continue;
      if (r0 == r1) {
        const int tc = (c0 + c1) / 2;
        if (!bfs_move(r0, c0, r0, tc, 1))
          break;
      } else {
        const int tr = (r0 + r1) / 2;
        if (!bfs_move(r0, c0, tr, c0, 1))
          break;
      }
    }
    if (ops_left() > 0 && can_connect(r0, c0, r1, c1) && connect_gain(r0, c0, r1, c1) > 0)
      apply_connect(r0, c0, r1, c1);
  }

  void sample_connects() {
    for (int r = 0; r < n; ++r) {
      for (int c = 0; c < n; ++c) {
        if (!is_computer(r, c))
          continue;
        const int t = grid[r][c];
        if (c + 1 < n && is_computer(r, c + 1) && grid[r][c + 1] == t &&
            can_connect(r, c, r, c + 1))
          apply_connect(r, c, r, c + 1);
        if (r + 1 < n && is_computer(r + 1, c) && grid[r + 1][c] == t &&
            can_connect(r, c, r + 1, c))
          apply_connect(r, c, r + 1, c);
      }
    }
  }

  void improve_with_moves(mt19937& rng) {
    struct Cand {
      int gain, dist, r0, c0, r1, c1;
      bool operator<(const Cand& o) const {
        if (gain != o.gain)
          return gain < o.gain;
        return dist > o.dist;
      }
    };
    vector<Cand> cand;
    for (int r = 0; r < n; ++r) {
      for (int c = 0; c < n; ++c) {
        if (!is_computer(r, c))
          continue;
        const int t = grid[r][c];
        for (int r2 = 0; r2 < n; ++r2) {
          for (int c2 = 0; c2 < n; ++c2) {
            if (!is_computer(r2, c2) || grid[r2][c2] != t)
              continue;
            if (r == r2 && c == c2)
              continue;
            if (r != r2 && c != c2)
              continue;
            if (can_connect(r, c, r2, c2))
              continue;
            const int g = connect_gain(r, c, r2, c2);
            if (g <= 0)
              continue;
            cand.push_back({g, abs(r - r2) + abs(c - c2), r, c, r2, c2});
          }
        }
      }
    }
    sort(cand.begin(), cand.end(), [](const Cand& a, const Cand& b) {
      if (a.gain != b.gain)
        return a.gain > b.gain;
      return a.dist < b.dist;
    });

    for (const auto& x : cand) {
      if (ops_left() <= 1)
        break;
      if (can_connect(x.r0, x.c0, x.r1, x.c1))
        continue;
      clear_and_connect(x.r0, x.c0, x.r1, x.c1);
    }
  }

  void solve(mt19937& rng) {
    sample_connects();
    while (ops_left() > 0 && try_connect_best()) {
    }
    improve_with_moves(rng);
    while (ops_left() > 0 && try_connect_best()) {
    }
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, K;
  cin >> N >> K;
  vector<vector<int>> init(N, vector<int>(N));
  for (int i = 0; i < N; ++i) {
    string s;
    cin >> s;
    for (int j = 0; j < N; ++j)
      init[i][j] = s[j] - '0';
  }

  const auto deadline = Clock::now() + chrono::milliseconds(2800);
  mt19937 rng(8812345);

  Sim best(N, K, init);
  best.solve(rng);
  int best_score = best.score();

  for (int iter = 1; Clock::now() < deadline; ++iter) {
    Sim cur(N, K, init);
    rng.seed(8812345u + iter);
    cur.solve(rng);
    const int sc = cur.score();
    if (sc > best_score) {
      best_score = sc;
      best = std::move(cur);
    }
  }

  cout << best.moves.size() << '\n';
  for (const auto& m : best.moves)
    cout << m.r0 << ' ' << m.c0 << ' ' << m.r1 << ' ' << m.c1 << '\n';
  cout << best.connects.size() << '\n';
  for (const auto& e : best.connects)
    cout << e.r0 << ' ' << e.c0 << ' ' << e.r1 << ' ' << e.c1 << '\n';

  cerr << "score=" << best_score << '\n';
  return 0;
}

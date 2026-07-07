// AHC008 A - Territory: https://atcoder.jp/contests/ahc008/tasks/ahc008_a
//
// Region partition: assign each human a horizontal or vertical band (fewer initial
// pets), march along the band perimeter placing walls, then move to band center.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int D = 30;
constexpr int MAX_T = 300;
constexpr int DR[4] = {-1, 1, 0, 0};
constexpr int DC[4] = {0, 0, -1, 1};
constexpr char WALL_C[4] = {'u', 'd', 'l', 'r'};
constexpr char MOVE_C[4] = {'U', 'D', 'L', 'R'};

struct Rect {
  int x0, x1, y0, y1;
};

struct Game {
  array<array<bool, D>, D> blocked{};
  vector<pair<int, int>> humans;
  vector<pair<int, int>> pets;
  int M = 0;
  int N = 0;

  bool in_bounds(int x, int y) const {
    return 0 <= x && x < D && 0 <= y && y < D;
  }

  bool pet_on(int x, int y) const {
    for (const auto& [px, py] : pets) {
      if (px == x && py == y)
        return true;
    }
    return false;
  }

  bool human_on(int x, int y, int skip = -1,
                const vector<pair<int, int>>* at = nullptr) const {
    const auto& hs = at ? *at : humans;
    for (int i = 0; i < (int)hs.size(); ++i) {
      if (i == skip)
        continue;
      if (hs[i].first == x && hs[i].second == y)
        return true;
    }
    return false;
  }

  bool pet_adjacent(int x, int y) const {
    for (int d = 0; d < 4; ++d) {
      const int nx = x + DR[d];
      const int ny = y + DC[d];
      if (in_bounds(nx, ny) && pet_on(nx, ny))
        return true;
    }
    return false;
  }

  bool can_block(int x, int y,
                 const vector<pair<int, int>>* humans_at = nullptr) const {
    if (!in_bounds(x, y) || blocked[x][y])
      return false;
    if (pet_on(x, y) || human_on(x, y, -1, humans_at))
      return false;
    return !pet_adjacent(x, y);
  }

  bool can_move(int x, int y) const {
    return in_bounds(x, y) && !blocked[x][y];
  }

  int pets_in_rect(const Rect& r) const {
    int cnt = 0;
    for (const auto& [px, py] : pets) {
      if (r.x0 <= px && px <= r.x1 && r.y0 <= py && py <= r.y1)
        ++cnt;
    }
    return cnt;
  }

  vector<pair<int, int>> perimeter_walls(const Rect& r) const {
    vector<pair<int, int>> cells;
    auto add = [&](int x, int y) {
      if (in_bounds(x, y) && !blocked[x][y])
        cells.emplace_back(x, y);
    };
    if (r.x0 > 0) {
      for (int y = r.y0; y <= r.y1; ++y)
        add(r.x0 - 1, y);
    }
    if (r.x1 + 1 < D) {
      for (int y = r.y0; y <= r.y1; ++y)
        add(r.x1 + 1, y);
    }
    if (r.y0 > 0) {
      for (int x = r.x0; x <= r.x1; ++x)
        add(x, r.y0 - 1);
    }
    if (r.y1 + 1 < D) {
      for (int x = r.x0; x <= r.x1; ++x)
        add(x, r.y1 + 1);
    }
    return cells;
  }

  vector<vector<int>> parent_bfs(int sx, int sy) const {
    vector<vector<int>> par(D, vector<int>(D, -1));
    queue<pair<int, int>> q;
    par[sx][sy] = -2;
    q.emplace(sx, sy);
    while (!q.empty()) {
      const auto [x, y] = q.front();
      q.pop();
      for (int d = 0; d < 4; ++d) {
        const int nx = x + DR[d];
        const int ny = y + DC[d];
        if (in_bounds(nx, ny) && !blocked[nx][ny] && par[nx][ny] < 0) {
          par[nx][ny] = d;
          q.emplace(nx, ny);
        }
      }
    }
    return par;
  }

  optional<int> first_step_toward(int sx, int sy, int tx, int ty) const {
    const auto par = parent_bfs(sx, sy);
    if (par[tx][ty] < 0)
      return nullopt;
    int x = tx, y = ty;
    int last = par[x][y];
    while (!(x == sx && y == sy)) {
      const int d = par[x][y];
      const int px = x - DR[d];
      const int py = y - DC[d];
      if (px == sx && py == sy)
        return d;
      x = px;
      y = py;
      last = par[x][y];
    }
    return last;
  }

  char move_toward(int hi, int tx, int ty) const {
    const int hx = humans[hi].first;
    const int hy = humans[hi].second;
    if (hx == tx && hy == ty)
      return '.';
    const auto step = first_step_toward(hx, hy, tx, ty);
    if (!step)
      return '.';
    return MOVE_C[*step];
  }

  char place_adjacent_wall(
      int hi, const vector<pair<int, int>>& targets,
      const vector<pair<int, int>>& humans_at) const {
    const int hx = humans[hi].first;
    const int hy = humans[hi].second;
    int best_d = -1;
    int best_dist = INT_MAX;
    for (int d = 0; d < 4; ++d) {
      const int tx = hx + DR[d];
      const int ty = hy + DC[d];
      if (!can_block(tx, ty, &humans_at))
        continue;
      bool wanted = false;
      int dist = INT_MAX;
      for (const auto& [wx, wy] : targets) {
        if (wx == tx && wy == ty)
          wanted = true;
        dist = min(dist, abs(wx - hx) + abs(wy - hy));
      }
      if (!wanted)
        continue;
      if (dist < best_dist) {
        best_dist = dist;
        best_d = d;
      }
    }
    if (best_d < 0)
      return '.';
    return WALL_C[best_d];
  }
};

struct Plan {
  Rect region{};
  pair<int, int> home{};
  vector<pair<int, int>> walls;
};

vector<Plan> build_plans(int M, const Game& g) {
  struct Cand {
    Rect r;
    int pets;
  };
  vector<Cand> cands;
  for (int i = 0; i < M; ++i) {
    const int x0 = i * D / M;
    const int x1 = (i + 1) * D / M - 1;
    cands.push_back({{x0, x1, 0, D - 1}, g.pets_in_rect({x0, x1, 0, D - 1})});
    const int y0 = i * D / M;
    const int y1 = (i + 1) * D / M - 1;
    cands.push_back({{0, D - 1, y0, y1}, g.pets_in_rect({0, D - 1, y0, y1})});
  }
  sort(cands.begin(), cands.end(),
       [](const Cand& a, const Cand& b) { return a.pets < b.pets; });

  vector<Plan> plans(M);
  vector<bool> used(cands.size(), false);
  for (int hi = 0; hi < M; ++hi) {
    int pick = -1;
    for (int j = 0; j < (int)cands.size(); ++j) {
      if (!used[j]) {
        pick = j;
        break;
      }
    }
    if (pick < 0)
      pick = hi % cands.size();
    used[pick] = true;

    Plan plan;
    plan.region = cands[pick].r;
    plan.home = {(plan.region.x0 + plan.region.x1) / 2,
                 (plan.region.y0 + plan.region.y1) / 2};
    plan.walls = g.perimeter_walls(plan.region);
    const int sx = g.humans[hi].first;
    const int sy = g.humans[hi].second;
    sort(plan.walls.begin(), plan.walls.end(), [&](const auto& a, const auto& b) {
      const int da = abs(a.first - sx) + abs(a.second - sy);
      const int db = abs(b.first - sx) + abs(b.second - sy);
      return da < db;
    });
    plans[hi] = plan;
  }
  return plans;
}

char decide(int hi, const Plan& plan, const Game& g,
            const vector<pair<int, int>>& humans_at) {
  vector<pair<int, int>> pending;
  for (const auto& [wx, wy] : plan.walls) {
    if (!g.blocked[wx][wy])
      pending.emplace_back(wx, wy);
  }

  if (!pending.empty()) {
    if (const char w = g.place_adjacent_wall(hi, pending, humans_at); w != '.')
      return w;
    const auto [tx, ty] = pending.front();
    if (const char mv = g.move_toward(hi, tx, ty); mv != '.')
      return mv;
    return g.move_toward(hi, plan.home.first, plan.home.second);
  }

  return g.move_toward(hi, plan.home.first, plan.home.second);
}

}  // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Game g;
  cin >> g.N;
  g.pets.resize(g.N);
  for (int i = 0; i < g.N; ++i) {
    int x, y, t;
    cin >> x >> y >> t;
    --x;
    --y;
    g.pets[i] = {x, y};
  }
  cin >> g.M;
  g.humans.resize(g.M);
  for (int i = 0; i < g.M; ++i) {
    int x, y;
    cin >> x >> y;
    --x;
    --y;
    g.humans[i] = {x, y};
  }

  const vector<Plan> plans = build_plans(g.M, g);

  for (int turn = 0; turn < MAX_T; ++turn) {
    const vector<pair<int, int>> humans_at = g.humans;
    Game sim = g;
    string out(g.M, '.');
    for (int i = 0; i < g.M; ++i) {
      out[i] = decide(i, plans[i], sim, humans_at);
      const char c = out[i];
      const int hx = sim.humans[i].first;
      const int hy = sim.humans[i].second;
      if (c == 'u' || c == 'd' || c == 'l' || c == 'r') {
        const int d = strchr("udlr", c) - "udlr";
        const int tx = hx + DR[d];
        const int ty = hy + DC[d];
        if (sim.can_block(tx, ty, &humans_at))
          sim.blocked[tx][ty] = true;
      } else if (c == 'U' || c == 'D' || c == 'L' || c == 'R') {
        const int d = strchr("UDLR", c) - "UDLR";
        const int tx = hx + DR[d];
        const int ty = hy + DC[d];
        if (sim.can_move(tx, ty))
          sim.humans[i] = {tx, ty};
      }
    }

    for (int i = 0; i < g.M; ++i) {
      const char c = out[i];
      const int hx = g.humans[i].first;
      const int hy = g.humans[i].second;
      if (c == 'u' || c == 'd' || c == 'l' || c == 'r') {
        const int d = strchr("udlr", c) - "udlr";
        const int tx = hx + DR[d];
        const int ty = hy + DC[d];
        if (g.can_block(tx, ty, &humans_at))
          g.blocked[tx][ty] = true;
      } else if (c == 'U' || c == 'D' || c == 'L' || c == 'R') {
        const int d = strchr("UDLR", c) - "UDLR";
        const int tx = hx + DR[d];
        const int ty = hy + DC[d];
        if (g.can_move(tx, ty))
          g.humans[i] = {tx, ty};
      }
    }

    cout << out << '\n' << flush;

    for (int i = 0; i < g.N; ++i) {
      string s;
      cin >> s;
      for (const char ch : s) {
        if (ch == '.')
          break;
        for (int d = 0; d < 4; ++d) {
          if (MOVE_C[d] == ch) {
            g.pets[i].first += DR[d];
            g.pets[i].second += DC[d];
            break;
          }
        }
      }
    }
  }

  return 0;
}

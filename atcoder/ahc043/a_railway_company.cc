// AHC043 A - Railway Company: https://atcoder.jp/contests/ahc043/tasks/ahc043_a
//
// Sample-style solver: connect the first affordable home-work pair, then wait.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

using Pos = pair<int, int>;

namespace {

constexpr int EMPTY = -1;
constexpr int DO_NOTHING = -1;
constexpr int STATION = 0;
constexpr int RAIL_HORIZONTAL = 1;
constexpr int RAIL_VERTICAL = 2;
constexpr int RAIL_LEFT_DOWN = 3;
constexpr int RAIL_LEFT_UP = 4;
constexpr int RAIL_RIGHT_UP = 5;
constexpr int RAIL_RIGHT_DOWN = 6;
constexpr int COST_STATION = 5000;
constexpr int COST_RAIL = 100;

struct UnionFind {
  int n;
  vector<int> p;
  explicit UnionFind(int n) : n(n), p(n, -1) {}
  int find(int x) {
    return p[x] < 0 ? x : p[x] = find(p[x]);
  }
  bool same(int a, int b) {
    return find(a) == find(b);
  }
  void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b)
      return;
    if (p[a] > p[b])
      swap(a, b);
    p[b] += p[a];
    p[a] = b;
  }
};

int dist(const Pos& a, const Pos& b) {
  return abs(a.first - b.first) + abs(a.second - b.second);
}

struct Field {
  int N;
  vector<vector<int>> rail;
  UnionFind uf;
  explicit Field(int n) : N(n), rail(n, vector<int>(N, EMPTY)), uf(n * n) {}

  int idx(int r, int c) const {
    return r * N + c;
  }

  void build(int type, int r, int c) {
    rail[r][c] = type;
    auto try_unite = [&](int nr, int nc, initializer_list<int> ok) {
      if (nr < 0 || nr >= N || nc < 0 || nc >= N)
        return;
      for (const int t : ok) {
        if (rail[nr][nc] == t) {
          uf.unite(idx(r, c), idx(nr, nc));
          break;
        }
      }
    };
    if (type == STATION || type == RAIL_VERTICAL || type == RAIL_LEFT_UP || type == RAIL_RIGHT_UP)
      try_unite(r - 1, c, {STATION, RAIL_VERTICAL, RAIL_LEFT_DOWN, RAIL_RIGHT_DOWN});
    if (type == STATION || type == RAIL_VERTICAL || type == RAIL_LEFT_DOWN || type == RAIL_RIGHT_DOWN)
      try_unite(r + 1, c, {STATION, RAIL_VERTICAL, RAIL_LEFT_UP, RAIL_RIGHT_UP});
    if (type == STATION || type == RAIL_HORIZONTAL || type == RAIL_LEFT_DOWN || type == RAIL_LEFT_UP)
      try_unite(r, c - 1, {STATION, RAIL_HORIZONTAL, RAIL_RIGHT_DOWN, RAIL_RIGHT_UP});
    if (type == STATION || type == RAIL_HORIZONTAL || type == RAIL_RIGHT_DOWN || type == RAIL_RIGHT_UP)
      try_unite(r, c + 1, {STATION, RAIL_HORIZONTAL, RAIL_LEFT_DOWN, RAIL_LEFT_UP});
  }

  vector<Pos> collect_stations(const Pos& pos) const {
    vector<Pos> out;
    for (int dr = -2; dr <= 2; dr++) {
      for (int dc = -2; dc <= 2; dc++) {
        if (abs(dr) + abs(dc) > 2)
          continue;
        const int r = pos.first + dr, c = pos.second + dc;
        if (0 <= r && r < N && 0 <= c && c < N && rail[r][c] == STATION)
          out.emplace_back(r, c);
      }
    }
    return out;
  }

  bool connected(const Pos& s, const Pos& t) {
    const auto ss = collect_stations(s);
    const auto ts = collect_stations(t);
    for (const auto& a : ss) {
      for (const auto& b : ts) {
        if (uf.same(idx(a.first, a.second), idx(b.first, b.second)))
          return true;
      }
    }
    return false;
  }
};

struct Solver {
  int N, M, K, T;
  vector<Pos> home, work;
  Field field;
  long long money;
  vector<string> actions;

  Solver(int N, int M, int K, int T, vector<Pos> h, vector<Pos> w)
      : N(N), M(M), K(K), T(T), home(move(h)), work(move(w)), field(N), money(K) {}

  int income() const {
    int s = 0;
    for (int i = 0; i < M; i++) {
      if (field.connected(home[i], work[i]))
        s += dist(home[i], work[i]);
    }
    return s;
  }

  void rail(int type, int r, int c) {
    field.build(type, r, c);
    money -= COST_RAIL;
    actions.push_back(to_string(type) + " " + to_string(r) + " " + to_string(c));
  }

  void station(int r, int c) {
    field.build(STATION, r, c);
    money -= COST_STATION;
    actions.push_back("0 " + to_string(r) + " " + to_string(c));
  }

  void wait_turn() {
    actions.push_back("-1");
  }

  void connect_pair(int id) {
    const int r0 = home[id].first, c0 = home[id].second;
    const int r1 = work[id].first, c1 = work[id].second;
    station(r0, c0);
    station(r1, c1);
    if (r0 < r1) {
      for (int r = r0 + 1; r < r1; r++)
        rail(RAIL_VERTICAL, r, c0);
      if (c0 < c1)
        rail(RAIL_RIGHT_UP, r1, c0);
      else if (c0 > c1)
        rail(RAIL_LEFT_UP, r1, c0);
    } else if (r0 > r1) {
      for (int r = r0 - 1; r > r1; r--)
        rail(RAIL_VERTICAL, r, c0);
      if (c0 < c1)
        rail(RAIL_RIGHT_DOWN, r1, c0);
      else if (c0 > c1)
        rail(RAIL_LEFT_DOWN, r1, c0);
    }
    if (c0 < c1) {
      for (int c = c0 + 1; c < c1; c++)
        rail(RAIL_HORIZONTAL, r1, c);
    } else if (c0 > c1) {
      for (int c = c0 - 1; c > c1; c--)
        rail(RAIL_HORIZONTAL, r1, c);
    }
  }

  void solve() {
    const int rail_budget = (K - 2 * COST_STATION) / COST_RAIL;
    int pick = -1;
    for (int i = 0; i < M; i++) {
      if (dist(home[i], work[i]) - 1 <= rail_budget) {
        pick = i;
        break;
      }
    }
    if (pick >= 0)
      connect_pair(pick);
    const int inc = income();
    money += inc;
    while ((int)actions.size() < T) {
      wait_turn();
      money += inc;
    }
  }
};


}  // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M, K, T;
  cin >> N >> M >> K >> T;
  vector<Pos> home(M), work(M);
  for (int i = 0; i < M; i++) {
    cin >> home[i].first >> home[i].second >> work[i].first >> work[i].second;
  }

  Solver solver(N, M, K, T, move(home), move(work));
  solver.solve();
  for (const auto& line : solver.actions)
    cout << line << '\n';
  return 0;
}

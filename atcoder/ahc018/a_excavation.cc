// AHC018 A - Excavation: https://atcoder.jp/contests/ahc018/tasks/ahc018_a
//
// Diamond-grid graph + Dijkstra routing. Phase 1 probes key cells along shortest
// paths; phase 2 digs consecutive cells with power tuned from neighbor hardness.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int N = 200;
constexpr int MAX_H = 5000;

int W, K, C, CLEV;
vector<pair<int, int>> sources, houses;

int extracted[N][N];
int watered[N][N];
int dig_lo[N][N], dig_hi[N][N], dig_last[N][N];
bool done = false;

int c_lev(int c) {
  int lev = 0;
  while ((1 << lev) < c)
    ++lev;
  return lev;
}

bool dig(int r, int c, int p) {
  if (done) {
    extracted[r][c] = 1;
    return true;
  }
  cout << r << ' ' << c << ' ' << p << '\n' << flush;
  int rv;
  if (!(cin >> rv))
    exit(0);
  dig_last[r][c] = p;
  dig_lo[r][c] = dig_hi[r][c] + 1;
  dig_hi[r][c] += p;
  dig_lo[r][c] = min(dig_lo[r][c], MAX_H);
  dig_hi[r][c] = min(dig_hi[r][c], MAX_H);
  if (rv == 1 || rv == 2) {
    extracted[r][c] = 1;
    dig_lo[r][c] = max(dig_lo[r][c], 10);
  }
  if (rv == 2)
    done = true;
  return rv == 2;
}

void crush(int r, int c, double scan_pow, int scan_start) {
  int p = dig_last[r][c] ? min((int)(dig_last[r][c] * scan_pow), 40 * C)
                           : max(16, C) * scan_start / 16;
  while (!extracted[r][c]) {
    p = min(p, MAX_H - dig_hi[r][c]);
    p = max(p, 1);
    dig(r, c, p);
    p = min((int)(p * scan_pow), MAX_H - dig_hi[r][c]);
  }
}

int best_power(int lo, int hi) {
  const int exp = (lo + hi) / 2;
  static const double waste_mul[] = {1.2, 1.2, 1.6, 2.3, 3.2, 3.2, 4.3, 7.0};
  double best = 1e18;
  int power = 1;
  for (int p = 1; p <= 1000; ++p) {
    const double v_waste = p * 0.5;
    const double c_waste = (exp + p - 1.0) / p * C;
    const double av = v_waste * waste_mul[CLEV] + c_waste;
    if (av < best) {
      best = av;
      power = p;
    }
  }
  return power;
}

int edge_cost(int r1, int c1, int r2, int c2) {
  const int d = abs(r1 - r2) + abs(c1 - c2);
  if (d == 0)
    return 0;
  auto cell_cost = [](int r, int c) {
    if (extracted[r][c])
      return (dig_lo[r][c] + dig_hi[r][c]) / 2 + 10;
    return 160 + (int)(dig_hi[r][c] * (105 - 4 * CLEV) /
                       (log(dig_hi[r][c] + 1.0) + 1.0));
  };
  const int v1 = cell_cost(r1, c1);
  const int v2 = cell_cost(r2, c2);
  return (int)((d - 1) * (C + (v1 + v2) * 0.5) + (extracted[r1][c1] ? 0 : C) +
               (extracted[r2][c2] ? 0 : C));
}

struct Graph {
  vector<pair<int, int>> verts;
  vector<vector<int>> adj;
  vector<pair<int, int>> start;
  map<pair<int, int>, int> id;

  void clear() {
    verts.clear();
    adj.clear();
    start.clear();
    id.clear();
  }

  int add_vert(pair<int, int> p) {
    if (id.count(p))
      return id[p];
    const int v = (int)verts.size();
    id[p] = v;
    verts.push_back(p);
    adj.emplace_back();
    start.emplace_back(-1, -1);
    return v;
  }

  void add_edge(pair<int, int> p1, pair<int, int> p2) {
    const int v1 = add_vert(p1);
    const int v2 = add_vert(p2);
    for (int u : adj[v1])
      if (u == v2)
        return;
    adj[v1].push_back(v2);
    adj[v2].push_back(v1);
  }

  void build(int scan_dist, const vector<pair<int, int>>& targets) {
    clear();
    int scan_sum = 1;
    while (scan_sum + scan_dist < N)
      scan_sum += scan_dist;
    const int off = (N - scan_sum) / 2;

    for (int r = off; r < N; r += scan_dist / 2) {
      for (int c = off; c < N; c += scan_dist / 2) {
        if (r % scan_dist != c % scan_dist)
          continue;
        if (r < N - scan_dist)
          add_edge({r, c}, {r + scan_dist, c});
        if (c < N - scan_dist)
          add_edge({r, c}, {r, c + scan_dist});
        if (r < N - scan_dist / 2 && c > scan_dist / 2)
          add_edge({r, c}, {r + scan_dist / 2, c - scan_dist / 2});
        if (r < N - scan_dist / 2 && c < N - scan_dist / 2)
          add_edge({r, c}, {r + scan_dist / 2, c + scan_dist / 2});
        for (const auto& t : targets) {
          if (abs(r - t.first) < scan_dist && abs(c - t.second) < scan_dist &&
              abs(r - t.first) + abs(c - t.second) > 0)
            add_edge({r, c}, t);
        }

        pair<int, int> best_start{-1, -1};
        int best = 1e9;
        for (int nr = max(0, r - scan_dist + 1); nr < min(N, r + scan_dist); ++nr) {
          for (int nc = max(0, c - scan_dist + 1); nc < min(N, c + scan_dist); ++nc) {
            const int dist = abs(r - nr) + abs(c - nc);
            if (dist >= scan_dist)
              continue;
            bool ok = watered[nr][nc];
            for (const auto& s : sources)
              ok |= s == pair<int, int>{nr, nc};
            if (!ok)
              continue;
            const int av = edge_cost(r, c, nr, nc);
            if (av < best) {
              best = av;
              best_start = {nr, nc};
            }
          }
        }
        start[id[{r, c}]] = best_start;
      }
    }
  }

  vector<pair<int, int>> shortest_path(const vector<pair<int, int>>& targets) {
    const int m = (int)verts.size();
    vector<int> dist(m, 1e9), par(m, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    for (int i = 0; i < m; ++i) {
      if (start[i].first == -1)
        continue;
      const auto [sr, sc] = start[i];
      const auto [vr, vc] = verts[i];
      const int d = edge_cost(vr, vc, sr, sc);
      dist[i] = d;
      par[i] = -1;
      pq.push({d, i});
    }

    int goal = -1;
    while (!pq.empty()) {
      auto [d, u] = pq.top();
      pq.pop();
      if (d > dist[u])
        continue;
      for (const auto& t : targets) {
        if (u == id[t]) {
          goal = u;
          break;
        }
      }
      if (goal != -1)
        break;
      for (int v : adj[u]) {
        const auto [r1, c1] = verts[u];
        const auto [r2, c2] = verts[v];
        const int nd = d + edge_cost(r1, c1, r2, c2);
        if (nd < dist[v]) {
          dist[v] = nd;
          par[v] = u;
          pq.push({nd, v});
        }
      }
    }

    vector<pair<int, int>> path;
    if (goal == -1)
      return path;
    int cur = goal;
    while (par[cur] != -1) {
      path.push_back(verts[cur]);
      cur = par[cur];
    }
    path.push_back(verts[cur]);
    path.push_back(start[cur]);
    reverse(path.begin(), path.end());
    return path;
  }
};

vector<pair<int, int>> rect_path(pair<int, int> a, pair<int, int> b) {
  vector<pair<int, int>> path{a};
  while (path.back() != b) {
    const auto [r, c] = path.back();
    if (abs(r - b.first) > abs(c - b.second))
      path.push_back({r + (b.first > r ? 1 : -1), c});
    else
      path.push_back({r, c + (b.second > c ? 1 : -1)});
  }
  return path;
}

void adjust_bounds(int r, int c) {
  const double ratio = C <= 2 ? 2.0 : C <= 16 ? 1.5 : C <= 32 ? 1.4 : C <= 64 ? 1.3 : 1.2;
  for (int nr = r - 1; nr <= r + 1; ++nr) {
    for (int nc = c - 1; nc <= c + 1; ++nc) {
      if (nr < 0 || nr >= N || nc < 0 || nc >= N || !extracted[nr][nc])
        continue;
      dig_lo[r][c] = max(dig_lo[r][c], (int)(dig_lo[nr][nc] / ratio));
      dig_hi[r][c] = min(dig_hi[r][c], (int)(dig_hi[nr][nc] * 2.1));
    }
  }
}

void dig_along(const vector<pair<int, int>>& fpath) {
  static const double last_v_mul[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.025, 1.05, 1.075};
  static const int min_dig[] = {0, 0, 0, 12, 13, 15, 19, 25};
  pair<int, int> prev{-1, -1};
  for (const auto& cur : fpath) {
    int power = prev.first == -1 ? 2 * C + 30
                                 : best_power(dig_lo[prev.first][prev.second],
                                              dig_hi[prev.first][prev.second]);
    const int last_v =
        prev.first == -1 ? 0 : (dig_lo[prev.first][prev.second] + dig_hi[prev.first][prev.second]) / 2;
    int step = 0;
    while (!extracted[cur.first][cur.second]) {
      ++step;
      int cur_power = power;
      if (last_v && last_v > power) {
        if (step == 1)
          cur_power = (int)(last_v * last_v_mul[CLEV]);
        else
          cur_power = (int)(power * 0.45);
      }
      cur_power = max(min_dig[CLEV], cur_power);
      cur_power = min(cur_power, MAX_H - dig_hi[cur.first][cur.second]);
      dig(cur.first, cur.second, max(cur_power, 1));
    }
    adjust_bounds(cur.first, cur.second);
    watered[cur.first][cur.second] = 1;
    prev = cur;
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n >> W >> K >> C;
  CLEV = c_lev(C);
  sources.resize(W);
  houses.resize(K);
  for (auto& [r, c] : sources)
    cin >> r >> c;
  for (auto& [r, c] : houses)
    cin >> r >> c;

  const double scan_pow = 1.07 + 0.085 * CLEV;
  static const int scan_start[] = {16, 13, 20, 20, 23, 17, 15, 11};

  for (const auto& h : houses)
    crush(h.first, h.second, scan_pow, scan_start[CLEV]);

  Graph g;
  while (!done) {
    vector<pair<int, int>> left;
    for (const auto& h : houses)
      if (!watered[h.first][h.second])
        left.push_back(h);
    if (left.empty())
      break;

    g.build(18, left);
    while (!done) {
      const auto path = g.shortest_path(left);
      pair<int, int> target{-1, -1};
      int best = INT_MIN;
      for (const auto& p : path) {
        if (extracted[p.first][p.second])
          continue;
        const int av = -dig_hi[p.first][p.second];
        if (av >= best) {
          best = av;
          target = p;
        }
      }

      if (target.first == -1) {
        if (path.size() < 2)
          break;
        vector<pair<int, int>> fpath{path[0]};
        for (size_t i = 1; i < path.size(); ++i) {
          const auto seg = rect_path(fpath.back(), path[i]);
          for (size_t j = 1; j < seg.size(); ++j)
            fpath.push_back(seg[j]);
        }
        dig_along(fpath);
        break;
      }

      int p = dig_last[target.first][target.second]
                  ? min((int)(dig_last[target.first][target.second] * scan_pow), 40 * C)
                  : max(16, C) * scan_start[CLEV] / 16;
      p = min(p, MAX_H - dig_hi[target.first][target.second]);
      dig(target.first, target.second, max(p, 1));
    }
  }

  return 0;
}

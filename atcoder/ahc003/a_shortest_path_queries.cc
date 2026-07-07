// AHC003 A - Shortest Path Queries: https://atcoder.jp/contests/ahc003/tasks/ahc003_a
//
// Hybrid of kmyk dual predictors and edge refitting: M1 (uniform row/col) and M2
// (two-segment row/col) compete on historical loss; after each query both SA-refit
// and the taken edges are scaled + piecewise L1-refit on the live grid.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int N = 30;
constexpr int Q = 1000;
constexpr int VALUE_MIN = 1000;
constexpr int VALUE_MAX = 9000;
constexpr int VALUE_CENTER = (VALUE_MAX + VALUE_MIN) / 2;

using Horiz = array<array<i64, N - 1>, N>;
using Vert = array<array<i64, N - 1>, N>;

vector<vector<int>> H, V;

i64 edge_cost(const Horiz& hr, const Vert& vr, int ay, int ax, int by, int bx) {
  if (ay == by)
    return hr[ay][min(ax, bx)];
  return vr[ax][min(ay, by)];
}

void step(char dir, int& r, int& c) {
  switch (dir) {
  case 'U': --r; break;
  case 'D': ++r; break;
  case 'L': --c; break;
  case 'R': ++c; break;
  }
}

char reverse_move(char ch) {
  switch (ch) {
  case 'U': return 'D';
  case 'D': return 'U';
  case 'L': return 'R';
  case 'R': return 'L';
  default: return ch;
  }
}

i64 path_score(const vector<pair<int, int>>& path, const Horiz& hr, const Vert& vr) {
  i64 sum = 0;
  for (int i = 0; i + 1 < (int)path.size(); ++i) {
    const auto [ay, ax] = path[i];
    const auto [by, bx] = path[i + 1];
    sum += edge_cost(hr, vr, ay, ax, by, bx);
  }
  return sum;
}

string path_to_command(const vector<pair<int, int>>& path) {
  string cmd;
  for (int i = 0; i + 1 < (int)path.size(); ++i) {
    const auto [ay, ax] = path[i];
    const auto [by, bx] = path[i + 1];
    if (by == ay - 1 && bx == ax)
      cmd.push_back('U');
    else if (by == ay + 1 && bx == ax)
      cmd.push_back('D');
    else if (by == ay && bx == ax + 1)
      cmd.push_back('R');
    else
      cmd.push_back('L');
  }
  return cmd;
}

Horiz live_horiz() {
  Horiz hr{};
  for (int r = 0; r < N; ++r)
    for (int c = 0; c < N - 1; ++c)
      hr[r][c] = H[r][c];
  return hr;
}

Vert live_vert() {
  Vert vr{};
  for (int c = 0; c < N; ++c)
    for (int r = 0; r < N - 1; ++r)
      vr[c][r] = V[r][c];
  return vr;
}

vector<pair<int, int>> dijkstra(int sy, int sx, int ty, int tx, const Horiz& hr,
                                const Vert& vr, double explore,
                                const vector<double>& row_vis,
                                const vector<double>& col_vis) {
  constexpr i64 INF = (1LL << 60);
  constexpr int DY[4] = {-1, 1, 0, 0};
  constexpr int DX[4] = {0, 0, 1, -1};

  array<array<i64, N>, N> dist{};
  array<array<pair<int, int>, N>, N> parent{};
  for (auto& row : dist)
    row.fill(INF);
  for (auto& row : parent)
    row.fill({-1, -1});

  using State = tuple<i64, int, int>;
  priority_queue<State, vector<State>, greater<State>> pq;
  dist[sy][sx] = 0;
  pq.emplace(0, sy, sx);

  while (!pq.empty()) {
    auto [d, y, x] = pq.top();
    pq.pop();
    if (d != dist[y][x])
      continue;
    for (int k = 0; k < 4; ++k) {
      const int ny = y + DY[k];
      const int nx = x + DX[k];
      if (ny < 0 || ny >= N || nx < 0 || nx >= N)
        continue;
      i64 w = edge_cost(hr, vr, y, x, ny, nx);
      if (explore > 0) {
        const double vis = (k <= 1) ? col_vis[nx] : row_vis[ny];
        const double blend = explore * max(0.0, 1.0 - vis * 3.0);
        w = (i64)(w * (1.0 - blend) + VALUE_CENTER * blend + 0.5);
      }
      const i64 nd = d + w;
      if (nd < dist[ny][nx]) {
        dist[ny][nx] = nd;
        parent[ny][nx] = {y, x};
        pq.emplace(nd, ny, nx);
      }
    }
  }

  vector<pair<int, int>> path;
  int y = ty, x = tx;
  path.emplace_back(y, x);
  while (parent[y][x] != pair<int, int>{-1, -1}) {
    tie(y, x) = parent[y][x];
    path.emplace_back(y, x);
  }
  reverse(path.begin(), path.end());
  return path;
}

void record_visits(const vector<pair<int, int>>& path, vector<double>& row_vis,
                   vector<double>& col_vis) {
  int r = path.front().first, c = path.front().second;
  const double inv = 1.0 / max(1, (int)path.size() - 1);
  for (int i = 1; i < (int)path.size(); ++i) {
    const auto [nr, nc] = path[i];
    if (nr == r)
      row_vis[r] += inv;
    else
      col_vis[c] += inv;
    r = nr;
    c = nc;
  }
}

void update_weights(const vector<pair<int, int>>& path, i64 measured) {
  const i64 est = path_score(path, live_horiz(), live_vert());
  if (est <= 0)
    return;
  const double factor = (double)measured / est;

  for (int i = 0; i + 1 < (int)path.size(); ++i) {
    const auto [ay, ax] = path[i];
    const auto [by, bx] = path[i + 1];
    if (ay == by) {
      const int c = min(ax, bx);
      H[ay][c] = max(1, (int)(H[ay][c] * factor + 0.5));
    } else {
      const int r = min(ay, by);
      V[r][ax] = max(1, (int)(V[r][ax] * factor + 0.5));
    }
  }
}

long long segment_cost(const vector<int>& w, int lo, int hi, int mean) {
  long long c = 0;
  for (int j = lo; j <= hi; ++j)
    c += abs(w[j] - mean);
  return c;
}

int segment_mean(const vector<int>& w, int lo, int hi) {
  long long s = 0;
  for (int j = lo; j <= hi; ++j)
    s += w[j];
  return (int)(s / (hi - lo + 1));
}

void fit_row(int i) {
  const vector<int> w = H[i];
  const int m1 = segment_mean(w, 0, N - 2);
  long long best = segment_cost(w, 0, N - 2, m1);
  vector<int> out(N - 1, m1);

  for (int split = 1; split < N - 1; ++split) {
    const int a = segment_mean(w, 0, split - 1);
    const int b = segment_mean(w, split, N - 2);
    const long long c =
        segment_cost(w, 0, split - 1, a) + segment_cost(w, split, N - 2, b);
    if (c < best) {
      best = c;
      for (int j = 0; j < split; ++j)
        out[j] = a;
      for (int j = split; j < N - 1; ++j)
        out[j] = b;
    }
  }
  H[i] = std::move(out);
}

void fit_col(int j) {
  vector<int> w(N - 1);
  for (int i = 0; i < N - 1; ++i)
    w[i] = V[i][j];
  const int m1 = segment_mean(w, 0, N - 2);
  long long best = segment_cost(w, 0, N - 2, m1);
  vector<int> out(N - 1, m1);

  for (int split = 1; split < N - 1; ++split) {
    const int a = segment_mean(w, 0, split - 1);
    const int b = segment_mean(w, split, N - 2);
    const long long c =
        segment_cost(w, 0, split - 1, a) + segment_cost(w, split, N - 2, b);
    if (c < best) {
      best = c;
      for (int i = 0; i < split; ++i)
        out[i] = a;
      for (int i = split; i < N - 1; ++i)
        out[i] = b;
    }
  }
  for (int i = 0; i < N - 1; ++i)
    V[i][j] = out[i];
}

void refit_all() {
  for (int i = 0; i < N; ++i)
    fit_row(i);
  for (int j = 0; j < N; ++j)
    fit_col(j);
}

struct RowColHistory {
  vector<vector<pair<int, int>>> paths;
  vector<i64> actual;
  array<vector<pair<int, int>>, N> used_row;
  array<vector<pair<int, int>>, N> used_col;

  void add(const vector<pair<int, int>>& path, i64 score) {
    const int j = (int)paths.size();
    paths.push_back(path);
    actual.push_back(score);
    auto touch = [&](vector<pair<int, int>>& used) {
      if (used.empty() || used.back().first != j)
        used.emplace_back(j, 0);
      used.back().second++;
    };
    for (int i = 0; i + 1 < (int)path.size(); ++i) {
      const auto [ay, ax] = path[i];
      const auto [by, bx] = path[i + 1];
      if (ay == by)
        touch(used_row[ay]);
      else
        touch(used_col[ax]);
    }
  }
};

struct UniformPredictor {
  RowColHistory history;
  array<i64, N> row{};
  array<i64, N> col{};
  array<i64, N> best_row{};
  array<i64, N> best_col{};
  vector<i64> predicted;
  vector<i64> best_predicted;
  i64 loss = 0;
  i64 best_loss = 0;

  UniformPredictor() {
    row.fill(VALUE_CENTER);
    col.fill(VALUE_CENTER);
    best_row = row;
    best_col = col;
  }

  pair<Horiz, Vert> weights() const {
    Horiz hr{};
    Vert vr{};
    for (int y = 0; y < N; ++y)
      hr[y].fill(best_row[y]);
    for (int x = 0; x < N; ++x)
      vr[x].fill(best_col[x]);
    return {hr, vr};
  }

  i64 avg_loss() const {
    return history.paths.empty() ? 0 : best_loss / (i64)history.paths.size();
  }

  void sync_from_grid() {
    for (int y = 0; y < N; ++y) {
      long long s = 0;
      for (int x = 0; x < N - 1; ++x)
        s += H[y][x];
      row[y] = best_row[y] = s / (N - 1);
    }
    for (int x = 0; x < N; ++x) {
      long long s = 0;
      for (int y = 0; y < N - 1; ++y)
        s += V[y][x];
      col[x] = best_col[x] = s / (N - 1);
    }
  }

  void add(const vector<pair<int, int>>& path, i64 score) {
    history.add(path, score);
    const auto [hr, vr] = weights();
    const i64 pred = path_score(path, hr, vr);
    predicted.push_back(pred);
    best_predicted.push_back(pred);
    const i64 delta = llabs(pred - score);
    loss += delta;
    best_loss += delta;
  }

  void anneal(mt19937& rng, chrono::steady_clock::time_point deadline) {
    const auto start = chrono::steady_clock::now();
    row = best_row;
    col = best_col;
    predicted = best_predicted;
    loss = best_loss;

    for (int iter = 0;; ++iter) {
      if (iter % 32 == 0 && chrono::steady_clock::now() >= deadline)
        break;

      const bool is_row = bernoulli_distribution(0.5)(rng);
      const int z = uniform_int_distribution<int>(0, N - 1)(rng);
      i64 d = uniform_int_distribution<int>(-200, 200)(rng);
      auto& value = is_row ? row[z] : col[z];
      if (value + d < VALUE_MIN + 300)
        d = VALUE_MIN + 300 - value;
      else if (value + d > VALUE_MAX - 300)
        d = VALUE_MAX - 300 - value;
      if (d == 0)
        continue;

      const auto& used = is_row ? history.used_row[z] : history.used_col[z];
      i64 delta = 0;
      for (const auto& [j, cnt] : used) {
        delta -= llabs(predicted[j] - history.actual[j]);
        delta += llabs(predicted[j] + cnt * d - history.actual[j]);
      }

      const auto now = chrono::steady_clock::now();
      const double temp = max(0.0, chrono::duration<double>(deadline - now).count() /
                                        chrono::duration<double>(deadline - start).count());
      if (delta <= 0 || bernoulli_distribution(max(0.0, 1.0 - delta / 3000.0) * temp)(rng)) {
        loss += delta;
        value += d;
        for (const auto& [j, cnt] : used)
          predicted[j] += cnt * d;
        if (loss < best_loss) {
          best_loss = loss;
          best_row = row;
          best_col = col;
          best_predicted = predicted;
        }
      }
    }
  }
};

struct SplitPredictor {
  struct State {
    array<int, N> sep_x{};
    array<int, N> sep_y{};
    array<i64, N> row1{};
    array<i64, N> row2{};
    array<i64, N> col1{};
    array<i64, N> col2{};
    vector<i64> predicted;
    i64 loss = 0;
  };

  vector<vector<pair<int, int>>> paths;
  vector<i64> actual;
  array<array<vector<int>, N - 1>, N> used_hr{};
  array<array<vector<int>, N - 1>, N> used_vr{};
  State cur, best;

  SplitPredictor() {
    cur.row1.fill(VALUE_CENTER);
    cur.row2.fill(VALUE_CENTER);
    cur.col1.fill(VALUE_CENTER);
    cur.col2.fill(VALUE_CENTER);
    cur.sep_x.fill(N / 2);
    cur.sep_y.fill(N / 2);
    best = cur;
  }

  pair<Horiz, Vert> weights(const State& st) const {
    Horiz hr{};
    Vert vr{};
    for (int y = 0; y < N; ++y)
      for (int x = 0; x < N - 1; ++x)
        hr[y][x] = x < st.sep_x[y] ? st.row1[y] : st.row2[y];
    for (int x = 0; x < N; ++x)
      for (int y = 0; y < N - 1; ++y)
        vr[x][y] = y < st.sep_y[x] ? st.col1[x] : st.col2[x];
    return {hr, vr};
  }

  pair<Horiz, Vert> best_weights() const { return weights(best); }

  i64 avg_loss() const {
    return paths.empty() ? 0 : best.loss / (i64)paths.size();
  }

  void sync_from_grid() {
    for (int y = 0; y < N; ++y) {
      int best_split = 1;
      long long best_cost = LLONG_MAX;
      for (int split = 1; split < N - 1; ++split) {
        long long s1 = 0, s2 = 0;
        for (int x = 0; x < split; ++x)
          s1 += H[y][x];
        for (int x = split; x < N - 1; ++x)
          s2 += H[y][x];
        const int m1 = (int)(s1 / split);
        const int m2 = (int)(s2 / (N - 1 - split));
        long long cost = 0;
        for (int x = 0; x < split; ++x)
          cost += abs(H[y][x] - m1);
        for (int x = split; x < N - 1; ++x)
          cost += abs(H[y][x] - m2);
        if (cost < best_cost) {
          best_cost = cost;
          best_split = split;
          cur.row1[y] = best.row1[y] = m1;
          cur.row2[y] = best.row2[y] = m2;
        }
      }
      cur.sep_x[y] = best.sep_x[y] = best_split;
    }
    for (int x = 0; x < N; ++x) {
      int best_split = 1;
      long long best_cost = LLONG_MAX;
      for (int split = 1; split < N - 1; ++split) {
        long long s1 = 0, s2 = 0;
        for (int y = 0; y < split; ++y)
          s1 += V[y][x];
        for (int y = split; y < N - 1; ++y)
          s2 += V[y][x];
        const int m1 = (int)(s1 / split);
        const int m2 = (int)(s2 / (N - 1 - split));
        long long cost = 0;
        for (int y = 0; y < split; ++y)
          cost += abs(V[y][x] - m1);
        for (int y = split; y < N - 1; ++y)
          cost += abs(V[y][x] - m2);
        if (cost < best_cost) {
          best_cost = cost;
          best_split = split;
          cur.col1[x] = best.col1[x] = m1;
          cur.col2[x] = best.col2[x] = m2;
        }
      }
      cur.sep_y[x] = best.sep_y[x] = best_split;
    }
  }

  void add(const vector<pair<int, int>>& path, i64 score) {
    const int j = (int)paths.size();
    paths.push_back(path);
    actual.push_back(score);
    for (int i = 0; i + 1 < (int)path.size(); ++i) {
      const auto [ay, ax] = path[i];
      const auto [by, bx] = path[i + 1];
      if (ay == by)
        used_hr[ay][min(ax, bx)].push_back(j);
      else
        used_vr[ax][min(ay, by)].push_back(j);
    }
    auto ingest = [&](State& st) {
      const auto [hr, vr] = weights(st);
      const i64 pred = path_score(path, hr, vr);
      st.predicted.push_back(pred);
      st.loss += llabs(pred - score);
    };
    ingest(cur);
    ingest(best);
  }

  void anneal(mt19937& rng, chrono::steady_clock::time_point deadline) {
    const auto start = chrono::steady_clock::now();
    cur = best;
    for (int iter = 0;; ++iter) {
      if (iter % 32 == 0 && chrono::steady_clock::now() >= deadline)
        break;

      const auto prob = [&](i64 delta) {
        const auto now = chrono::steady_clock::now();
        const double temp = max(0.0, chrono::duration<double>(deadline - now).count() /
                                          chrono::duration<double>(deadline - start).count());
        return max(0.0, 1.0 - delta / 3000.0) * temp;
      };

      if (bernoulli_distribution(0.7)(rng)) {
        const int mode = uniform_int_distribution<int>(0, 3)(rng);
        const int z = uniform_int_distribution<int>(0, N - 1)(rng);
        i64 d = uniform_int_distribution<int>(-200, 200)(rng);
        auto& value = (mode == 0)   ? cur.row1[z]
                      : (mode == 1) ? cur.row2[z]
                      : (mode == 2) ? cur.col1[z]
                                    : cur.col2[z];
        if (value + d < VALUE_MIN + 300)
          d = VALUE_MIN + 300 - value;
        else if (value + d > VALUE_MAX - 300)
          d = VALUE_MAX - 300 - value;
        if (d == 0)
          continue;

        i64 delta = 0;
        if (mode <= 1) {
          for (int x = 0; x < N - 1; ++x) {
            if ((mode == 0 && x < cur.sep_x[z]) || (mode == 1 && x >= cur.sep_x[z])) {
              for (int j : used_hr[z][x]) {
                delta -= llabs(cur.predicted[j] - actual[j]);
                delta += llabs(cur.predicted[j] + d - actual[j]);
              }
            }
          }
        } else {
          for (int y = 0; y < N - 1; ++y) {
            if ((mode == 2 && y < cur.sep_y[z]) || (mode == 3 && y >= cur.sep_y[z])) {
              for (int j : used_vr[z][y]) {
                delta -= llabs(cur.predicted[j] - actual[j]);
                delta += llabs(cur.predicted[j] + d - actual[j]);
              }
            }
          }
        }

        if (delta <= 0 || bernoulli_distribution(prob(delta))(rng)) {
          cur.loss += delta;
          value += d;
          if (mode <= 1) {
            for (int x = 0; x < N - 1; ++x) {
              if ((mode == 0 && x < cur.sep_x[z]) || (mode == 1 && x >= cur.sep_x[z])) {
                for (int j : used_hr[z][x])
                  cur.predicted[j] += d;
              }
            }
          } else {
            for (int y = 0; y < N - 1; ++y) {
              if ((mode == 2 && y < cur.sep_y[z]) || (mode == 3 && y >= cur.sep_y[z])) {
                for (int j : used_vr[z][y])
                  cur.predicted[j] += d;
              }
            }
          }
          if (cur.loss < best.loss)
            best = cur;
        }
      } else {
        const bool is_row = bernoulli_distribution(0.5)(rng);
        const int z = uniform_int_distribution<int>(0, N - 1)(rng);
        const int step = bernoulli_distribution(0.5)(rng) ? 1 : -1;
        auto& sep = is_row ? cur.sep_x[z] : cur.sep_y[z];
        const int nsep = sep + step;
        if (nsep < 1 || nsep > N - 2)
          continue;
        const i64 d = (sep < nsep) ? (is_row ? cur.row2[z] - cur.row1[z]
                                             : cur.col2[z] - cur.col1[z])
                                   : (is_row ? cur.row1[z] - cur.row2[z]
                                             : cur.col1[z] - cur.col2[z]);
        i64 delta = 0;
        const int lo = min(sep, nsep);
        const int hi = max(sep, nsep);
        if (is_row) {
          for (int k = lo; k < hi; ++k)
            for (int j : used_hr[z][k]) {
              delta -= llabs(cur.predicted[j] - actual[j]);
              delta += llabs(cur.predicted[j] + d - actual[j]);
            }
        } else {
          for (int k = lo; k < hi; ++k)
            for (int j : used_vr[z][k]) {
              delta -= llabs(cur.predicted[j] - actual[j]);
              delta += llabs(cur.predicted[j] + d - actual[j]);
            }
        }
        if (delta <= 0 || bernoulli_distribution(prob(delta))(rng)) {
          cur.loss += delta;
          if (is_row) {
            for (int k = lo; k < hi; ++k)
              for (int j : used_hr[z][k])
                cur.predicted[j] += d;
          } else {
            for (int k = lo; k < hi; ++k)
              for (int j : used_vr[z][k])
                cur.predicted[j] += d;
          }
          sep = nsep;
          if (cur.loss < best.loss)
            best = cur;
        }
      }
    }
  }
};

void solve() {
  H.assign(N, vector<int>(N - 1, VALUE_CENTER));
  V.assign(N - 1, vector<int>(N, VALUE_CENTER));
  vector<double> row_vis(N, 0), col_vis(N, 0);

  mt19937 rng(20210425);
  const auto t_begin = chrono::steady_clock::now();
  const auto t_end = t_begin + chrono::milliseconds(1900);

  UniformPredictor m1;
  SplitPredictor m2;

  for (int query = 0; query < Q; ++query) {
    int sy, sx, ty, tx;
    if (!(cin >> sy >> sx >> ty >> tx))
      break;

    const double explore = (query < 150) ? 0.35 * (1.0 - (double)query / 150.0) : 0.0;

    const bool use_m1 = m1.avg_loss() < m2.avg_loss() + 1500;
    const auto [phr, pvr] = use_m1 ? m1.weights() : m2.best_weights();
    const auto [lhr, lvr] = pair<Horiz, Vert>{live_horiz(), live_vert()};

    auto path_pred =
        dijkstra(sy, sx, ty, tx, phr, pvr, explore, row_vis, col_vis);
    auto path_live =
        dijkstra(sy, sx, ty, tx, lhr, lvr, explore, row_vis, col_vis);

    const vector<pair<int, int>> path =
        (query < 80 || path_score(path_pred, phr, pvr) <= path_score(path_live, lhr, lvr))
            ? path_pred
            : path_live;

    cout << path_to_command(path) << '\n' << flush;

    i64 measured = 0;
    cin >> measured;

    record_visits(path, row_vis, col_vis);
    update_weights(path, measured);
    refit_all();
    m1.sync_from_grid();
    m2.sync_from_grid();
    m1.add(path, measured);
    m2.add(path, measured);

    const auto budget = [&](int slot, int slots) {
      return t_begin + (t_end - t_begin) * slot / slots;
    };
    m1.anneal(rng, budget(2 * query + 1, 2 * Q));
    m2.anneal(rng, budget(2 * query + 2, 2 * Q));
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  solve();
  return 0;
}

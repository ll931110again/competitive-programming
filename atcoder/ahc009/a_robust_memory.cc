// AHC009 A - Robust Memory of Commuting Routes:
// https://atcoder.jp/contests/ahc009/tasks/ahc009_a
//
// Random-weight Dijkstra base path, duplicate moves for forget-resilience, then
// simulated annealing (insert/delete/change/copy) under exact expected-score DP.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int N = 20;
constexpr int L = 200;
constexpr int MAXV = N * N;
constexpr int dr[4] = {-1, 0, 1, 0};
constexpr int dc[4] = {0, -1, 0, 1};
constexpr char DCH[4] = {'U', 'L', 'D', 'R'};

int si, sj, ti, tj;
double P;
bool hs[N][N - 1];
bool vs[N - 1][N];
mt19937 rng;

inline int vid(int r, int c) { return r * N + c; }

bool can_move(int r, int c, int d) {
  switch (d) {
    case 0:
      return r > 0 && !vs[r - 1][c];
    case 1:
      return c > 0 && !hs[r][c - 1];
    case 2:
      return r + 1 < N && !vs[r][c];
    case 3:
      return c + 1 < N && !hs[r][c];
  }
  return false;
}

int dir_of(char c) {
  for (int d = 0; d < 4; ++d)
    if (DCH[d] == c)
      return d;
  return -1;
}

i64 score_of(const string& s) {
  if ((int)s.size() > L)
    return 0;
  static double cur[N][N];
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      cur[i][j] = 0.0;
  cur[si][sj] = 1.0;
  double sum = 0.0;
  const int len = (int)s.size();
  for (int t = 0; t < len; ++t) {
    const int d = dir_of(s[t]);
    if (d < 0)
      return 0;
    static double nxt[N][N];
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        nxt[i][j] = 0.0;
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        const double p = cur[i][j];
        if (p == 0.0)
          continue;
        if (can_move(i, j, d)) {
          const int ni = i + dr[d], nj = j + dc[d];
          nxt[ni][nj] += p * (1.0 - P);
          nxt[i][j] += p * P;
        } else {
          nxt[i][j] += p;
        }
      }
    }
    memcpy(cur, nxt, sizeof(cur));
    sum += cur[ti][tj] * (2 * L - t);
    cur[ti][tj] = 0.0;
  }
  return (i64)llround(1e8 * sum / (2.0 * L));
}

vector<int> dijkstra_path(int w_lo, int w_hi) {
  vector<int> w(MAXV * 4, 0);
  for (int v = 0; v < MAXV; ++v) {
    const int r = v / N, c = v % N;
    for (int d = 0; d < 4; ++d) {
      if (!can_move(r, c, d))
        continue;
      w[v * 4 + d] = uniform_int_distribution<int>(w_lo, w_hi)(rng);
    }
  }
  const int S = vid(si, sj), T = vid(ti, tj);
  const i64 INF = (1LL << 60);
  vector<i64> dist(MAXV, INF);
  vector<int> par_v(MAXV, -1), par_d(MAXV, -1);
  using State = pair<i64, int>;
  priority_queue<State, vector<State>, greater<State>> pq;
  dist[S] = 0;
  pq.emplace(0, S);
  while (!pq.empty()) {
    auto [d, v] = pq.top();
    pq.pop();
    if (d != dist[v])
      continue;
    if (v == T)
      break;
    const int r = v / N, c = v % N;
    for (int dirc = 0; dirc < 4; ++dirc) {
      if (!can_move(r, c, dirc))
        continue;
      const int wgt = w[v * 4 + dirc];
      if (wgt <= 0)
        continue;
      const int u = vid(r + dr[dirc], c + dc[dirc]);
      if (dist[u] > d + wgt) {
        dist[u] = d + wgt;
        par_v[u] = v;
        par_d[u] = dirc;
        pq.emplace(dist[u], u);
      }
    }
  }
  if (dist[T] == INF)
    return {};
  vector<int> rev;
  for (int v = T; v != S; v = par_v[v])
    rev.push_back(par_d[v]);
  reverse(rev.begin(), rev.end());
  return rev;
}

string dirs_to_string(const vector<int>& dirs) {
  string s;
  s.reserve(dirs.size());
  for (int d : dirs)
    s.push_back(DCH[d]);
  return s;
}

string pad_to_L(string s) {
  while ((int)s.size() < L) {
    const int i = uniform_int_distribution<int>(0, (int)s.size() - 1)(rng);
    s.insert(s.begin() + i, s[i]);
  }
  if ((int)s.size() > L)
    s.resize(L);
  return s;
}

string build_initial() {
  string best;
  i64 best_sc = -1;
  for (int t = 0; t < 400; ++t) {
    const int w_hi = 50 + (t % 20);
    auto dirs = dijkstra_path(1, w_hi);
    if (dirs.empty())
      continue;
    string s = pad_to_L(dirs_to_string(dirs));
    const i64 sc = score_of(s);
    if (sc > best_sc) {
      best_sc = sc;
      best = std::move(s);
    }
  }
  if (best.empty()) {
    auto dirs = dijkstra_path(1, 1);
    best = pad_to_L(dirs_to_string(dirs));
  }
  return best;
}

string sa_refine(string cur, double time_limit_sec) {
  i64 cur_sc = score_of(cur);
  string best = cur;
  i64 best_sc = cur_sc;
  const auto t0 = chrono::steady_clock::now();
  uniform_real_distribution<double> ur(0.0, 1.0);
  uniform_int_distribution<int> coin(0, 1);
  int iter = 0;
  while (true) {
    ++iter;
    const double elapsed =
        chrono::duration<double>(chrono::steady_clock::now() - t0).count();
    if (elapsed >= time_limit_sec)
      break;
    const double temp = max(1e-8, 1.0 - elapsed / time_limit_sec);
    string nxt = cur;
    const int n = (int)nxt.size();
    const int kind =
        (n < 100) ? 0 : uniform_int_distribution<int>(0, 4)(rng);
    switch (kind) {
      case 0: {  // copy char at i to position j
        if (n >= L)
          break;
        const int i = uniform_int_distribution<int>(0, n - 1)(rng);
        const int j = uniform_int_distribution<int>(0, n)(rng);
        nxt.insert(nxt.begin() + j, nxt[i]);
        break;
      }
      case 1: {  // insert random dir
        if (n >= L)
          break;
        const int j = uniform_int_distribution<int>(0, n)(rng);
        nxt.insert(nxt.begin() + j, DCH[uniform_int_distribution<int>(0, 3)(rng)]);
        break;
      }
      case 2: {  // delete
        if (n <= 1)
          break;
        const int i = uniform_int_distribution<int>(0, n - 1)(rng);
        nxt.erase(nxt.begin() + i);
        break;
      }
      case 3: {  // change
        const int i = uniform_int_distribution<int>(0, n - 1)(rng);
        nxt[i] = DCH[uniform_int_distribution<int>(0, 3)(rng)];
        break;
      }
      case 4: {  // move char
        if (n <= 1)
          break;
        const int i = uniform_int_distribution<int>(0, n - 1)(rng);
        const char c = nxt[i];
        nxt.erase(nxt.begin() + i);
        const int j = uniform_int_distribution<int>(0, (int)nxt.size())(rng);
        nxt.insert(nxt.begin() + j, c);
        break;
      }
    }
    if ((int)nxt.size() > L)
      continue;
    const i64 nsc = score_of(nxt);
    const double delta = (double)(nsc - cur_sc);
    if (delta >= 0 || ur(rng) < exp(delta / (temp * 5e6))) {
      cur = std::move(nxt);
      cur_sc = nsc;
      if (cur_sc > best_sc) {
        best_sc = cur_sc;
        best = cur;
      }
    }
  }
  (void)iter;
  return best;
}

void read_input() {
  if (!(cin >> si >> sj >> ti >> tj >> P))
    exit(0);
  string row;
  for (int i = 0; i < N; ++i) {
    cin >> row;
    for (int j = 0; j < N - 1; ++j)
      hs[i][j] = row[j] == '1';
  }
  for (int i = 0; i < N - 1; ++i) {
    cin >> row;
    for (int j = 0; j < N; ++j)
      vs[i][j] = row[j] == '1';
  }
}

}  // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  read_input();
  rng.seed(chrono::steady_clock::now().time_since_epoch().count());
  string ans = build_initial();
  ans = sa_refine(std::move(ans), 1.85);
  cout << ans << '\n';
  return 0;
}

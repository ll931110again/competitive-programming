// AHC007 A - Online MST: https://atcoder.jp/contests/ahc007/tasks/ahc007_a
//
// Edges arrive from 5 shuffled MST layers. Monte Carlo estimates completion
// cost with/without the current edge; progressive thresholds guarantee connectivity.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int N = 400;
constexpr int M = 1995;
constexpr double LO = 1.14094;
constexpr double HI = 3.01629;

struct XorShift {
  uint64_t x = 88172645463325252ull;

  uint64_t next() {
    x ^= x << 7;
    x ^= x >> 9;
    return x;
  }

  int randint(int lo, int hi) { return lo + int(next() % uint64_t(hi - lo + 1)); }
};

struct UF {
  int p[N], sz[N];

  void init() {
    for (int i = 0; i < N; ++i) {
      p[i] = i;
      sz[i] = 1;
    }
  }

  int find(int x) {
    while (p[x] != x) {
      p[x] = p[p[x]];
      x = p[x];
    }
    return x;
  }

  bool same(int a, int b) { return find(a) == find(b); }

  void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b)
      return;
    if (sz[a] < sz[b])
      swap(a, b);
    p[b] = a;
    sz[a] += sz[b];
  }

  int components() {
    int c = 0;
    for (int i = 0; i < N; ++i)
      if (find(i) == i)
        ++c;
    return c;
  }

  int size(int x) { return sz[find(x)]; }
};

int xs[N], ys[N];
int eu[M], ev[M], ed[M];
int adopted[M];
int elen[M];
UF cur_uf;
XorShift rng;

int dist_round(int u, int v) {
  const i64 dx = xs[u] - xs[v];
  const i64 dy = ys[u] - ys[v];
  return int(llround(sqrt(double(dx * dx + dy * dy))));
}

int sample_len(int i, XorShift& r) {
  const int lo = max(1, int(ed[i] * LO));
  const int hi = max(lo, int(ed[i] * HI + 0.5));
  return r.randint(lo, hi);
}

i64 simulate(int idx, bool take, XorShift& r) {
  UF uf;
  uf.init();
  i64 cost = 0;
  for (int i = 0; i < idx; ++i) {
    if (!adopted[i])
      continue;
    uf.unite(eu[i], ev[i]);
    cost += elen[i];
  }

  array<tuple<int, int, int>, M> pool{};
  int k = 0;
  if (take) {
    uf.unite(eu[idx], ev[idx]);
    cost += elen[idx];
  } else {
    pool[k++] = {elen[idx], eu[idx], ev[idx]};
  }

  for (int i = idx + 1; i < M; ++i)
    pool[k++] = {sample_len(i, r), eu[i], ev[i]};

  sort(pool.begin(), pool.begin() + k);
  for (int t = 0; t < k; ++t) {
    auto [w, u, v] = pool[t];
    if (uf.same(u, v))
      continue;
    uf.unite(u, v);
    cost += w;
  }

  if (uf.components() != 1)
    return (1ll << 60);
  return cost;
}

int decide(int idx, int len) {
  const int u = eu[idx], v = ev[idx];
  const int d = ed[idx];
  elen[idx] = len;

  if (cur_uf.same(u, v))
    return 0;

  const int need = cur_uf.components() - 1;
  const int rem = M - idx;
  const int su = cur_uf.size(u), sv = cur_uf.size(v);
  const int small = min(su, sv);

  // Endgame: must merge remaining components with the edges we have left.
  if (rem <= need)
    return 1;
  if (need <= 8)
    return 1;
  if (idx >= M - 120)
    return 1;
  if (small == 1)
    return 1;
  if (small <= 4 && len <= 3 * d)
    return 1;

  const double progress = double(idx) / M;
  const int prog_cap = int(d * (1.0 + 2.0 * progress) + 0.5);
  if (len <= prog_cap)
    return 1;

  if (rem <= need * 2 && len <= 3 * d)
    return 1;

  if (len <= int(d * 1.25))
    return 1;

  const int samples = 40 + min(80, rem / 20);
  i64 sum_take = 0, sum_skip = 0;
  for (int s = 0; s < samples; ++s) {
    sum_take += simulate(idx, true, rng);
    sum_skip += simulate(idx, false, rng);
  }

  const double avg_take = double(sum_take) / samples;
  const double avg_skip = double(sum_skip) / samples;
  if (avg_skip >= (1ll << 59))
    return 1;
  const double margin = 0.992 - 0.004 * progress;
  return avg_take <= avg_skip * margin ? 1 : 0;
}

}  // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  for (int i = 0; i < N; ++i)
    cin >> xs[i] >> ys[i];
  for (int i = 0; i < M; ++i) {
    cin >> eu[i] >> ev[i];
    ed[i] = dist_round(eu[i], ev[i]);
  }

  cur_uf.init();
  for (int i = 0; i < M; ++i) {
    int len;
    cin >> len;
    const int take = decide(i, len);
    adopted[i] = take;
    if (take)
      cur_uf.unite(eu[i], ev[i]);
    cout << take << '\n' << flush;
  }

  return 0;
}

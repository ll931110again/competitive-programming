/*
 * Solution for problem B: Binary Operation.
 *
 * Column j evolves as d <- op[d][digit_j(i)] for i = a+1 .. b (independent columns).
 * P = 10^{j+1}: digit_j(i) depends only on i mod P; each digit repeats in runs of length 10^j.
 * - Full periods: compose macro_d[j] and jump with repeat_lookup.
 * - Never O(P) single-step alignment: align i to a multiple of P using chunked runs (<=11 runs).
 * - When P > b: only chunked over [a+1, b]; each run length <= 10^j, O((b-a)/10^j + 10) iterations.
 */

#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;

namespace {

int op[10][10];
u64 pow10_u[22];

int digit_at(u64 x, int j) {
  return (int)((x / pow10_u[j]) % 10ULL);
}

// Apply x -> op[x][dig] exactly len times (state space size 10).
int repeat_fixed_input(int x, int dig, u64 len) {
  if (len == 0)
    return x;
  int seq[24];
  int sl = 0;
  seq[sl++] = x;
  for (u64 step = 1;; ++step) {
    int nx = op[seq[sl - 1]][dig];
    if (step == len)
      return nx;
    for (int i = 0; i < sl; ++i) {
      if (seq[i] == nx) {
        u64 mu = (u64)i;
        u64 lam = step - mu;
        u64 rem = (len - mu) % lam;
        return seq[(int)(mu + rem)];
      }
    }
    seq[sl++] = nx;
  }
}

int combine_period_fast(int j, int start_d) {
  int d = start_d;
  u64 run = pow10_u[j];
  for (int v = 0; v <= 9; ++v)
    d = repeat_fixed_input(d, v, run);
  return d;
}

int repeat_lookup(const int nxt[10], int d, u64 nb) {
  if (nb == 0)
    return d;
  int seq[24];
  int sl = 0;
  seq[sl++] = d;
  for (u64 step = 1;; ++step) {
    int nx = nxt[seq[sl - 1]];
    if (step == nb)
      return nx;
    for (int i = 0; i < sl; ++i) {
      if (seq[i] == nx) {
        u64 mu = (u64)i;
        u64 lam = step - mu;
        u64 rem = (nb - mu) % lam;
        return seq[(int)(mu + rem)];
      }
    }
    seq[sl++] = nx;
  }
}

// Apply transitions for i in [L, R] inclusive; digit column j; P = 10^{j+1}.
int apply_chunked_range(int j, int d, u64 L, u64 R) {
  u64 P = pow10_u[j + 1];
  u64 powj = pow10_u[j];
  u64 i = L;
  while (i <= R) {
    u64 rem = i % P;
    u64 q = i / P;
    u64 t = rem / powj;
    int dig = (int)t;
    u64 i_end = q * P + (t + 1) * powj - 1;
    if (i_end > R)
      i_end = R;
    d = repeat_fixed_input(d, dig, i_end - i + 1);
    i = i_end + 1;
  }
  return d;
}

int evolve_digit(int j, int start_d, u64 a, u64 b) {
  int d = start_d;
  u64 i = a + 1;
  u64 P = pow10_u[j + 1];
  int macro_d[22][10];
  bool macro_ready[22];
  if (!macro_ready[j]) {
    for (int sd = 0; sd < 10; ++sd)
      macro_d[j][sd] = combine_period_fast(j, sd);
    macro_ready[j] = true;
  }
  while (i <= b) {
    if (i % P != 0) {
      u64 align = (i + P - 1) / P * P;
      u64 up_to = min(b, align - 1);
      if (i <= up_to)
        d = apply_chunked_range(j, d, i, up_to);
      i = up_to + 1;
      if (i > b)
        break;
    }
    u64 rem = b - i + 1;
    if (i % P == 0 && rem >= P) {
      u64 nb = rem / P;
      d = repeat_lookup(macro_d[j], d, nb);
      i += nb * P;
      continue;
    }
    d = apply_chunked_range(j, d, i, b);
    break;
  }
  return d;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("binary.in", "r", stdin);
  freopen("binary.out", "w", stdout);

  pow10_u[0] = 1;
  for (int k = 1; k <= 21; ++k)
    pow10_u[k] = pow10_u[k - 1] * 10ULL;

  for (int x = 0; x < 10; ++x)
    for (int y = 0; y < 10; ++y)
      cin >> op[x][y];

  u64 a, b;
  cin >> a >> b;

  int max_j = 0;
  for (int j = 0; j <= 20; ++j)
    if (b >= pow10_u[j])
      max_j = j;

  vector<int> outdig(max_j + 1);
  for (int j = 0; j <= max_j; ++j)
    outdig[j] = evolve_digit(j, digit_at(a, j), a, b);

  int hi = max_j;
  while (hi > 0 && outdig[hi] == 0)
    --hi;

  u64 ans = (unsigned)outdig[hi];
  for (int j = hi - 1; j >= 0; --j)
    ans = ans * 10ULL + (unsigned)outdig[j];

  cout << ans << '\n';
  return 0;
}

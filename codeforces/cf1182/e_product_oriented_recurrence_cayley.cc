// CF 1182E - Product Oriented Recurrence (Cayley–Hamilton route)
// https://codeforces.com/contest/1182/problem/E
//
// (x_i), (y_i), (z_i) are order-3 linear recurrences (companion matrix C). Cayley–Hamilton:
// C^n is a degree-2 polynomial in C, so x_n is the k-th term via that polynomial mod PHI.
// (w_i) is order-5 (inhomogeneous); still a linear recurrence on the augmented state — matrix
// exponentiation on the 5×5 companion matrix (same theorem, larger degree).

#include <bits/stdc++.h>
#include "../../common/berlekamp_massey.hpp"

using namespace std;

static constexpr int MOD = 1'000'000'007;
static constexpr int PHI = MOD - 1;

using Mint = ModInt<PHI>;

static long long mod_pow_answer(long long a, long long e) {
  long long r = 1;
  a %= MOD;
  while (e > 0) {
    if (e & 1) r = (__int128)r * a % MOD;
    a = (__int128)a * a % MOD;
    e >>= 1;
  }
  return r;
}

static long long kth_fib_like(long long n, int a1, int a2, int a3) {
  if (n <= 0) return 0;
  if (n == 1) return a1;
  if (n == 2) return a2;
  if (n == 3) return a3;
  vector<Mint> init = {Mint(a1), Mint(a2), Mint(a3)};
  vector<Mint> trans = {Mint(1), Mint(1), Mint(1)};
  return kth_linear_recurrence(init, trans, n - 1).x;
}

struct Mat {
  int n;
  vector<vector<long long>> a;
  Mat(int n, bool ident = false) : n(n), a(n, vector<long long>(n, 0)) {
    if (ident)
      for (int i = 0; i < n; ++i) a[i][i] = 1;
  }
  static Mat mul(const Mat &x, const Mat &y) {
    Mat z(x.n);
    for (int i = 0; i < x.n; ++i)
      for (int k = 0; k < x.n; ++k) if (x.a[i][k])
        for (int j = 0; j < x.n; ++j)
          z.a[i][j] = (z.a[i][j] + x.a[i][k] * y.a[k][j]) % PHI;
    return z;
  }
  static Mat pow(Mat base, long long e) {
    Mat res(base.n, true);
    while (e > 0) {
      if (e & 1) res = mul(res, base);
      base = mul(base, base);
      e >>= 1;
    }
    return res;
  }
  vector<long long> apply(const vector<long long> &v) const {
    vector<long long> r(n);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        r[i] = (r[i] + a[i][j] * v[j]) % PHI;
    return r;
  }
};

static long long exp_w(long long n) {
  if (n <= 3) return 0;
  Mat baseB(5);
  baseB.a = {
      {1, 1, 1, 1, 1},
      {1, 0, 0, 0, 0},
      {0, 1, 0, 0, 0},
      {0, 0, 0, 1, 1},
      {0, 0, 0, 0, 1},
  };
  vector<long long> vw = {0, 0, 0, 0, 2};
  vw = Mat::pow(baseB, n - 3).apply(vw);
  return vw[0];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  long long n, f1, f2, f3, c;
  cin >> n >> f1 >> f2 >> f3 >> c;

  long long wn = exp_w(n);
  long long xn = kth_fib_like(n, 1, 0, 0);
  long long yn = kth_fib_like(n, 0, 1, 0);
  long long zn = kth_fib_like(n, 0, 0, 1);

  long long ans = mod_pow_answer(c, wn);
  ans = ans * mod_pow_answer(f1, xn) % MOD;
  ans = ans * mod_pow_answer(f2, yn) % MOD;
  ans = ans * mod_pow_answer(f3, zn) % MOD;
  cout << ans << '\n';
  return 0;
}

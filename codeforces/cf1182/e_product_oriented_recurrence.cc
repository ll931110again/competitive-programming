// CF 1182E - Product Oriented Recurrence: https://codeforces.com/contest/1182/problem/E
//
// f_x = c^{2x-6} * f_{x-1} * f_{x-2} * f_{x-3}  (x >= 4).
// Take logs: each f_i = c^{w_i} * f_1^{x_i} * f_2^{y_i} * f_3^{z_i} with linear recurrences on
// (w_i), (x_i), (y_i), (z_i). Companion matrices satisfy their characteristic polynomials
// (Cayley–Hamilton): powers reduce to degree-(d-1) polynomials in the matrix, equivalently
// the scalar sequences satisfy the same order-d recurrence — computed here via matrix exp on
// exponents mod phi(MOD) (Euler).

#include <bits/stdc++.h>
using namespace std;

static constexpr long long MOD = 1'000'000'007;
static constexpr long long PHI = MOD - 1;

static long long mod_pow(long long a, long long e, long long mod = MOD) {
  long long r = 1 % mod;
  a %= mod;
  while (e > 0) {
    if (e & 1)
      r = (__int128)r * a % mod;
    a = (__int128)a * a % mod;
    e >>= 1;
  }
  return r;
}

struct Mat {
  int n;
  vector<vector<long long>> a;

  Mat(int n, bool ident = false) : n(n), a(n, vector<long long>(n, 0)) {
    if (ident)
      for (int i = 0; i < n; ++i)
        a[i][i] = 1;
  }

  static Mat mul(const Mat& x, const Mat& y) {
    Mat z(x.n);
    for (int i = 0; i < x.n; ++i)
      for (int k = 0; k < x.n; ++k)
        if (x.a[i][k])
          for (int j = 0; j < x.n; ++j)
            z.a[i][j] = (z.a[i][j] + x.a[i][k] * y.a[k][j]) % PHI;
    return z;
  }

  static Mat pow(Mat base, long long e) {
    Mat res(base.n, true);
    while (e > 0) {
      if (e & 1)
        res = mul(res, base);
      base = mul(base, base);
      e >>= 1;
    }
    return res;
  }

  vector<long long> apply(const vector<long long>& v) const {
    vector<long long> r(n);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        r[i] = (r[i] + a[i][j] * v[j]) % PHI;
    return r;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  long long n, f1, f2, f3, c;
  cin >> n >> f1 >> f2 >> f3 >> c;

  // x, y, z: tribonacci on exponents. Companion state at i = 3 is (s_i, s_{i-1}, s_{i-2}).
  Mat baseA(3);
  baseA.a = {{1, 1, 1}, {1, 0, 0}, {0, 1, 0}};
  vector<long long> vx = {0, 0, 1}, vy = {0, 1, 0}, vz = {1, 0, 0}; // (x_3,x_2,x_1), (y_3,...)

  // w_i = w_{i-1} + w_{i-2} + w_{i-3} + 2i - 6; augmented with (2i-6).
  Mat baseB(5);
  baseB.a = {
      {1, 1, 1, 1, 1}, {1, 0, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 0, 0, 1, 1}, {0, 0, 0, 0, 1},
  };
  vector<long long> vw = {0, 0, 0, 0, 2};

  if (n >= 4) {
    Mat pa = Mat::pow(baseA, n - 3);
    Mat pb = Mat::pow(baseB, n - 3);
    vx = pa.apply(vx);
    vy = pa.apply(vy);
    vz = pa.apply(vz);
    vw = pb.apply(vw);
  }

  long long wn = vw[0], xn = vx[0], yn = vy[0], zn = vz[0];
  long long ans = mod_pow(c, wn);
  ans = ans * mod_pow(f1, xn) % MOD;
  ans = ans * mod_pow(f2, yn) % MOD;
  ans = ans * mod_pow(f3, zn) % MOD;
  cout << ans << '\n';
  return 0;
}

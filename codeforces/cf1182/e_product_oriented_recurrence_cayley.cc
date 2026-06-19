// CF 1182E - Product Oriented Recurrence (Cayley–Hamilton route)
// https://codeforces.com/contest/1182/problem/E
//
// f_x = c^{2x-6} * f_{x-1} * f_{x-2} * f_{x-3}. Log-exponents (w,x,y,z) satisfy linear
// recurrences from companion matrices. Cayley–Hamilton: A^n is a degree-(d-1) polynomial in A,
// so each scalar sequence is order-d linear — computed here via matrix exponentiation mod phi(MOD).

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr i64 MOD = 1'000'000'007;
constexpr i64 PHI = MOD - 1;

i64 mod_pow(i64 a, i64 e) {
  i64 r = 1 % MOD;
  a %= MOD;
  while (e > 0) {
    if (e & 1)
      r = (__int128)r * a % MOD;
    a = (__int128)a * a % MOD;
    e >>= 1;
  }
  return r;
}

struct Mat {
  int n;
  vector<vector<i64>> a;

  Mat(int n, bool ident = false) : n(n), a(n, vector<i64>(n, 0)) {
    if (ident)
      for (int i = 0; i < n; ++i)
        a[i][i] = 1;
  }

  Mat mul(const Mat& x, const Mat& y) {
    Mat z(x.n);
    for (int i = 0; i < x.n; ++i)
      for (int k = 0; k < x.n; ++k)
        if (x.a[i][k])
          for (int j = 0; j < x.n; ++j)
            z.a[i][j] = (z.a[i][j] + x.a[i][k] * y.a[k][j]) % PHI;
    return z;
  }

  Mat pow(Mat base, i64 e) {
    Mat res(base.n, true);
    while (e > 0) {
      if (e & 1)
        res = mul(res, base);
      base = mul(base, base);
      e >>= 1;
    }
    return res;
  }

  vector<i64> apply(const vector<i64>& v) const {
    vector<i64> r(n);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        r[i] = (r[i] + a[i][j] * v[j]) % PHI;
    return r;
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 n, f1, f2, f3, c;
  cin >> n >> f1 >> f2 >> f3 >> c;

  Mat base_a(3);
  base_a.a = {{1, 1, 1}, {1, 0, 0}, {0, 1, 0}};
  vector<i64> vx = {0, 0, 1}, vy = {0, 1, 0}, vz = {1, 0, 0};

  Mat base_b(5);
  base_b.a = {
      {1, 1, 1, 1, 1}, {1, 0, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 0, 0, 1, 1}, {0, 0, 0, 0, 1},
  };
  vector<i64> vw = {0, 0, 0, 0, 2};

  if (n >= 4) {
    Mat pa = Mat::pow(base_a, n - 3);
    Mat pb = Mat::pow(base_b, n - 3);
    vx = pa.apply(vx);
    vy = pa.apply(vy);
    vz = pa.apply(vz);
    vw = pb.apply(vw);
  }

  i64 ans = mod_pow(c, vw[0]);
  ans = ans * mod_pow(f1, vx[0]) % MOD;
  ans = ans * mod_pow(f2, vy[0]) % MOD;
  ans = ans * mod_pow(f3, vz[0]) % MOD;
  cout << ans << '\n';
  return 0;
}

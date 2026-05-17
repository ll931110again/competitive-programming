// Berlekamp–Massey + k-th term of a linear recurrence (mod prime).
// Typical use after Cayley–Hamilton: matrix powers satisfy a recurrence of order <= n.

#pragma once

#include <cassert>
#include <vector>

template <int MOD> struct ModInt {
  int x;
  ModInt(long long v = 0) : x(int((v % MOD + MOD) % MOD)) {}
  ModInt &operator+=(ModInt o) {
    x += o.x;
    if (x >= MOD) x -= MOD;
    return *this;
  }
  ModInt &operator-=(ModInt o) {
    x -= o.x;
    if (x < 0) x += MOD;
    return *this;
  }
  ModInt &operator*=(ModInt o) {
    x = int((long long)x * o.x % MOD);
    return *this;
  }
  friend ModInt operator+(ModInt a, ModInt b) { return a += b; }
  friend ModInt operator-(ModInt a, ModInt b) { return a -= b; }
  friend ModInt operator*(ModInt a, ModInt b) { return a *= b; }
  friend ModInt operator/(ModInt a, ModInt b) { return a * mod_pow(b, MOD - 2); }
  bool operator==(const ModInt &o) const { return x == o.x; }
  bool operator!=(const ModInt &o) const { return x != o.x; }
};

template <int MOD> ModInt<MOD> mod_pow(ModInt<MOD> a, long long e) {
  ModInt<MOD> r(1);
  while (e > 0) {
    if (e & 1) r *= a;
    a *= a;
    e >>= 1;
  }
  return r;
}

// Returns coeffs c[0..L-1] for recurrence s_i = sum_{j=0}^{L-1} c[j] * s_{i-j-1}.
template <int MOD>
std::vector<ModInt<MOD>> berlekamp_massey(const std::vector<ModInt<MOD>> &s) {
  const int n = (int)s.size();
  std::vector<ModInt<MOD>> C(n), B(n);
  C[0] = B[0] = ModInt<MOD>(1);
  ModInt<MOD> b(1);
  int L = 0;
  for (int i = 0, m = 1; i < n; ++i) {
    ModInt<MOD> d = s[i];
    for (int j = 1; j <= L; ++j) d += C[j] * s[i - j];
    if (d == ModInt<MOD>(0)) {
      ++m;
      continue;
    }
    std::vector<ModInt<MOD>> T = C;
    ModInt<MOD> coef = d / b;
    for (int j = m; j < n; ++j) C[j] = C[j] - coef * B[j - m];
    if (2 * L > i) {
      ++m;
      continue;
    }
    L = i + 1 - L;
    B = std::move(T);
    b = d;
    m = 1;
  }
  // Relation: s[i] + sum_{j=1}^{L} C[j] * s[i-j] = 0  =>  s[i] = sum_{j=0}^{L-1} (-C[j+1]) * s[i-j-1].
  std::vector<ModInt<MOD>> res(L);
  for (int j = 0; j < L; ++j) res[j] = ModInt<MOD>(0) - C[j + 1];
  return res;
}

template <int MOD>
ModInt<MOD> kth_linear_recurrence(std::vector<ModInt<MOD>> init,
                                  const std::vector<ModInt<MOD>> &trans, long long k) {
  const int n = (int)trans.size();
  assert((int)init.size() >= n);
  if (k < (int)init.size()) return init[(int)k];

  using Poly = std::vector<ModInt<MOD>>;
  Poly r(n + 1), b(n + 2);
  r[0] = ModInt<MOD>(1);
  b[1] = ModInt<MOD>(1);

  auto combine = [&](const Poly &a, const Poly &bb) {
    Poly res(2 * n + 1);
    for (int i = 0; i <= n; ++i)
      for (int j = 0; j <= n; ++j)
        if (a[i] != ModInt<MOD>(0) && bb[j] != ModInt<MOD>(0)) res[i + j] += a[i] * bb[j];
    for (int i = 2 * n; i > n; --i) {
      if (res[i] == ModInt<MOD>(0)) continue;
      for (int j = 0; j < n; ++j) res[i - 1 - j] += res[i] * trans[j];
    }
    res.resize(n + 1);
    return res;
  };

  ++k;
  while (k > 0) {
    if (k & 1) r = combine(r, b);
    b = combine(b, b);
    k >>= 1;
  }

  ModInt<MOD> ans(0);
  for (int i = 0; i < n; ++i) ans += r[i + 1] * init[n - 1 - i];
  return ans;
}

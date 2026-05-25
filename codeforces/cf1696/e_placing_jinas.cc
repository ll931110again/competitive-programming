// CF 1696E — Placing Jinas
// https://codeforces.com/contest/1696/problem/E
//
// Minimum operations to clear white cells on a grid defined by non-increasing heights a_i.
// Answer: sum_{i=0}^{n} C(i + a_i, i + 1) mod 1e9+7.
// With a_i, n <= 2e5 we have i + a_i < MOD, so factorials suffice (no Lucas needed).

#include "../../bits/stdc++.h"
using namespace std;

static constexpr unsigned MOD = 1'000'000'007;

template <unsigned M> struct ModInt {
  unsigned x;

  ModInt() : x(0) {}
  ModInt(unsigned v) : x(v % M) {}
  ModInt(unsigned long long v) : x(unsigned(v % M)) {}
  ModInt(int v) : x(((v %= int(M)) < 0) ? (v + int(M)) : v) {}

  ModInt& operator+=(const ModInt& a) {
    x = ((x += a.x) >= M) ? (x - M) : x;
    return *this;
  }
  ModInt operator+(const ModInt& a) const {
    return ModInt(*this) += a;
  }

  friend ostream& operator<<(ostream& os, const ModInt& a) {
    return os << a.x;
  }
};

using Mint = ModInt<MOD>;

static constexpr int MAXN = 400005;

static int pow_mod(int a, long long e) {
  long long r = 1, b = a % MOD;
  while (e > 0) {
    if (e & 1)
      r = r * b % MOD;
    b = b * b % MOD;
    e >>= 1;
  }
  return int(r);
}

static unsigned fac[MAXN], ifac[MAXN];

static unsigned C(int n, int k) {
  if (k < 0 || k > n)
    return 0;
  return unsigned(1ULL * fac[n] * ifac[k] % MOD * ifac[n - k] % MOD);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  fac[0] = 1;
  for (int i = 1; i < MAXN; i++)
    fac[i] = unsigned(1ULL * fac[i - 1] * i % MOD);
  ifac[MAXN - 1] = pow_mod(int(fac[MAXN - 1]), MOD - 2);
  for (int i = MAXN - 1; i >= 1; i--)
    ifac[i - 1] = unsigned(1ULL * ifac[i] * i % MOD);

  int n;
  cin >> n;
  vector<int> a(n + 1);
  for (int i = 0; i <= n; i++)
    cin >> a[i];

  Mint ans = 0;
  for (int i = 0; i <= n; i++)
    ans += Mint(C(i + a[i], i + 1));
  cout << ans << '\n';
  return 0;
}

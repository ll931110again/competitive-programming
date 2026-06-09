// CSES 2209 - Counting Necklaces: https://cses.fi/problemset/task/2209
// (CSES Mathematics; also listed in CF community CSES gym clones when available.)
//
// Pólya / Burnside on Z_n (cyclic rotations only):
//   answer = (1/n) * sum_{i=0}^{n-1} m^{gcd(i,n)}
//          = (1/n) * sum_{d|n} phi(n/d) * m^d.

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr unsigned MOD = 1000000007;

template <unsigned M> struct ModInt {
  unsigned x;
  ModInt() : x(0) {}
  ModInt(unsigned long long v) : x(unsigned(v % M)) {}
  ModInt(int v) : x(((v %= int(M)) < 0) ? (v + int(M)) : v) {}

  ModInt& operator+=(ModInt a) {
    x = ((x += a.x) >= M) ? (x - M) : x;
    return *this;
  }
  ModInt& operator-=(ModInt a) {
    x = ((x -= a.x) >= M) ? (x + M) : x;
    return *this;
  }
  ModInt& operator*=(ModInt a) {
    x = unsigned((unsigned long long)x * a.x % M);
    return *this;
  }
  ModInt& operator/=(ModInt a) {
    return *this *= a.inv();
  }

  ModInt pow(long long e) const {
    if (e < 0)
      return inv().pow(-e);
    ModInt a = *this, b = 1;
    for (; e; e >>= 1) {
      if (e & 1)
        b *= a;
      a *= a;
    }
    return b;
  }

  ModInt inv() const {
    unsigned a = M, b = x;
    int y = 0, z = 1;
    for (; b;) {
      unsigned q = a / b;
      unsigned c = a - q * b;
      a = b;
      b = c;
      int w = y - int(q) * z;
      y = z;
      z = w;
    }
    return ModInt(y);
  }

  ModInt operator+(ModInt a) const {
    return ModInt(*this) += a;
  }
  ModInt operator-(ModInt a) const {
    return ModInt(*this) -= a;
  }
  ModInt operator*(ModInt a) const {
    return ModInt(*this) *= a;
  }
  ModInt operator/(ModInt a) const {
    return ModInt(*this) /= a;
  }
};

using Mint = ModInt<MOD>;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  long long n, m;
  cin >> n >> m;

  auto euler_phi = [](long long x) {
    long long r = x;
    for (long long p = 2; p * p <= x; p++) {
      if (x % p)
        continue;
      while (x % p == 0)
        x /= p;
      r -= r / p;
    }
    if (x > 1)
      r -= r / x;
    return r;
  };

  Mint sum = 0;
  for (long long d = 1; d * d <= n; d++) {
    if (n % d)
      continue;
    const long long d1 = d;
    const long long d2 = n / d;
    sum += Mint((unsigned long long)euler_phi(d1)) * Mint((unsigned long long)m).pow(d2);
    if (d1 != d2)
      sum += Mint((unsigned long long)euler_phi(d2)) * Mint((unsigned long long)m).pow(d1);
  }

  const Mint ans = sum / Mint((unsigned long long)n);
  cout << ans.x << '\n';
  return 0;
}

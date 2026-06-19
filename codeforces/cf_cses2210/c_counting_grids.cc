// CSES 2210 - Counting Grids: https://cses.fi/problemset/task/2210
//
// Pólya / Burnside on C_4 (90° rotations of an n×n board). Closed form for huge n:
//   even n: (1/4)(2^{n^2} + 2·2^{n^2/4} + 2^{n^2/2})
//   odd n:  (1/4)(2^{n^2} + 2·2^{(n^2+3)/4} + 2^{(n^2+n)/2 - n/2})

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
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

  ModInt pow(i64 e) const {
    e %= (M - 1); // Fermat, MOD prime
    if (e < 0)
      e += (M - 1);
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

  i64 n;
  cin >> n;

  const i64 n2 = n * n;
  Mint sum;
  if (n % 2 == 0) {
    // Cycle exponents for C_4 on n×n cells: n^2, n^2/4, n^2/2, n^2/4.
    sum = Mint(2).pow(n2) + Mint(2) * Mint(2).pow(n2 / 4) + Mint(2).pow(n2 / 2);
  } else {
    // n^2, (n^2+3)/4, (n^2+n)/2 - n/2, (n^2+3)/4.
    const i64 e90 = (n2 + 3) / 4;
    const i64 e180 = (n2 + n) / 2 - n / 2;
    sum = Mint(2).pow(n2) + Mint(2) * Mint(2).pow(e90) + Mint(2).pow(e180);
  }

  const Mint ans = sum / Mint(4);
  cout << ans.x << '\n';
  return 0;
}

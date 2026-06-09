// GCPC 2017 / CF Gym 101873B - Buildings: https://codeforces.com/gym/101873/problem/B
//
// Each house has m walls; wall i is an n×n grid colored with c colors (c^((n^2))
// choices per wall). Two designs are the same if the m-gon can be rotated to match.
// Rotations form Z_m acting on the tuple of walls.
//
// Burnside / Pólya (necklace on m beads, "color" of bead = one wall pattern):
//   answer = (1/m) * sum_{s=0}^{m-1} (c^(n^2))^(gcd(s,m))
//          = (1/m) * sum_{d|m} phi(m/d) * (c^(n^2))^d   (mod 10^9+7).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr unsigned MOD = 1000000007;

template <unsigned M> struct ModInt {
  unsigned x;

  ModInt() : x(0) {}
  ModInt(unsigned v) : x(v % M) {}
  ModInt(unsigned long long v) : x(unsigned(v % M)) {}
  ModInt(int v) : x(((v %= static_cast<int>(M)) < 0) ? (v + static_cast<int>(M)) : v) {}
  ModInt(long long v)
      : x(((v %= static_cast<long long>(M)) < 0) ? (v + static_cast<long long>(M)) : v) {}

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

  int n, m, c;
  cin >> n >> m >> c;

  const int per_wall_exp = n * n;
  const Mint bead = Mint(c).pow(per_wall_exp);

  Mint sum = 0;
  for (int s = 0; s < m; s++) {
    const int g = gcd(s, m);
    sum += bead.pow(g);
  }

  const Mint ans = sum / Mint(m);
  cout << ans.x << '\n';
  return 0;
}

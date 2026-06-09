// CF 1065E - Side Transmutations: https://codeforces.com/contest/1065/problem/E
//
// Count strings of length n over alphabet A, modulo moves: pick b_i, swap the first/last
// b_i chars with the opposite half reversed. Moves commute and are self-inverse, so any
// sequence is a subset of {b_1,...,b_m} (group = (Z/2)^m under XOR).
//
// Burnside: answer = (1/|G|) * sum_{g in G} |Fix(g)|, |G| = 2^m.
// For subset with effective swapped-pair count l (alternating sum of chosen b_i),
// a string is fixed iff matched positions agree => |Fix(g)| = |A|^(n-l).
//
// Summing over subsets via the recurrence from the editorial / Burnside tutorial:
//   p_i = p_{i-1} + |A|^{-b_i} * r_{i-1},  r_i = r_{i-1} + |A|^{b_i} * p_{i-1},  p_0=r_0=1
//   answer = |A|^n * p_m * inv(2^m)  (mod 998244353).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr unsigned MOD = 998244353;

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

  long long n;
  int m;
  long long alphabet;
  cin >> n >> m >> alphabet;

  vector<int> b(m);
  for (int i = 0; i < m; i++)
    cin >> b[i];

  const Mint A(alphabet);
  Mint p = 1, r = 1;
  for (int i = 0; i < m; i++) {
    const Mint np = p + A.pow(-b[i]) * r;
    const Mint nr = r + A.pow(b[i]) * p;
    p = np;
    r = nr;
  }

  const Mint ans = A.pow(n) * p * Mint(2).pow(m).inv();
  cout << ans.x << '\n';
  return 0;
}

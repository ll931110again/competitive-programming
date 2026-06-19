// ABC284 Ex - Count Unlabeled Graphs: https://atcoder.jp/contests/abc284/tasks/abc284_h
//
// Pólya enumeration on S_N (vertex relabeling). For a permutation with cycle lengths
// (c_1,...,c_m), fixed labeled graphs = C^m * 2^{sum floor(c_i/2) + sum_{i<j} gcd(c_i,c_j)}.
// Sum over all permutations, divide by N!, then inclusion-exclusion for exactly K colors.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

unsigned PMOD;

struct Mint {
  unsigned x;
  Mint() : x(0) {}
  Mint(unsigned long long v) : x(unsigned(v % PMOD)) {}
  Mint(int v) : x(((v %= int(PMOD)) < 0) ? (v + int(PMOD)) : v) {}

  Mint& operator+=(Mint a) {
    x = ((x += a.x) >= PMOD) ? (x - PMOD) : x;
    return *this;
  }
  Mint& operator-=(Mint a) {
    x = ((x -= a.x) >= PMOD) ? (x + PMOD) : x;
    return *this;
  }
  Mint& operator*=(Mint a) {
    x = unsigned((unsigned long long)x * a.x % PMOD);
    return *this;
  }
  Mint& operator/=(Mint a) {
    return *this *= a.inv();
  }

  Mint pow(i64 e) const {
    if (e < 0)
      return inv().pow(-e);
    Mint a = *this, b = 1;
    for (; e; e >>= 1) {
      if (e & 1)
        b *= a;
      a *= a;
    }
    return b;
  }

  Mint inv() const {
    unsigned a = PMOD, b = x;
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
    return Mint(y);
  }

  Mint operator+(Mint a) const {
    return Mint(*this) += a;
  }
  Mint operator-(Mint a) const {
    return Mint(*this) -= a;
  }
  Mint operator*(Mint a) const {
    return Mint(*this) *= a;
  }
  Mint operator/(Mint a) const {
    return Mint(*this) /= a;
  }
};

int N, K;
vector<int> part;
vector<Mint> fact;

Mint count_at_most(int C) {
  Mint total = 0;
  const auto go = [&](auto&& self, int rem, int max_part) -> void {
    if (rem == 0) {
      const int m = (int)part.size();
      i64 exp = 0;
      for (int i = 0; i < m; i++) {
        exp += part[i] / 2;
        for (int j = 0; j < i; j++)
          exp += gcd(part[i], part[j]);
      }
      Mint fix = Mint(2).pow(exp) * Mint(C).pow(m);

      Mint denom = 1;
      for (int i = 0; i < m;) {
        int j = i;
        while (j < m && part[j] == part[i])
          ++j;
        const int cnt = j - i;
        for (int t = 0; t < cnt; t++)
          denom *= Mint(part[i]);
        denom *= fact[cnt];
        i = j;
      }
      const Mint perms = fact[N] / denom;
      total += perms * fix;
      return;
    }
    for (int k = min(max_part, rem); k >= 1; k--) {
      part.push_back(k);
      self(self, rem - k, k);
      part.pop_back();
    }
  };
  go(go, N, N);
  return total / fact[N];
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  unsigned P;
  cin >> N >> K >> P;
  PMOD = P;

  fact.assign(N + 1, Mint(0));
  fact[0] = 1;
  for (int i = 1; i <= N; i++)
    fact[i] = fact[i - 1] * Mint(i);

  Mint ans = 0;
  for (int j = 0; j <= K; j++) {
    Mint term = count_at_most(K - j) * fact[K] / (fact[j] * fact[K - j]);
    if (j & 1)
      ans -= term;
    else
      ans += term;
  }

  cout << ans.x << '\n';
  return 0;
}

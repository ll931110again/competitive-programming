// CF 582D - Number of Binominal Coefficients: https://codeforces.com/contest/582/problem/D
//
// Count pairs (n, k) with 0 <= k <= n <= A such that p^alpha | C(n, k).
// Kummer's theorem: v_p(C(n,k)) equals the number of carries when adding k and n-k in base p
// (equivalently when adding k and (n-k), i.e. n in base p). Digit DP on A (decimal, up to 10^1000).

#include <bits/stdc++.h>
using namespace std;

static constexpr unsigned MOD = 1000000007;

template <unsigned M> struct ModInt {
  unsigned x;
  ModInt() : x(0) {}
  ModInt(unsigned long long v) : x(unsigned(v % M)) {}
  ModInt(long long v) : x(unsigned(((v %= (long long)M) < 0) ? (v + (long long)M) : v)) {}
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

  ModInt operator+(ModInt a) const {
    return ModInt(*this) += a;
  }
  ModInt operator*(ModInt a) const {
    return ModInt(*this) *= a;
  }
};

using Mint = ModInt<MOD>;

static constexpr int MAXD = 4005;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int p, alpha;
  string s;
  cin >> p >> alpha >> s;
  const int n = (int)s.size();
  vector<int> a(MAXD + 1);
  for (int i = 1; i <= n; i++)
    a[i] = s[n - i] - '0';

  vector<long long> b(MAXD + 1);
  for (int i = n; i >= 1; i--) {
    for (int j = 1; j < MAXD; j++)
      b[j] *= 10;
    b[1] += a[i];
    for (int j = 1; j < MAXD - 1; j++) {
      if (b[j] >= p) {
        b[j + 1] += b[j] / p;
        b[j] %= p;
      }
    }
  }
  for (int i = 1; i <= MAXD; i++)
    a[i] = (int)b[i];
  int len = MAXD - 1;
  while (len > 0 && !a[len])
    --len;
  if (!len) {
    cout << 0 << '\n';
    return 0;
  }

  static Mint f[2][2][MAXD], g[2][2][MAXD];
  f[1][0][0] = 1;
  for (int i = len; i >= 1; i--) {
    const Mint a0 = Mint(1LL * p * (p + 1) / 2);
    const Mint a1 = Mint(1LL * p * (p - 1) / 2);
    const Mint a2 = Mint(1LL * a[i] * (a[i] + 1) / 2);
    const Mint a3 = Mint(1LL * a[i] * (a[i] - 1) / 2);
    const Mint a4 = Mint(1LL * a[i] * (p + p - a[i] - 1) / 2);
    const Mint a5 = Mint(1LL * a[i] * (p + p - a[i] + 1) / 2);

    for (int j = 0; j <= len + 1; j++)
      g[0][0][j] = g[0][1][j] = g[1][0][j] = g[1][1][j] = 0;

    for (int j = 0; j <= len - i; j++) {
      const Mint f00 = f[0][0][j], f01 = f[0][1][j];
      const Mint f10 = f[1][0][j], f11 = f[1][1][j];
      g[0][0][j] += f00 * a0 + f10 * a2;
      g[0][1][j] += f00 * a1 + f10 * a3;
      g[1][0][j] += f10 * Mint(a[i] + 1);
      g[1][1][j] += f10 * Mint(a[i]);
      g[0][0][j + 1] += f01 * a1 + f11 * a4;
      g[0][1][j + 1] += f01 * a0 + f11 * a5;
      g[1][0][j + 1] += f11 * Mint(p - a[i] - 1);
      g[1][1][j + 1] += f11 * Mint(p - a[i]);
    }
    memcpy(f, g, sizeof(f));
  }

  Mint ans;
  for (int i = alpha; i <= len; i++)
    ans += f[0][0][i] + f[1][0][i];
  cout << ans.x << '\n';
  return 0;
}

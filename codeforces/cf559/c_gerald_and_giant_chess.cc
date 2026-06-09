// CF 559C — Gerald and Giant Chess
// https://codeforces.com/contest/559/problem/C
//
// Paths from (1,1) to (h,w) moving only right/down, avoiding black cells.
// Inclusion–exclusion on sorted obstacles: dp[i] = ways to reach i minus paths through
// earlier black cells. Binomials C(x+y-2, x-1) have arguments ≤ h+w ≤ 2e5 < 1e9+7,
// so factorials suffice here (Lucas is not required, unlike the P3807 template).

#include <bits/stdc++.h>
using namespace std;

namespace {

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
  ModInt& operator-=(const ModInt& a) {
    x = ((x += M - a.x) >= M) ? (x - M) : x;
    return *this;
  }
  ModInt& operator*=(const ModInt& a) {
    x = unsigned((unsigned long long)x * a.x % M);
    return *this;
  }
  ModInt pow(unsigned long long e) const {
    ModInt r = 1, b = *this;
    while (e) {
      if (e & 1)
        r *= b;
      b *= b;
      e >>= 1;
    }
    return r;
  }
  ModInt inv() const {
    return pow(M - 2);
  }

  ModInt operator+(const ModInt& a) const {
    return ModInt(*this) += a;
  }
  ModInt operator-(const ModInt& a) const {
    return ModInt(*this) -= a;
  }
  ModInt operator*(const ModInt& a) const {
    return ModInt(*this) *= a;
  }
  bool operator==(const ModInt& a) const {
    return x == a.x;
  }
  bool operator!=(const ModInt& a) const {
    return x != a.x;
  }
  friend ostream& operator<<(ostream& os, const ModInt& a) {
    return os << a.x;
  }
};

constexpr unsigned MOD = 1'000'000'007;
using Mint = ModInt<MOD>;

constexpr int MAXN = 200005;

Mint fact[MAXN], ifac[MAXN];

Mint C(int n, int k) {
  if (k < 0 || k > n)
    return 0;
  return fact[n] * ifac[k] * ifac[n - k];
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int h, w, n;
  cin >> h >> w >> n;
  vector<pair<int, int>> pts(n + 1);
  for (int i = 0; i < n; i++)
    cin >> pts[i].first >> pts[i].second;
  pts[n] = {h, w};

  sort(pts.begin(), pts.end());

  fact[0] = 1;
  for (int i = 1; i < MAXN; i++)
    fact[i] = fact[i - 1] * i;
  ifac[MAXN - 1] = fact[MAXN - 1].inv();
  for (int i = MAXN - 2; i >= 0; i--)
    ifac[i] = ifac[i + 1] * Mint(i + 1);

  vector<Mint> dp(n + 1);
  for (int i = 0; i <= n; i++) {
    const int x = pts[i].first, y = pts[i].second;
    dp[i] = C(x + y - 2, x - 1);
    for (int j = 0; j < i; j++) {
      const int xj = pts[j].first, yj = pts[j].second;
      if (xj <= x && yj <= y)
        dp[i] -= dp[j] * C(x - xj + y - yj, x - xj);
    }
  }

  cout << dp[n] << '\n';
  return 0;
}

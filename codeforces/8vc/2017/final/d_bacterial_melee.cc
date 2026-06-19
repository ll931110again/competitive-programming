// Codeforces 756 (8VC Venture Cup 2017 - Final Round) — D. Bacterial Melee
// Submission: https://codeforces.com/contest/756/submission/335430920

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 5005;
template <unsigned M_> struct ModInt {
  constexpr unsigned M = M_;
  unsigned x;
  constexpr ModInt() : x(0U) {}
  constexpr ModInt(unsigned x_) : x(x_ % M) {}
  constexpr ModInt(unsigned long long x_) : x(x_ % M) {}
  constexpr ModInt(int x_)
      : x(((x_ %= static_cast<int>(M)) < 0) ? (x_ + static_cast<int>(M)) : x_) {}
  constexpr ModInt(i64 x_)
      : x(((x_ %= static_cast<i64>(M)) < 0) ? (x_ + static_cast<i64>(M)) : x_) {}
  ModInt& operator+=(const ModInt& a) {
    x = ((x += a.x) >= M) ? (x - M) : x;
    return *this;
  }
  ModInt& operator-=(const ModInt& a) {
    x = ((x -= a.x) >= M) ? (x + M) : x;
    return *this;
  }
  ModInt& operator*=(const ModInt& a) {
    x = (static_cast<unsigned long long>(x) * a.x) % M;
    return *this;
  }
  ModInt& operator/=(const ModInt& a) {
    return (*this *= a.inv());
  }
  ModInt pow(i64 e) const {
    if (e < 0)
      return inv().pow(-e);
    ModInt a = *this, b = 1U;
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
      const unsigned q = a / b;
      const unsigned c = a - q * b;
      a = b;
      b = c;
      const int w = y - static_cast<int>(q) * z;
      y = z;
      z = w;
    }
    return ModInt(y);
  }
  ModInt operator+() const {
    return *this;
  }
  ModInt operator-() const {
    ModInt a;
    a.x = x ? (M - x) : 0U;
    return a;
  }
  ModInt operator+(const ModInt& a) const {
    return (ModInt(*this) += a);
  }
  ModInt operator-(const ModInt& a) const {
    return (ModInt(*this) -= a);
  }
  ModInt operator*(const ModInt& a) const {
    return (ModInt(*this) *= a);
  }
  ModInt operator/(const ModInt& a) const {
    return (ModInt(*this) /= a);
  }
  template <class T> friend ModInt operator+(T a, const ModInt& b) {
    return (ModInt(a) += b);
  }
  template <class T> friend ModInt operator-(T a, const ModInt& b) {
    return (ModInt(a) -= b);
  }
  template <class T> friend ModInt operator*(T a, const ModInt& b) {
    return (ModInt(a) *= b);
  }
  template <class T> friend ModInt operator/(T a, const ModInt& b) {
    return (ModInt(a) /= b);
  }
  explicit operator bool() const {
    return x;
  }
  bool operator==(const ModInt& a) const {
    return (x == a.x);
  }
  bool operator!=(const ModInt& a) const {
    return (x != a.x);
  }
  friend ostream& operator<<(ostream& os, const ModInt& a) {
    return os << a.x;
  }
};

constexpr unsigned MOD = 1'000'000'007;
using Mint = ModInt<MOD>;

int n;
string s;

Mint fact[k_max_n], inv[k_max_n];
Mint dp[k_max_n][26], sum_dp[k_max_n];

Mint binom(int x, int y) {
  return fact[x] * inv[y] * inv[x - y];
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  cin >> s;

  string compressed = "";
  for (int i = 0; i < n; i++)
    if (i == 0 || s[i] != s[i - 1]) {
      compressed += s[i];
    }

  for (auto c : compressed) {
    int ds = c - 'a';

    if (dp[1][ds] == 0) {
      sum_dp[1] += 1;
    }
    dp[1][ds] = 1;

    for (int i = 2; i <= n; i++) {
      sum_dp[i] -= dp[i][ds];
      dp[i][ds] = sum_dp[i - 1] - dp[i - 1][ds];
      sum_dp[i] += dp[i][ds];
    }
  }

  fact[0] = 1;
  for (int i = 1; i < k_max_n; i++) {
    fact[i] = fact[i - 1] * i;
  }
  inv[k_max_n - 1] = fact[k_max_n - 1].inv();
  for (int i = k_max_n - 2; i >= 0; i--) {
    inv[i] = inv[i + 1] * (i + 1);
  }

  Mint ans = 0;
  for (int i = 1; i <= n; i++) {
    ans += sum_dp[i] * binom(n - 1, i - 1);
  }
  cout << ans << endl;

  return 0;
}

// Codeforces 1750 (CodeTON Round 3 (Div. 1 + Div. 2, Rated, Prizes!)) — D. Count GCD
// Submission: https://codeforces.com/contest/1750/submission/334473118

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 200005;
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

constexpr unsigned MOD = 998'244'353;
using Mint = ModInt<MOD>;

int T, n, m;
int a[k_max_n];
vector<int> factors;

Mint solve() {
  for (int i = 1; i < n; i++) {
    if (a[i - 1] % a[i] > 0) {
      return 0;
    }
  }

  factors.clear();
  int tmp = a[0];
  for (int i = 2; i * i <= m; i++)
    if (tmp % i == 0) {
      factors.push_back(i);
      while (tmp % i == 0) {
        tmp /= i;
      }
    }
  if (tmp > 1) {
    factors.push_back(tmp);
  }

  Mint ans = 1;
  for (int i = 1; i < n; i++) {
    Mint value = 0;

    tmp = a[i - 1] / a[i];
    vector<int> i_factors;
    for (auto x : factors)
      if (tmp % x == 0) {
        i_factors.push_back(x);
      }

    int k = i_factors.size();
    for (int mask = 0; mask < (1 << k); mask++) {
      int prod = 1;
      int sgn = 1;
      for (int j = 0; j < k; j++)
        if (mask & (1 << j)) {
          sgn *= -1;
          prod *= i_factors[j];
        }
      value += sgn * (m / a[i]) / prod;
    }

    ans *= value;
  }

  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    cout << solve() << endl;
  }
  return 0;
}

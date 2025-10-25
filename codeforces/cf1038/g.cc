#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 400005
using namespace std;

int T, n, k;

template <unsigned M_> struct ModInt {
  static constexpr unsigned M = M_;
  unsigned x;
  constexpr ModInt() : x(0U) {}
  constexpr ModInt(unsigned x_) : x(x_ % M) {}
  constexpr ModInt(unsigned long long x_) : x(x_ % M) {}
  constexpr ModInt(int x_) : x(((x_ %= static_cast<int>(M)) < 0) ? (x_ + static_cast<int>(M)) : x_) {}
  constexpr ModInt(long long x_) : x(((x_ %= static_cast<long long>(M)) < 0) ? (x_ + static_cast<long long>(M)) : x_) {}
  ModInt &operator+=(const ModInt &a) { x = ((x += a.x) >= M) ? (x - M) : x; return *this; }
  ModInt &operator-=(const ModInt &a) { x = ((x -= a.x) >= M) ? (x + M) : x; return *this; }
  ModInt &operator*=(const ModInt &a) { x = (static_cast<unsigned long long>(x) * a.x) % M; return *this; }
  ModInt &operator/=(const ModInt &a) { return (*this *= a.inv()); }
  ModInt pow(long long e) const {
    if (e < 0) return inv().pow(-e);
    ModInt a = *this, b = 1U; for (; e; e >>= 1) { if (e & 1) b *= a; a *= a; } return b;
  }
  ModInt inv() const {
    unsigned a = M, b = x; int y = 0, z = 1;
    for (; b; ) { const unsigned q = a / b; const unsigned c = a - q * b; a = b; b = c; const int w = y - static_cast<int>(q) * z; y = z; z = w; }
    return ModInt(y);
  }
  ModInt operator+() const { return *this; }
  ModInt operator-() const { ModInt a; a.x = x ? (M - x) : 0U; return a; }
  ModInt operator+(const ModInt &a) const { return (ModInt(*this) += a); }
  ModInt operator-(const ModInt &a) const { return (ModInt(*this) -= a); }
  ModInt operator*(const ModInt &a) const { return (ModInt(*this) *= a); }
  ModInt operator/(const ModInt &a) const { return (ModInt(*this) /= a); }
  template <class T> friend ModInt operator+(T a, const ModInt &b) { return (ModInt(a) += b); }
  template <class T> friend ModInt operator-(T a, const ModInt &b) { return (ModInt(a) -= b); }
  template <class T> friend ModInt operator*(T a, const ModInt &b) { return (ModInt(a) *= b); }
  template <class T> friend ModInt operator/(T a, const ModInt &b) { return (ModInt(a) /= b); }
  explicit operator bool() const { return x; }
  bool operator==(const ModInt &a) const { return (x == a.x); }
  bool operator!=(const ModInt &a) const { return (x != a.x); }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &a) { return os << a.x; }
};
 
constexpr unsigned MOD = 998244353;
using Mint = ModInt<MOD>;

Mint fact[maxn], inv[maxn];

Mint binom(int x, int y) {
    return fact[x] * inv[y] * inv[x - y];
}

Mint A(int n, int k) {
    Mint ans = Mint(0);
    for (int i = 0; i <= k; i++) {
        Mint value = binom(n + 1, i) * Mint(k + 1 - i).pow(n);
        if (i & 1) {
            value = -value;
        }
        ans += value;
    }
    return ans;
}

Mint solve(int n, int k) {
    Mint ans = A(n - 1, k - 1);
    ans *= fact[2 * n];
    ans /= n;
    ans /= Mint(2).pow(n);
    return ans;
}

int main() {
    fact[0] = Mint(1);
    for (int i = 1; i < maxn; i++) {
        fact[i] = fact[i - 1] * i;
    }

    inv[maxn - 1] = fact[maxn - 1].inv();
    for (int i = maxn - 2; i >= 0; i--) {   
        inv[i] = inv[i + 1] * (i + 1);
    }

    cin >> T;
    while (T--) {
        cin >> n >> k;
        cout << solve(n, k) << endl;
    }

    return 0;
}
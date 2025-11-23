#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif
 
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <unordered_set>
#define maxn 400005
using namespace std;
 
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
 
constexpr unsigned MOD = 1e9 + 7;
using Mint = ModInt<MOD>;
 
Mint fact[maxn], inv[maxn];
 
Mint binom(int x, int y) {
    if (x < y) {
        return 0;
    }
    return fact[x] * inv[y] * inv[x - y];
}
 
Mint g(int n, int m, int x) {
    if (n < 0 || m < 0) {
        return 0;
    }
 
    if (n == 0) {
        return (m == 0);
    }
 
    Mint ans = 0;
    for (int t = 0; t * (x + 1) <= m; t++) {
        Mint value = binom(n, t) * binom(m + n - 1 - t * (x + 1), n - 1);
        if (t & 1) {
            ans -= value;
        } else {
            ans += value;
        }
    }
    return ans;
}
 
Mint solve_fn(int n, int m, int x) {
    Mint g1 = g(n - 1, m - x, x);
    Mint g2 = g(n - 2, m - 2 * x, x);
 
    Mint x_sum = g1 * x + g2 * x * (n - 1);
    Mint a_sum = g1 * (m - x) / (n - 1) + g2 * x;
    if (n > 2) {
        a_sum += g2 * (m - 2 * x);
    }
 
    return x_sum - a_sum;
}
 
Mint solve(int n, int m) {
    Mint ans = 0;
    for (int x = 1; x <= m; x++) {
        ans += solve_fn(n, m, x);        
    }
    return ans;
}
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
 
    fact[0] = 1;
    for (int i = 1; i < maxn; i++) {
        fact[i] = fact[i - 1] * i;
    }
    inv[maxn - 1] = fact[maxn - 1].inv();
    for (int i = maxn - 2; i >= 0; i--) {
        inv[i] = inv[i + 1] * (i + 1);
    }
 
    int T, n, m;
 
    cin >> T;
    while (T--) {
        cin >> n >> m;
        cout << solve(n, m) << endl;
    }
    return 0;
}

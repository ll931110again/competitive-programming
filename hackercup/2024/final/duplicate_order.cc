#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 10005
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
 
constexpr unsigned MOD = 1'000'000'007;
using Mint = ModInt<MOD>;

int T;
int n, m1, m2, h, sigma;

Mint fact[maxn], inv[maxn];
Mint prefixes[maxn];

Mint binom(int x, int y) {
    if (x < y) {
        return 0;
    }
    return fact[x] * inv[y] * inv[x - y];
}

Mint solve() {
    for (int i = 0; i <= n; i++) {
        prefixes[i] = Mint(sigma - 1).pow(i) * binom(n - h, i);
        if (i) {
            prefixes[i] += prefixes[i - 1];
        }
    }

    Mint ans = 0;
    for (int a = 0; a <= h; a++) if (h - a <= m1) {
        for (int b = 0; a + b <= h; b++) if (h - b <= m2) {
            Mint arrangement = binom(h, a) * binom(h - a, b) * Mint(sigma - 2).pow(h - a - b);
            int bound = min(m1 - (h - a), m2 - (h - b));
            arrangement *= prefixes[bound];

            ans += arrangement;
        }
    }

    Mint ways = Mint(sigma).pow(n) * binom(n, h) * Mint(sigma - 1).pow(h);
    return ans * ways;
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
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
        cerr << it << endl;
        cin >> n >> m1 >> m2 >> h >> sigma;
        cout << "Case #" << it << ": " << solve() << endl;
    }

    return 0;
}
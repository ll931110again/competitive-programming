#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 200005
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

struct event {
  int l, r;
  Mint p;
};

int T, n, k;
event E[maxn];
Mint ans[maxn];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    /*
     * The probability that i is covered by exactly one segment is
        p_1 (1 - p_2)..(1 - p_n) + p_2 (1 - p_1).. (1 - p_n) + ..
        = (sum p_i / (1 - p_i)) * (1 - p_1) .. (1 - p_n)
     */

    cin >> n >> k;
    for (int i = 1; i <= k; i++) {
      int p, q;
      cin >> E[i].l >> E[i].r >> p >> q;
      E[i].p = p;
      E[i].p /= q;
    }

    vector< pair<int,int> > events;
    for (int i = 1; i <= k; i++) {
      events.push_back({E[i].l, i});
      events.push_back({E[i].r + 1, -i});
    }
    sort(events.begin(), events.end());

    Mint sum_value = 0, prod_value = 1;

    int it = 0;
    for (int i = 1; i <= n; i++) {
      while (it < events.size() && events[it].first <= i) {
        int pos = abs(events[it].second);
        if (events[it].second > 0) {
          sum_value += E[pos].p / (1 - E[pos].p);
          prod_value *= (1 - E[pos].p);
        } else {
          sum_value -= E[pos].p / (1 - E[pos].p);
          prod_value /= (1 - E[pos].p);
        }
        it++;
      }
      ans[i] = sum_value * prod_value;
    }

    for (int i = 1; i <= n; i++) {
      cout << ans[i];
      if (i < n) {
        cout << " ";
      } else {
        cout << endl;
      }
    }
  
    return 0;
}
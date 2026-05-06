#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <vector>
#define maxn 100005
using namespace std;

template <int MOD = 1'000'000'007> struct Modular {
  int value;
  static const int MOD_value = MOD;

  Modular(long long v = 0) {
    value = v % MOD;
    if (value < 0)
      value += MOD;
  }
  Modular(long long a, long long b) : value(0) {
    *this += a;
    *this /= b;
  }

  Modular &operator+=(Modular const &b) {
    value += b.value;
    if (value >= MOD)
      value -= MOD;
    return *this;
  }
  Modular &operator-=(Modular const &b) {
    value -= b.value;
    if (value < 0)
      value += MOD;
    return *this;
  }
  Modular &operator*=(Modular const &b) {
    value = (long long)value * b.value % MOD;
    return *this;
  }

  friend Modular mexp(Modular a, long long e) {
    Modular res = 1;
    while (e) {
      if (e & 1)
        res *= a;
      a *= a;
      e >>= 1;
    }
    return res;
  }
  friend Modular inverse(Modular a) { return mexp(a, MOD - 2); }

  Modular &operator/=(Modular const &b) { return *this *= inverse(b); }
  friend Modular operator+(Modular a, Modular const b) { return a += b; }
  friend Modular operator-(Modular a, Modular const b) { return a -= b; }
  friend Modular operator-(Modular const a) { return 0 - a; }
  friend Modular operator*(Modular a, Modular const b) { return a *= b; }
  friend Modular operator/(Modular a, Modular const b) { return a /= b; }
  friend std::ostream &operator<<(std::ostream &os, Modular const &a) {
    return os << a.value;
  }
  friend bool operator==(Modular const &a, Modular const &b) {
    return a.value == b.value;
  }
  friend bool operator!=(Modular const &a, Modular const &b) {
    return a.value != b.value;
  }
};

int T, n;
using Mint = Modular<1000000007>;

vector<int> adj[maxn];
Mint dp[maxn];
int sz[maxn];
Mint fact[maxn], inv[maxn], p2[maxn];

Mint binom(int x, int y) { return fact[x] * inv[y] * inv[x - y]; }

Mint rec(int u, int p) {
  sz[u] = 1;
  dp[u] = 1;

  int odd_count = 0, even_count = 0;
  for (auto v : adj[u])
    if (v != p) {
      rec(v, u);
      dp[u] *= dp[v];

      sz[u] += sz[v];

      if (sz[v] & 1) {
        odd_count++;
      } else {
        even_count++;
      }
    }

  if (odd_count > 0) {
    dp[u] *= p2[even_count] * binom(odd_count, odd_count / 2);
  }

  return dp[u];
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  fact[0] = 1;
  for (int i = 1; i < maxn; i++) {
    fact[i] = fact[i - 1] * i;
  }
  inv[maxn - 1] = Mint(1) / fact[maxn - 1];
  for (int i = maxn - 2; i >= 0; i--) {
    inv[i] = inv[i + 1] * (i + 1);
  }

  p2[0] = 1;
  for (int i = 1; i < maxn; i++) {
    p2[i] = p2[i - 1] * 2;
  }

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 1; i <= n; i++) {
      adj[i].clear();
    }
    for (int i = 1; i < n; i++) {
      int u, v;
      cin >> u >> v;
      adj[u].push_back(v);
      adj[v].push_back(u);
    }

    for (int i = 1; i <= n; i++) {
      dp[i] = -1;
    }
    cout << rec(1, -1) << endl;
  }

  return 0;
}
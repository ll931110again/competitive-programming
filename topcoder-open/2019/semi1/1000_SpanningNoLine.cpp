#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
#define mod 998244353
#define maxm 2000005

i64 fact[maxm], inv[maxm];

struct SpanningNoLine {
  int countSpanning(int n, int m) {
    fact[0] = 1;
    for (int i = 1; i <= 2 * m; i++) {
      fact[i] = fact[i - 1] * i % mod;
    }
    inv[2 * m] = get_power(fact[2 * m], mod - 2);
    for (int i = 2 * m - 1; i >= 0; i--) {
      inv[i] = inv[i + 1] * (i + 1) % mod;
    }

    i64 nx = get_power(n, n - 2), ni = get_power(n, mod - 2);

    i64 ret = 0;
    for (int k = 0; k < m; k++) {
      i64 val = binom(2 * m - k - 1, k);
      val = val * nx % mod;
      if (k % 2 == 0) {
        ret = (ret + val) % mod;
      } else {
        ret = (ret + mod - val) % mod;
      }
      nx = (nx * ni) % mod;
    }
    return ret;
  }

  i64 binom(int x, int y) {
    if (x < y) {
      return 0;
    }
    i64 up = fact[x];
    i64 down = inv[y] * inv[x - y] % mod;
    return (up * down) % mod;
  }

  i64 get_power(int x, int p) {
    if (!p) {
      return 1;
    }
    i64 q = get_power(x, p / 2);
    q = (q * q) % mod;
    if (p & 1) {
      q = (q * x) % mod;
    }
    return q;
  }
};

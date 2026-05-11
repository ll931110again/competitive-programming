#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>
#define maxn 400005
using namespace std;

static constexpr unsigned MOD = 1'000'000'007;
#include "../../lib/modint.h"
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

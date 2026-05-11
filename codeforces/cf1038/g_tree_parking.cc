#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <vector>
#define maxn 400005
using namespace std;

int T, n, k;

static constexpr unsigned MOD = 998244353;
#include "../../lib/modint.h"
using Mint = ModInt<MOD>;

Mint fact[maxn], inv[maxn];

Mint binom(int x, int y) { return fact[x] * inv[y] * inv[x - y]; }

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
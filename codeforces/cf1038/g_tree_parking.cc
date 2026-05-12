// CF 2122G - Tree Parking: https://codeforces.com/contest/2122/problem/G
//
// Model: rooted tree at 1. Car i enters at time l_i, travels root -> i, parks at
// i until it leaves at r_i (same path back). A parked car blocks its vertex for
// others. The tree is "valid" for schedules (l, r) if every car can follow its
// route without conflict. Count valid (l, r) over all labeled trees on n
// vertices with exactly k leaves (mod 998244353).
//
// Combinatorial closed form (see editorial / writeups):
//   answer = A(n-1, k-1) * (2n)! / (n * 2^n),
// where A(n, k) is the Eulerian number: permutations of [n] with exactly k
// ascents (equivalently k descents of the reverse). One inclusion-exclusion form
// is  sum_{i=0}^{k} (-1)^i * C(n+1, i) * (k+1-i)^n  (Wikipedia: Eulerian number).

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

// Eulerian number A(n, k): sum_{i=0}^{k} (-1)^i * C(n+1,i) * (k+1-i)^n.
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

// Total valid (l, r) over all labeled n-vertex trees with k leaves.
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
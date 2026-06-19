// CF 1864H - Asterism Stream
// https://codeforces.com/contest/1864/problem/H
//
// Problem
// -------
// Start with x = 1. Each move, with probability 1/2: x += 1 or x *= 2.
// Find the expected number of moves until x >= n (1 <= n <= 1e18), modulo 998244353.
//
// Overview
// --------
// Let k = 1/2 (mod M) and f(x) = expected moves from state x. Then
//   f(x) = 0           if x >= n
//   f(x) = k(f(x+1) + f(2x)) + 1   otherwise.
//
// Represent a function on an interval as a sum of geometric terms {a, b} meaning a * k^{b*x}.
// On [n, 2n] we have f = 0 = {0, 0}. If [2l, 2r] is already a sum of such terms, the
// recurrence above rewrites every f(x) on [l, r] as O(1) new terms (merge geometric series
// using k^{2b+1} != 1). Halve (l, r) until l = 1; evaluate f(1) = n0 + sum_i Q[i]*k^{-2^i}.
//
// Complexity: O(log n) layers, O(log n) terms per layer => O(log^2 n) per test case.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int MOD = 998244353;

i64 power(i64 x, i64 p) {
  if (p < 0)
    return power(power(x, MOD - 2), -p);
  i64 ans = 1;
  x %= MOD;
  while (p > 0) {
    if (p & 1)
      ans = ans * x % MOD;
    x = x * x % MOD;
    p >>= 1;
  }
  return ans;
}

void solve_one(i64 n) {
  const i64 k = power(2, MOD - 2);
  i64 l = n;
  i64 r = max(1LL, (n - 1) * 2);
  i64 n0 = 0;
  vector<i64> Q;

  while (l > 1) {
    vector<i64> P;
    l = (l + 1) / 2;
    r /= 2;
    i64 s = n0;
    for (int i = 0; i < (int)Q.size(); ++i) {
      s = (s + Q[i] * power(power(k, -(1LL << i)), r + 1)) % MOD;
    }
    P.push_back(((power(k, r + 1) * s - power(k, r)) % MOD + MOD) % MOD);
    for (int i = 0; i < (int)Q.size(); ++i) {
      const i64 base = power(k, 1 - (1LL << (i + 1)));
      const i64 inv = power((base - 1 + MOD) % MOD, MOD - 2);
      P[0] = (P[0] + inv * Q[i] % MOD * power(base, r + 1) % MOD * k) % MOD;
      P.push_back(((MOD - Q[i] * k % MOD * inv) % MOD + MOD) % MOD);
    }
    P[0] = (P[0] + (MOD - n0) * power(k, r + 1)) % MOD;
    for (auto& x : P)
      x %= MOD;
    Q = move(P);
    n0 += 2;
  }

  i64 s = n0;
  for (int i = 0; i < (int)Q.size(); ++i) {
    s = (s + Q[i] * power(power(k, -(1LL << i)), 1)) % MOD;
  }
  cout << s << '\n';
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--) {
    i64 n;
    cin >> n;
    solve_one(n);
  }
  return 0;
}

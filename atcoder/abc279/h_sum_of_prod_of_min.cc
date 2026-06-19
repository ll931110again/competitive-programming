// ABC279 Ex (H) — Sum of Prod of Min
// https://atcoder.jp/contests/abc279/tasks/abc279_h
//
// Sum over positive S with sum(S)=M of prod_k min(k, S_k), modulo prime 200003.
// Generating function + Euler's pentagonal theorem → O(sqrt N) binomials
// C(M+N-e-1, 2N-1) mod 200003 with N up to 1e12 (Lucas theorem).

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int MOD = 200003;

int pow_mod(int a, int mod, i64 e) {
  i64 r = 1, b = a % mod;
  while (e > 0) {
    if (e & 1)
      r = r * b % mod;
    b = b * b % mod;
    e >>= 1;
  }
  return int(r);
}

struct CombPrime {
  int p;
  vector<int> fac, ifac;

  explicit CombPrime(int p_) : p(p_) {
    fac.assign(p, 1);
    for (int i = 1; i < p; i++)
      fac[i] = int(1LL * fac[i - 1] * i % p);
    ifac.assign(p, 1);
    ifac[p - 1] = pow_mod(fac[p - 1], p, p - 2);
    for (int i = p - 1; i >= 1; i--)
      ifac[i - 1] = int(1LL * ifac[i] * i % p);
  }

  int small_c(int n, int k) const {
    if (k < 0 || k > n)
      return 0;
    return int(1LL * fac[n] * ifac[k] % p * ifac[n - k] % p);
  }

  int lucas(i64 n, i64 k) const {
    if (k < 0 || n < k)
      return 0;
    i64 res = 1;
    while (n > 0 || k > 0) {
      const int ni = int(n % p);
      const int ki = int(k % p);
      if (ki > ni)
        return 0;
      res = res * small_c(ni, ki) % p;
      n /= p;
      k /= p;
    }
    return int(res);
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 n, m;
  cin >> n >> m;
  CombPrime comb(MOD);

  i64 ans = 0;
  const i64 lim = m - n;

  for (i64 i = 0;; i++) {
    const i64 j = i * (3 * i + 1) / 2;
    if (j > lim)
      break;
    const i64 sign = (i & 1) ? -1 : 1;
    ans = (ans + sign * comb.lucas(m + n - j - 1, 2 * n - 1)) % MOD;
  }
  for (i64 i = 1;; i++) {
    const i64 j = i * (3 * i - 1) / 2;
    if (j > lim)
      break;
    const i64 sign = (i & 1) ? -1 : 1;
    ans = (ans + sign * comb.lucas(m + n - j - 1, 2 * n - 1)) % MOD;
  }
  if (ans < 0)
    ans += MOD;
  cout << ans << '\n';
  return 0;
}

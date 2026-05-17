// Classic Lucas theorem: C(n, m) mod p for huge n, m and prime p <= 10^5.
// Same I/O as Luogu P3807 / Library Checker "Binomial Coefficient (mod prime)".
//
// Lucas: C(n,m) mod p = prod_i C(n_i, m_i) mod p where n_i, m_i are base-p digits.

#include <bits/stdc++.h>
using namespace std;

static int pow_mod(int a, int mod, long long e) {
  long long r = 1, b = a % mod;
  while (e > 0) {
    if (e & 1) r = r * b % mod;
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
    for (int i = 1; i < p; i++) fac[i] = int(1LL * fac[i - 1] * i % p);
    ifac.assign(p, 1);
    ifac[p - 1] = pow_mod(fac[p - 1], p, p - 2);
    for (int i = p - 1; i >= 1; i--) ifac[i - 1] = int(1LL * ifac[i] * i % p);
  }

  int smallC(int n, int k) const {
    if (k < 0 || k > n) return 0;
    return int(1LL * fac[n] * ifac[k] % p * ifac[n - k] % p);
  }

  int lucas(long long n, long long m) const {
    if (m < 0 || n < m) return 0;
    long long res = 1;
    while (n > 0 || m > 0) {
      const int ni = int(n % p);
      const int mi = int(m % p);
      if (mi > ni) return 0;
      res = res * smallC(ni, mi) % p;
      n /= p;
      m /= p;
    }
    return int(res);
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--) {
    long long n, m;
    int p;
    cin >> n >> m >> p;
    CombPrime comb(p);
    cout << comb.lucas(n, m) << '\n';
  }
  return 0;
}

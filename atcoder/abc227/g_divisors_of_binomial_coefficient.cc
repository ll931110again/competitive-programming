// ABC227 G - Divisors of Binomial Coefficient: https://atcoder.jp/contests/abc227/tasks/abc227_g
//
// C(N,K) = prod_{i=1}^K (N-K+i) / i. Sieve-factor 1..K and [N-K+1..N] (editorial).

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr unsigned MOD = 998244353;
constexpr int PMAX = 1000000;

#ifdef __GNUC__
#pragma GCC optimize("O3,unroll-loops")
#endif

inline unsigned mul_mod(unsigned a, unsigned b) {
  return unsigned((unsigned long long)a * b % MOD);
}

inline unsigned pow_mod_factor(i64 e) {
  i64 v = (e + 1) % (i64)MOD;
  if (v < 0)
    v += MOD;
  return unsigned(v);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 N;
  int K;
  cin >> N >> K;

  if (K == 0 || K == N) {
    cout << 1 << '\n';
    return 0;
  }

  int primes[80000];
  int pc = 0;
  char comp[PMAX + 1];
  memset(comp, 0, sizeof comp);
  for (int i = 2; i <= PMAX; i++) {
    if (comp[i])
      continue;
    primes[pc++] = i;
    for (int j = i + i; j <= PMAX; j += i)
      comp[j] = 1;
  }

  int cnt[PMAX + 1];
  memset(cnt, 0, sizeof cnt);

  int den[PMAX + 1];
  for (int i = 1; i <= K; i++)
    den[i] = i;

  const i64 L = N - K + 1;
  vector<i64> num(K);
  for (int i = 0; i < K; i++)
    num[i] = L + i;

  for (int pi = 0; pi < pc; pi++) {
    const int p = primes[pi];
    if (p > N)
      break;

    if (p <= K) {
      for (int i = p; i <= K; i += p) {
        while (den[i] % p == 0) {
          cnt[p]--;
          den[i] /= p;
        }
      }
    }

    const int first = int((L + p - 1) / p * p - L);
    for (int idx = first; idx < K; idx += p) {
      while (num[idx] % p == 0) {
        cnt[p]++;
        num[idx] /= p;
      }
    }
  }

  vector<i64> big;
  big.reserve(70000);
  for (int i = 0; i < K; i++) {
    if (num[i] > 1)
      big.push_back(num[i]);
  }

  unsigned ans = 1;
  for (int pi = 0; pi < pc; pi++) {
    const int p = primes[pi];
    if (cnt[p] > 0)
      ans = mul_mod(ans, pow_mod_factor(cnt[p]));
  }

  if (!big.empty()) {
    sort(big.begin(), big.end());
    big.erase(unique(big.begin(), big.end()), big.end());
    const i64 M = N - K;
    for (i64 p : big) {
      const i64 e = N / p - K / p - M / p;
      if (e > 0)
        ans = mul_mod(ans, pow_mod_factor(e));
    }
  }

  cout << ans << '\n';
  return 0;
}

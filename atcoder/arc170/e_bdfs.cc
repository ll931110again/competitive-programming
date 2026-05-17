// ARC170 E - BDFS: https://atcoder.jp/contests/arc170/tasks/arc170_e
//
// Equivalent O(log N) closed form from the pair decomposition.
// Cayley–Hamilton / Berlekamp–Massey route: e_bdfs_cayley.cc

#include <bits/stdc++.h>
using namespace std;

static constexpr int MOD = 998244353;

static int mod_pow(long long a, long long e) {
  long long r = 1;
  a %= MOD;
  while (e > 0) {
    if (e & 1)
      r = r * a % MOD;
    a = a * a % MOD;
    e >>= 1;
  }
  return (int)r;
}

static int mod_inv(int a) {
  return mod_pow(a, MOD - 2);
}

static int solve(long long N, int P) {
  long long n = N - 1;
  int inv2 = (MOD + 1) / 2;
  int inv100 = mod_inv(100);

  int r = (int)((2LL * P % MOD) * inv100 % MOD);
  r = (r + MOD - 1) % MOD;

  int nn = (int)(n % MOD);
  int n1 = (int)((n - 1) % MOD);

  int ans = (int)(((__int128)nn * (nn + 1) % MOD) * inv2 % MOD);

  int rn = mod_pow(r, n);
  int geom, weighted;
  if (r == 1) {
    geom = nn;
    weighted = (int)(((__int128)n1 * ((n1 + MOD - 1) % MOD) % MOD) * inv2 % MOD);
  } else {
    int denom = (r + MOD - 1) % MOD;
    geom = (int)(((__int128)(rn + MOD - 1) * mod_inv(denom)) % MOD);
    int val = (int)(((__int128)n1 * rn % MOD + 1 + MOD - geom) % MOD);
    weighted = (int)((__int128)val * mod_inv(denom) % MOD);
  }
  ans = (ans + (int)((__int128)nn * geom % MOD)) % MOD;
  ans = (ans + MOD - weighted) % MOD;
  ans = (int)((__int128)ans * inv2 % MOD);
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--) {
    long long N;
    int P;
    cin >> N >> P;
    cout << solve(N, P) << '\n';
  }
  return 0;
}

// Facebook Hacker Cup 2011 R2 - Bonus Assignments
// gcd(all bonuses) = 1  <=>  for every bottle price p>=2 some worker has remainder > 0
// Answer = sum_d mu(d) * F(d), F(d) = assignments with all bonuses divisible by d.

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int MOD = 1000000007;
constexpr int MAXV = 1000005;

int mu_arr[MAXV];

void sieve_mobius(int limit) {
  vector<int> primes;
  vector<bool> comp(limit + 1);
  mu_arr[0] = 0;
  mu_arr[1] = 1;
  for (int i = 2; i <= limit; ++i) {
    if (!comp[i]) {
      primes.push_back(i);
      mu_arr[i] = -1;
    }
    for (int p : primes) {
      long long v = 1LL * i * p;
      if (v > limit)
        break;
      comp[v] = true;
      if (i % p == 0) {
        mu_arr[v] = 0;
        break;
      }
      mu_arr[v] = -mu_arr[i];
    }
  }
}

int mod_pow(long long a, long long e) {
  long long r = 1 % MOD;
  a %= MOD;
  while (e > 0) {
    if (e & 1)
      r = r * a % MOD;
    a = a * a % MOD;
    e >>= 1;
  }
  return (int)r;
}

int range_sum(const vector<int>& pref, int lo, int hi) {
  if (lo > hi)
    return 0;
  if (lo <= 0)
    return pref[hi];
  int v = pref[hi] - pref[lo - 1];
  if (v < 0)
    v += MOD;
  return v;
}

int count_min_max(int a, int b, int c, int dmax, const vector<int>& pref) {
  if (a > b || c > dmax)
    return 0;
  long long total = 0;
  for (int m = a; m <= b; ++m) {
    int L = max(0, c - m);
    int R = dmax - m;
    if (L > R)
      continue;
    total += range_sum(pref, L, R);
    if (total >= MOD)
      total %= MOD;
  }
  return (int)(total % MOD);
}

int f_multiple_of_d(int A, int B, int C, int D, int d, const vector<int>& pref) {
  int a = (A + d - 1) / d;
  int b = B / d;
  int c = (C + d - 1) / d;
  int dd = D / d;
  return count_min_max(a, b, c, dd, pref);
}

void build_h_prefix(int n, int max_k, vector<int>& pref) {
  vector<int> pow_tab(max_k + 3);
  for (int i = 0; i < (int)pow_tab.size(); ++i)
    pow_tab[i] = mod_pow(i, n);

  vector<int> h(max_k + 1);
  h[0] = 1;
  for (int k = 1; k <= max_k; ++k) {
    long long v = pow_tab[k + 1] - 2LL * pow_tab[k] + pow_tab[k - 1];
    v %= MOD;
    if (v < 0)
      v += MOD;
    h[k] = (int)v;
  }

  pref.resize(max_k + 1);
  long long s = 0;
  for (int k = 0; k <= max_k; ++k) {
    s += h[k];
    if (s >= MOD)
      s %= MOD;
    pref[k] = (int)s;
  }
}

int solve_one(int n, int A, int B, int C, int D) {
  int max_gap = max({D - A, D - C, B - A, B - C}) + 3;
  if (max_gap > MAXV)
    max_gap = MAXV;

  vector<int> pref;
  build_h_prefix(n, max_gap, pref);

  long long ans = 0;
  for (int d = 1; d <= B; ++d) {
    int mu = mu_arr[d];
    if (mu == 0)
      continue;
    int fd = f_multiple_of_d(A, B, C, D, d, pref);
    ans += (long long)mu * fd;
    ans = (ans % MOD + MOD) % MOD;
  }
  return (int)ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  sieve_mobius(MAXV - 1);

  int T;
  if (!(cin >> T))
    return 0;
  for (int tc = 1; tc <= T; ++tc) {
    int N, A, B, C, D;
    cin >> N >> A >> B >> C >> D;
    int ans = solve_one(N, A, B, C, D);
    cout << "Case #" << tc << ": " << ans << "\n";
  }
  return 0;
}

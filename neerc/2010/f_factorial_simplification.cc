/*
 * Solution for problem F: Factorial Simplification.
 *
 * Prime exponents of (prod p_i!) / (prod q_j!). Integer iff all exponents >= 0.
 * Greedy (NEERC review): strip as many copies of R! as possible for R from
 * (next_prime(max(p,q)) - 1) down to 2. Upper bound is tight per statement.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;

static bool is_prime(int x) {
  if (x < 2)
    return false;
  for (int d = 2; (long long)d * d <= x; ++d)
    if (x % d == 0)
      return false;
  return true;
}

static int next_prime_after(int x) {
  int y = x + 1;
  while (!is_prime(y))
    ++y;
  return y;
}

static vector<int> primes;

static void sieve(int lim) {
  vector<char> comp(lim + 1, 0);
  for (int i = 2; (long long)i * i <= lim; ++i) {
    if (!comp[i])
      for (int j = i * i; j <= lim; j += i)
        comp[j] = 1;
  }
  primes.clear();
  for (int i = 2; i <= lim; ++i)
    if (!comp[i])
      primes.push_back(i);
}

static long long vp_fact(int p, int n) {
  long long s = 0;
  long long x = n;
  while (x) {
    x /= p;
    s += x;
  }
  return s;
}

static void add_factorial(vector<long long>& exp, int n, int sign) {
  for (int p : primes) {
    if (p > n)
      break;
    exp[p] += sign * vp_fact(p, n);
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("factorial.in", "r", stdin);
  freopen("factorial.out", "w", stdout);

  int n, m;
  cin >> n >> m;
  vector<int> pnum(n), qnum(m);
  int mxv = 0;
  for (int i = 0; i < n; ++i) {
    cin >> pnum[i];
    mxv = max(mxv, pnum[i]);
  }
  for (int i = 0; i < m; ++i) {
    cin >> qnum[i];
    mxv = max(mxv, qnum[i]);
  }

  int np = next_prime_after(mxv);
  int r_hi = max(2, np - 1);
  int lim = max(r_hi, mxv) + 50;
  sieve(lim);

  vector<long long> exp(lim + 2, 0);
  for (int v : pnum)
    add_factorial(exp, v, +1);
  for (int v : qnum)
    add_factorial(exp, v, -1);

  for (int p : primes) {
    if (p > lim)
      break;
    if (exp[p] < 0) {
      cout << -1 << '\n';
      return 0;
    }
  }

  vector<pair<int, long long>> out;
  for (int r = r_hi; r >= 2; --r) {
    long long take = LLONG_MAX;
    for (int p : primes) {
      if (p > r)
        break;
      long long vp = vp_fact(p, r);
      if (vp == 0)
        continue;
      take = min(take, exp[p] / vp);
    }
    if (take == LLONG_MAX || take <= 0)
      continue;
    out.push_back({r, take});
    for (int p : primes) {
      if (p > r)
        break;
      long long vp = vp_fact(p, r);
      __int128 dec = (__int128)take * (__int128)vp;
      exp[p] -= (long long)dec;
    }
  }

  for (int p : primes) {
    if (p > lim)
      break;
    if (exp[p] != 0) {
      cout << 0 << '\n';
      return 0;
    }
  }

  cout << (int)out.size() << '\n';
  for (auto& e : out)
    cout << e.first << ' ' << e.second << '\n';
  return 0;
}

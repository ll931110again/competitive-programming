// Facebook Hacker Cup 2011 R2 - Problem C: Studious Student II
//
// Count operation sequences (i,j,c) starting from a string of 'a'/'b'.
// Each op replaces s[i..j] (i<j) with one occurrence of c (which appears in s[i..j]).
//
// Key idea. Any sequence of ops corresponds to merging contiguous original
// positions into intervals. Sub-sequences on disjoint intervals are interleaved
// in time, contributing a multinomial coefficient -- exactly what EGF
// multiplication encodes (binomial convolution of integer sequences).
//
// For each interval I = [l, r] of the original string, store EGF coefficients
// (a_t = number of t-op sequences) for:
//   P(I)              total op-sequences on I
//   Q_a(I), Q_b(I)    collapse-sequences ending in single char 'a' / 'b'
//   v_a(I), v_b(I)    sequences whose result is an all-'a' / all-'b' string (len >= 1)
//
// Recurrence (decompose by leftmost final interval [l, m]):
//   alpha_a = sum_{m=l}^{r-1} Q_a([l,m]) (x) v_a([m+1,r])
//   alpha_b = sum_{m=l}^{r-1} Q_b([l,m]) (x) v_b([m+1,r])
//   beta    = sum_{m=l}^{r-1} (Q_a+Q_b)([l,m]) (x) P([m+1,r])
// The last op of a collapse-to-c uses an intermediate of length >= 2 containing c.
// After algebra (self-references in Q_a, Q_b cancel):
//   Q_a = integ(beta - alpha_b),  Q_b = integ(beta - alpha_a)
//   v_a = alpha_a + Q_a,          v_b = alpha_b + Q_b
//   P   = beta + Q_a + Q_b
// Answer = sum of coefficients of P([0, n-1]).

#include <iostream>
#include <string>
#include <vector>
using namespace std;

using u64 = unsigned long long;
static constexpr u64 MOD = 1000000007ULL;
static constexpr int MAXN = 64;

static u64 BIN[MAXN + 1][MAXN + 1];

static void precompute_binomials() {
  for (int i = 0; i <= MAXN; ++i) {
    BIN[i][0] = 1;
    for (int j = 1; j <= i; ++j) {
      u64 v = BIN[i - 1][j - 1] + (j <= i - 1 ? BIN[i - 1][j] : 0);
      BIN[i][j] = v % MOD;
    }
  }
}

struct Poly {
  vector<u64> a;
  Poly() = default;
  explicit Poly(int n) : a(n, 0) {}
  int size() const {
    return (int)a.size();
  }
};

static Poly add(const Poly& x, const Poly& y) {
  int n = max(x.size(), y.size());
  Poly r(n);
  for (int i = 0; i < x.size(); ++i)
    r.a[i] = x.a[i];
  for (int i = 0; i < y.size(); ++i) {
    u64 v = r.a[i] + y.a[i];
    r.a[i] = v >= MOD ? v - MOD : v;
  }
  return r;
}

static Poly sub(const Poly& x, const Poly& y) {
  int n = max(x.size(), y.size());
  Poly r(n);
  for (int i = 0; i < x.size(); ++i)
    r.a[i] = x.a[i];
  for (int i = 0; i < y.size(); ++i) {
    u64 v = r.a[i] + MOD - y.a[i];
    r.a[i] = v >= MOD ? v - MOD : v;
  }
  return r;
}

// Binomial convolution: c_t = sum_k C(t,k) * x_k * y_{t-k}.
static Poly bmul(const Poly& x, const Poly& y) {
  int nx = x.size(), ny = y.size();
  if (nx == 0 || ny == 0)
    return Poly();
  Poly r(nx + ny - 1);
  for (int i = 0; i < nx; ++i) {
    u64 xi = x.a[i];
    if (!xi)
      continue;
    for (int j = 0; j < ny; ++j) {
      u64 yj = y.a[j];
      if (!yj)
        continue;
      u64 prod = xi * yj % MOD;
      prod = prod * BIN[i + j][i] % MOD;
      u64 v = r.a[i + j] + prod;
      r.a[i + j] = v >= MOD ? v - MOD : v;
    }
  }
  return r;
}

// Integrate EGF: shift coefficients right by one (insert 0 at index 0).
static Poly integ(const Poly& x) {
  Poly r(x.size() + 1);
  for (int i = 0; i < x.size(); ++i)
    r.a[i + 1] = x.a[i];
  return r;
}

static u64 solve(const string& s) {
  int n = (int)s.size();

  vector<vector<Poly>> P(n, vector<Poly>(n));
  vector<vector<Poly>> Qa(n, vector<Poly>(n));
  vector<vector<Poly>> Qb(n, vector<Poly>(n));
  vector<vector<Poly>> Va(n, vector<Poly>(n));
  vector<vector<Poly>> Vb(n, vector<Poly>(n));

  for (int i = 0; i < n; ++i) {
    P[i][i] = Poly(1);
    P[i][i].a[0] = 1;
    Qa[i][i] = Poly(1);
    Qb[i][i] = Poly(1);
    Va[i][i] = Poly(1);
    Vb[i][i] = Poly(1);
    if (s[i] == 'a') {
      Qa[i][i].a[0] = 1;
      Va[i][i].a[0] = 1;
    } else {
      Qb[i][i].a[0] = 1;
      Vb[i][i].a[0] = 1;
    }
  }

  for (int len = 2; len <= n; ++len) {
    for (int l = 0; l + len <= n; ++l) {
      int r = l + len - 1;
      Poly alpha_a, alpha_b, beta;
      for (int m = l; m < r; ++m) {
        Poly qab = add(Qa[l][m], Qb[l][m]);
        alpha_a = add(alpha_a, bmul(Qa[l][m], Va[m + 1][r]));
        alpha_b = add(alpha_b, bmul(Qb[l][m], Vb[m + 1][r]));
        beta = add(beta, bmul(qab, P[m + 1][r]));
      }
      Qa[l][r] = integ(sub(beta, alpha_b));
      Qb[l][r] = integ(sub(beta, alpha_a));
      Va[l][r] = add(alpha_a, Qa[l][r]);
      Vb[l][r] = add(alpha_b, Qb[l][r]);
      P[l][r] = add(add(beta, Qa[l][r]), Qb[l][r]);
    }
  }

  u64 ans = 0;
  for (u64 c : P[0][n - 1].a) {
    ans += c;
    if (ans >= MOD)
      ans -= MOD;
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  precompute_binomials();

  int T;
  if (!(cin >> T))
    return 0;
  for (int tc = 1; tc <= T; ++tc) {
    string s;
    cin >> s;
    cout << "Case #" << tc << ": " << solve(s) << "\n";
  }
  return 0;
}

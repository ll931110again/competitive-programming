#include <bits/stdc++.h>
using namespace std;

using namespace std;
static const int MOD = 1000000009;
static const int MAXN = 200005;

static int fact[MAXN], invFact[MAXN], pow26[MAXN], pow10[MAXN];

static int addmod(int a, int b) {
  int s = a + b;
  if (s >= MOD)
    s -= MOD;
  return s;
}

static int submod(int a, int b) {
  int s = a - b;
  if (s < 0)
    s += MOD;
  return s;
}

static int mulmod(long long a, long long b) {
  return int((a * b) % MOD);
}

static int modpow(int a, long long e) {
  long long r = 1, x = a;
  while (e) {
    if (e & 1)
      r = r * x % MOD;
    x = x * x % MOD;
    e >>= 1;
  }
  return int(r);
}

static vector<int> convolveNaive(const vector<int>& a, const vector<int>& b) {
  if (a.empty() || b.empty())
    return vector<int>();
  vector<int> c(a.size() + b.size() - 1);
  for (int i = 0; i < (int)a.size(); ++i)
    if (a[i])
      for (int j = 0; j < (int)b.size(); ++j)
        if (b[j])
          c[i + j] = addmod(c[i + j], mulmod(a[i], b[j]));
  return c;
}

static vector<int> convolve(const vector<int>& a, const vector<int>& b) {
  int n = (int)a.size(), m = (int)b.size();
  if (!n || !m)
    return vector<int>();
  if (min(n, m) <= 64)
    return convolveNaive(a, b);

  int sz = max(n, m);
  int half = 1;
  while (half < sz)
    half <<= 1;
  half >>= 1;

  vector<int> a0(a.begin(), a.begin() + min(n, half));
  vector<int> a1;
  if (n > half)
    a1.assign(a.begin() + half, a.end());
  vector<int> b0(b.begin(), b.begin() + min(m, half));
  vector<int> b1;
  if (m > half)
    b1.assign(b.begin() + half, b.end());

  vector<int> z0 = convolve(a0, b0);
  vector<int> z2 = convolve(a1, b1);

  vector<int> a01(a0);
  a01.resize(half);
  for (int i = 0; i < (int)a1.size(); ++i)
    a01[i] = addmod(a01[i], a1[i]);
  vector<int> b01(b0);
  b01.resize(half);
  for (int i = 0; i < (int)b1.size(); ++i)
    b01[i] = addmod(b01[i], b1[i]);

  vector<int> z1 = convolve(a01, b01);
  for (int i = 0; i < (int)z0.size(); ++i)
    z1[i] = submod(z1[i], z0[i]);
  for (int i = 0; i < (int)z2.size(); ++i)
    if (i < (int)z1.size())
      z1[i] = submod(z1[i], z2[i]);
    else
      z1.push_back(submod(0, z2[i]));

  vector<int> res(z0);
  res.resize(max(res.size(), (size_t)half + z1.size()));
  for (int i = 0; i < (int)z1.size(); ++i)
    res[half + i] = addmod(res[half + i], z1[i]);
  res.resize(max(res.size(), (size_t)2 * half + z2.size()));
  for (int i = 0; i < (int)z2.size(); ++i)
    res[2 * half + i] = addmod(res[2 * half + i], z2[i]);
  return res;
}

class Passwords {
public:
  int countValid(int N, int L, int U, int D) {
    if (L + U + D > N)
      return 0;
    int E = N - L - U - D;

    vector<int> va(E + 1), vb(E + 1), vc(E + 1);
    for (int i = 0; i <= E; ++i) {
      va[i] = mulmod(invFact[L + i], pow26[i]);
      vb[i] = mulmod(invFact[U + i], pow26[i]);
      vc[i] = mulmod(invFact[D + i], pow10[i]);
    }

    vector<int> ab = convolve(va, vb);
    vector<int> abc = convolve(ab, vc);

    long long ans = abc[E];
    ans = ans * fact[N] % MOD;
    ans = ans * fact[E] % MOD;
    ans = ans * pow26[L] % MOD;
    ans = ans * pow26[U] % MOD;
    ans = ans * pow10[D] % MOD;
    return int(ans);
  }
};

static struct Init {
  Init() {
    fact[0] = 1;
    for (int i = 1; i < MAXN; ++i)
      fact[i] = mulmod(fact[i - 1], i);
    invFact[MAXN - 1] = modpow(fact[MAXN - 1], MOD - 2);
    for (int i = MAXN - 2; i >= 0; --i)
      invFact[i] = mulmod(invFact[i + 1], i + 1);
    pow26[0] = pow10[0] = 1;
    for (int i = 1; i < MAXN; ++i) {
      pow26[i] = mulmod(pow26[i - 1], 26);
      pow10[i] = mulmod(pow10[i - 1], 10);
    }
  }
} init;

#ifdef LOCAL_TEST
int main() {
  struct Case {
    int N, L, U, D, want;
  };
  Case cases[] = {{2, 0, 0, 2, 100},
                  {3, 1, 1, 1, 40560},
                  {4, 1, 1, 1, 5029440},
                  {10, 1, 3, 3, 818019214},
                  {5, 2, 2, 2, 0},
                  {1, 0, 0, 0, 62},
                  {200000, 0, 0, 0, 738150635},
                  {200000, 100000, 1, 1, 470373717}};
  Passwords p;
  for (const Case& c : cases) {
    int got = p.countValid(c.N, c.L, c.U, c.D);
    printf("%d %d %d %d -> %d %s\n", c.N, c.L, c.U, c.D, got, got == c.want ? "OK" : "FAIL");
  }
  return 0;
}
#endif

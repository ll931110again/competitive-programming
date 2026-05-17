// Facebook Hacker Cup 2011 R2 - Problem B: Scott's New Trick
//
// Count pairs (i,j) with (a_i * b_j) mod P < L.
// Let c_a[x], c_b[y] be frequencies mod P. Zeros: product 0.
// For nonzero values, write x = g^k (g primitive root), and y = g^l.
// Then x*y = g^{k+l mod (P-1)}. Cyclic convolution of exponent histograms gives counts per product
// residue. Sum conv[m] for all m with (g^m mod P) < L.

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

using int64 = long long;
using i128 = __int128_t;

static int64 mod_pow(int64 a, int64 e, int64 mod) {
  int64 r = 1 % mod;
  a %= mod;
  while (e > 0) {
    if (e & 1)
      r = (i128)r * a % mod;
    a = (i128)a * a % mod;
    e >>= 1;
  }
  return r;
}

static int64 mod_inv(int64 a, int64 mod) {
  int64 t = 0, nt = 1, r = mod, nr = a % mod;
  while (nr != 0) {
    int64 q = r / nr;
    int64 tmp = t - q * nt;
    t = nt;
    nt = tmp;
    tmp = r - q * nr;
    r = nr;
    nr = tmp;
  }
  if (t < 0)
    t += mod;
  return t;
}

static int primitive_root(int p) {
  if (p == 2)
    return 1;
  int phi = p - 1;
  vector<int> fac;
  int x = phi;
  for (int i = 2; (int64)i * i <= x; ++i) {
    if (x % i == 0) {
      fac.push_back(i);
      while (x % i == 0)
        x /= i;
    }
  }
  if (x > 1)
    fac.push_back(x);
  for (int g = 2; g < p; ++g) {
    bool ok = true;
    for (int q : fac) {
      if (mod_pow(g, phi / q, p) == 1) {
        ok = false;
        break;
      }
    }
    if (ok)
      return g;
  }
  return -1;
}

// ---------- NTT (power-of-2 length) ----------
static void ntt(vector<int64>& a, bool invert, int64 mod, int64 root) {
  int n = (int)a.size();
  for (int i = 1, j = 0; i < n; ++i) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1)
      j ^= bit;
    j ^= bit;
    if (i < j)
      swap(a[i], a[j]);
  }
  for (int len = 2; len <= n; len <<= 1) {
    int64 wlen = mod_pow(root, (mod - 1) / len, mod);
    if (invert)
      wlen = mod_inv(wlen, mod);
    for (int i = 0; i < n; i += len) {
      int64 w = 1;
      for (int j = 0; j < len / 2; ++j) {
        int64 u = a[i + j];
        int64 v = (i128)a[i + j + len / 2] * w % mod;
        int64 x = u + v;
        if (x >= mod)
          x -= mod;
        a[i + j] = x;
        x = u - v;
        if (x < 0)
          x += mod;
        a[i + j + len / 2] = x;
        w = (i128)w * wlen % mod;
      }
    }
  }
  if (invert) {
    int64 inv_n = mod_inv(n, mod);
    for (int64& x : a)
      x = (i128)x * inv_n % mod;
  }
}

static const int64 MOD1 = 998244353;
static const int64 MOD2 = 1004535809;
static const int64 ROOT1 = 3;
static const int64 ROOT2 = 3;

static vector<int64> convolution_ll(const vector<int64>& aa, const vector<int64>& bb) {
  int need = (int)aa.size() + (int)bb.size() - 1;
  int n = 1;
  while (n < need)
    n <<= 1;
  vector<int64> fa(n), fb(n);
  for (int i = 0; i < (int)aa.size(); ++i)
    fa[i] = aa[i] % MOD1;
  for (int i = 0; i < (int)bb.size(); ++i)
    fb[i] = bb[i] % MOD1;
  ntt(fa, false, MOD1, ROOT1);
  ntt(fb, false, MOD1, ROOT1);
  for (int i = 0; i < n; ++i)
    fa[i] = (i128)fa[i] * fb[i] % MOD1;
  ntt(fa, true, MOD1, ROOT1);

  vector<int64> ga(n), gb(n);
  for (int i = 0; i < (int)aa.size(); ++i)
    ga[i] = aa[i] % MOD2;
  for (int i = 0; i < (int)bb.size(); ++i)
    gb[i] = bb[i] % MOD2;
  ntt(ga, false, MOD2, ROOT2);
  ntt(gb, false, MOD2, ROOT2);
  for (int i = 0; i < n; ++i)
    ga[i] = (i128)ga[i] * gb[i] % MOD2;
  ntt(ga, true, MOD2, ROOT2);

  vector<int64> res(need);
  int64 inv_mod1_mod2 = mod_inv(MOD1 % MOD2, MOD2);
  i128 M = (i128)MOD1 * MOD2;
  for (int i = 0; i < need; ++i) {
    int64 a1 = fa[i];
    int64 a2 = ga[i];
    int64 t = (i128)((a2 - a1) % MOD2 + MOD2) * inv_mod1_mod2 % MOD2;
    i128 x = (i128)a1 + (i128)MOD1 * t;
    if (x >= M)
      x %= M;
    res[i] = (int64)x;
  }
  return res;
}

// Cyclic convolution of length L: C[k] = sum_{i+j ≡ k (mod L)} A[i]B[j]
static vector<int64> cyclic_convolution(const vector<int64>& A, const vector<int64>& B, int L) {
  int pad = 1;
  while (pad < 2 * L)
    pad <<= 1;
  vector<int64> aa(pad), bb(pad);
  for (int i = 0; i < L; ++i) {
    aa[i] = A[i];
    bb[i] = B[i];
  }
  vector<int64> lin = convolution_ll(aa, bb);
  vector<int64> C(L);
  for (int k = 0; k < L; ++k) {
    int64 v = lin[k];
    if (k + L < (int)lin.size())
      v += lin[k + L];
    C[k] = v;
  }
  return C;
}

static void add_freq_mod(int64 N, int64 P, int64 A1, int64 A2, int64 A3, int64 A4, int64 A5,
                         vector<int64>& cnt) {
  cnt.assign((size_t)P, 0);
  int64 x = A1 % P, y = A2 % P;
  cnt[(size_t)x]++;
  cnt[(size_t)y]++;
  for (int64 i = 3; i <= N; ++i) {
    int64 z = ((i128)(x)*A3 + (i128)(y)*A4 + A5) % P;
    cnt[(size_t)z]++;
    x = y;
    y = z;
  }
}

static int64 solve_one(int64 P, int64 L, int64 N, int64 A1, int64 A2, int64 A3, int64 A4, int64 A5,
                       int64 M, int64 B1, int64 B2, int64 B3, int64 B4, int64 B5) {
  vector<int64> ca, cb;
  add_freq_mod(N, P, A1, A2, A3, A4, A5, ca);
  add_freq_mod(M, P, B1, B2, B3, B4, B5, cb);

  int64 zpairs = ca[0] * M + cb[0] * N - ca[0] * cb[0];
  int64 ans = 0;
  if (L > 0)
    ans += zpairs;

  int64 Llen = P - 1;
  if (Llen <= 0)
    return ans;

  int g = primitive_root((int)P);
  vector<int64> ea((size_t)Llen), eb((size_t)Llen);
  for (int64 k = 0; k < Llen; ++k) {
    int64 v = mod_pow(g, k, P);
    ea[(size_t)k] = ca[(size_t)v];
    eb[(size_t)k] = cb[(size_t)v];
  }

  vector<int64> conv = cyclic_convolution(ea, eb, (int)Llen);

  for (int64 m = 0; m < Llen; ++m) {
    int64 val = mod_pow(g, m, P);
    if (val < L)
      ans += conv[(size_t)m];
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  if (!(cin >> T))
    return 0;
  for (int tc = 1; tc <= T; ++tc) {
    int64 P, L;
    cin >> P >> L;
    int64 N, A1, A2, A3, A4, A5;
    cin >> N >> A1 >> A2 >> A3 >> A4 >> A5;
    int64 M, B1, B2, B3, B4, B5;
    cin >> M >> B1 >> B2 >> B3 >> B4 >> B5;
    int64 ans = solve_one(P, L, N, A1, A2, A3, A4, A5, M, B1, B2, B3, B4, B5);
    cout << "Case #" << tc << ": " << ans << "\n";
  }
  return 0;
}

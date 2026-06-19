// Facebook Hacker Cup 2011 R2 - Problem B: Scott's New Trick
//
// Count pairs (i,j) with (a_i * b_j) mod P < L.
// Let c_a[x], c_b[y] be frequencies mod P. Zeros: product 0.
// For nonzero values, write x = g^k (g primitive root), and y = g^l.
// Then x*y = g^{k+l mod (P-1)}. Cyclic convolution of exponent histograms gives counts per product
// residue. Sum conv[m] for all m with (g^m mod P) < L.

#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;

namespace {

i64 mod_pow(i64 a, i64 e, i64 mod) {
  i64 r = 1 % mod;
  a %= mod;
  while (e > 0) {
    if (e & 1)
      r = (i128)r * a % mod;
    a = (i128)a * a % mod;
    e >>= 1;
  }
  return r;
}

i64 mod_inv(i64 a, i64 mod) {
  i64 t = 0, nt = 1, r = mod, nr = a % mod;
  while (nr != 0) {
    i64 q = r / nr;
    i64 tmp = t - q * nt;
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

int primitive_root(int p) {
  if (p == 2)
    return 1;
  int phi = p - 1;
  vector<int> fac;
  int x = phi;
  for (int i = 2; (i64)i * i <= x; ++i) {
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
void ntt(vector<i64>& a, bool invert, i64 mod, i64 root) {
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
    i64 wlen = mod_pow(root, (mod - 1) / len, mod);
    if (invert)
      wlen = mod_inv(wlen, mod);
    for (int i = 0; i < n; i += len) {
      i64 w = 1;
      for (int j = 0; j < len / 2; ++j) {
        i64 u = a[i + j];
        i64 v = (i128)a[i + j + len / 2] * w % mod;
        i64 x = u + v;
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
    i64 inv_n = mod_inv(n, mod);
    for (i64& x : a)
      x = (i128)x * inv_n % mod;
  }
}

const i64 MOD1 = 998244353;
const i64 MOD2 = 1004535809;
const i64 ROOT1 = 3;
const i64 ROOT2 = 3;

vector<i64> convolution_ll(const vector<i64>& aa, const vector<i64>& bb) {
  int need = (int)aa.size() + (int)bb.size() - 1;
  int n = 1;
  while (n < need)
    n <<= 1;
  vector<i64> fa(n), fb(n);
  for (int i = 0; i < (int)aa.size(); ++i)
    fa[i] = aa[i] % MOD1;
  for (int i = 0; i < (int)bb.size(); ++i)
    fb[i] = bb[i] % MOD1;
  ntt(fa, false, MOD1, ROOT1);
  ntt(fb, false, MOD1, ROOT1);
  for (int i = 0; i < n; ++i)
    fa[i] = (i128)fa[i] * fb[i] % MOD1;
  ntt(fa, true, MOD1, ROOT1);

  vector<i64> ga(n), gb(n);
  for (int i = 0; i < (int)aa.size(); ++i)
    ga[i] = aa[i] % MOD2;
  for (int i = 0; i < (int)bb.size(); ++i)
    gb[i] = bb[i] % MOD2;
  ntt(ga, false, MOD2, ROOT2);
  ntt(gb, false, MOD2, ROOT2);
  for (int i = 0; i < n; ++i)
    ga[i] = (i128)ga[i] * gb[i] % MOD2;
  ntt(ga, true, MOD2, ROOT2);

  vector<i64> res(need);
  i64 inv_mod1_mod2 = mod_inv(MOD1 % MOD2, MOD2);
  i128 M = (i128)MOD1 * MOD2;
  for (int i = 0; i < need; ++i) {
    i64 a1 = fa[i];
    i64 a2 = ga[i];
    i64 t = (i128)((a2 - a1) % MOD2 + MOD2) * inv_mod1_mod2 % MOD2;
    i128 x = (i128)a1 + (i128)MOD1 * t;
    if (x >= M)
      x %= M;
    res[i] = (i64)x;
  }
  return res;
}

// Cyclic convolution of length L: C[k] = sum_{i+j ≡ k (mod L)} A[i]B[j]
vector<i64> cyclic_convolution(const vector<i64>& A, const vector<i64>& B, int L) {
  int pad = 1;
  while (pad < 2 * L)
    pad <<= 1;
  vector<i64> aa(pad), bb(pad);
  for (int i = 0; i < L; ++i) {
    aa[i] = A[i];
    bb[i] = B[i];
  }
  vector<i64> lin = convolution_ll(aa, bb);
  vector<i64> C(L);
  for (int k = 0; k < L; ++k) {
    i64 v = lin[k];
    if (k + L < (int)lin.size())
      v += lin[k + L];
    C[k] = v;
  }
  return C;
}

void add_freq_mod(i64 N, i64 P, i64 A1, i64 A2, i64 A3, i64 A4, i64 A5, vector<i64>& cnt) {
  cnt.assign((size_t)P, 0);
  i64 x = A1 % P, y = A2 % P;
  cnt[(size_t)x]++;
  cnt[(size_t)y]++;
  for (i64 i = 3; i <= N; ++i) {
    i64 z = ((i128)(x)*A3 + (i128)(y)*A4 + A5) % P;
    cnt[(size_t)z]++;
    x = y;
    y = z;
  }
}

i64 solve_one(i64 P, i64 L, i64 N, i64 A1, i64 A2, i64 A3, i64 A4, i64 A5, i64 M, i64 B1, i64 B2,
              i64 B3, i64 B4, i64 B5) {
  vector<i64> ca, cb;
  add_freq_mod(N, P, A1, A2, A3, A4, A5, ca);
  add_freq_mod(M, P, B1, B2, B3, B4, B5, cb);

  i64 zpairs = ca[0] * M + cb[0] * N - ca[0] * cb[0];
  i64 ans = 0;
  if (L > 0)
    ans += zpairs;

  i64 Llen = P - 1;
  if (Llen <= 0)
    return ans;

  int g = primitive_root((int)P);
  vector<i64> ea((size_t)Llen), eb((size_t)Llen);
  for (i64 k = 0; k < Llen; ++k) {
    i64 v = mod_pow(g, k, P);
    ea[(size_t)k] = ca[(size_t)v];
    eb[(size_t)k] = cb[(size_t)v];
  }

  vector<i64> conv = cyclic_convolution(ea, eb, (int)Llen);

  for (i64 m = 0; m < Llen; ++m) {
    i64 val = mod_pow(g, m, P);
    if (val < L)
      ans += conv[(size_t)m];
  }
  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  if (!(cin >> T))
    return 0;
  for (int tc = 1; tc <= T; ++tc) {
    i64 P, L;
    cin >> P >> L;
    i64 N, A1, A2, A3, A4, A5;
    cin >> N >> A1 >> A2 >> A3 >> A4 >> A5;
    i64 M, B1, B2, B3, B4, B5;
    cin >> M >> B1 >> B2 >> B3 >> B4 >> B5;
    i64 ans = solve_one(P, L, N, A1, A2, A3, A4, A5, M, B1, B2, B3, B4, B5);
    cout << "Case #" << tc << ": " << ans << "\n";
  }
  return 0;
}

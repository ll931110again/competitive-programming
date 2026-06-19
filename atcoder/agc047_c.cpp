// AtCoder Grand Contest 047 - C: Product Modulo
// https://atcoder.jp/contests/agc047/tasks/agc047_c
//
// Sum over i<j of (A_i * A_j) mod P (integer sum, not mod P).
// Pairs with a zero contribute 0. For nonzero entries, map values by primitive root g:
// x = g^k mod P → multiplication becomes addition of exponents mod (P-1).
// Cyclic convolution of histogram H[k] = count at g^k gives ordered pair counts per
// product residue. Subtract diagonal (p,p), multiply by g^m mod P, divide by 2 for i<j.
//
// Convolution coefficients are ≤ N² ≤ 4·10¹⁰, so one standard ~10⁹ NTT modulus would
// overflow; either CRT (two primes) or a single NTT prime q with q > N² and enough
// powers of two in (q−1) for the transform length. Padding is 2¹⁹ and linear conv
// needs length 2²¹, so q ≡ 1 (mod 2²¹). Example: q = 19086·2²¹ + 1 > N²_max.

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

// q = 19086·2^21 + 1 > 200000² and 2^21 | (q−1); primitive root 5 (q−1 = 2^21·3·3181).
const i64 MOD = 40026243073LL;
const i64 ROOT = 5;

vector<i64> convolution_ll(const vector<i64>& aa, const vector<i64>& bb) {
  int need = (int)aa.size() + (int)bb.size() - 1;
  int n = 1;
  while (n < need)
    n <<= 1;
  vector<i64> fa(n), fb(n);
  for (int i = 0; i < (int)aa.size(); ++i)
    fa[i] = aa[i] % MOD;
  for (int i = 0; i < (int)bb.size(); ++i)
    fb[i] = bb[i] % MOD;
  ntt(fa, false, MOD, ROOT);
  ntt(fb, false, MOD, ROOT);
  for (int i = 0; i < n; ++i)
    fa[i] = (i128)fa[i] * fb[i] % MOD;
  ntt(fa, true, MOD, ROOT);

  vector<i64> res(need);
  copy(fa.begin(), fa.begin() + need, res.begin());
  return res;
}

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

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  constexpr int P = 200003;
  int N;
  if (!(cin >> N))
    return 0;

  vector<i64> cnt(P, 0);
  for (int i = 0; i < N; ++i) {
    int x;
    cin >> x;
    cnt[x]++;
  }

  const int L = P - 1;
  int g = primitive_root(P);

  // H[k] = how many A_i equal g^k mod P (nonzero values only)
  vector<i64> H(L, 0);
  for (int k = 0; k < L; ++k) {
    i64 v = mod_pow(g, k, P);
    H[k] = cnt[(size_t)v];
  }

  vector<i64> R = cyclic_convolution(H, H, L);

  vector<i64> diag(L, 0);
  for (int e = 0; e < L; ++e) {
    int m = (2 * e) % L;
    diag[m] += H[e];
  }

  i128 sum_ordered = 0;
  for (int m = 0; m < L; ++m) {
    i64 val = mod_pow(g, m, P);
    i128 pairs = (i128)R[m] - (i128)diag[m];
    sum_ordered += (i128)val * pairs;
  }

  i128 ans = sum_ordered / 2;
  cout << (i64)ans << '\n';
  return 0;
}

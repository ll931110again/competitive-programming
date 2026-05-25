#include <bits/stdc++.h>
using namespace std;

const int MOD = 100003;
const int MAXN = 100;

long long fac[MOD], inv[MOD];

struct Matrix {
  int n;
  long long a[MAXN][MAXN];

  Matrix() : n(0) {
    memset(a, 0, sizeof(a));
  }

  explicit Matrix(int n_) : n(n_) {
    memset(a, 0, sizeof(a));
  }

  static Matrix identity(int n_) {
    Matrix m(n_);
    for (int i = 0; i < n_; i++)
      m.a[i][i] = 1;
    return m;
  }

  Matrix operator*(const Matrix& rhs) const {
    Matrix res(n);
    for (int k = 0; k < n; k++)
      for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
          res.a[i][j] = (res.a[i][j] + a[i][k] * rhs.a[k][j]) % MOD;
    return res;
  }

  Matrix pow(long long exp) const {
    Matrix base = *this;
    Matrix res = identity(n);
    while (exp > 0) {
      if (exp & 1)
        res = res * base;
      base = base * base;
      exp >>= 1;
    }
    return res;
  }
};

struct ShrooksOnTheBoard {
  int count(int K, int H, int W) {
    K++;
    fac[0] = inv[0] = 1;
    for (int i = 1; i < MOD; i++) {
      fac[i] = (fac[i - 1] * 1LL * i) % MOD;
      inv[i] = power(fac[i], MOD - 2);
    }

    long long one = (K <= MAXN) ? matrix_method(K, W) : comb_method(K, W);
    return (int)(power(one, H) + MOD - 1) % MOD;
  }

  long long power(long long x, long long p) {
    if (!p)
      return 1;
    long long q = power(x, p / 2);
    q = (q * q) % MOD;
    if (p & 1)
      q = (q * x) % MOD;
    return q;
  }

  long long C(int n, int k) {
    if (n < k)
      return 0;
    long long ret = (inv[k] * inv[n - k]) % MOD;
    return (ret * fac[n]) % MOD;
  }

  long long bigC(int n, int k) {
    long long ret = 1;
    while (n || k) {
      ret = (ret * C(n % MOD, k % MOD)) % MOD;
      n /= MOD;
      k /= MOD;
    }
    return ret;
  }

  long long comb_method(int K, int W) {
    long long ret = 1;
    for (int i = 1; K * (i - 1) <= W; i++)
      ret = (ret + bigC(W - K * (i - 1) + i - 1, i)) % MOD;
    return ret;
  }

  long long matrix_method(int K, int W) {
    if (K >= W)
      return (W + 1) % MOD;

    Matrix base(K);
    for (int i = 1; i < K; i++)
      base.a[i][i - 1] = 1;
    base.a[0][K - 1] = base.a[K - 1][K - 1] = 1;

    Matrix pw = base.pow(W - K);
    long long ret = 0;
    for (int i = 0; i < K; i++)
      ret = (ret + 1LL * (i + 2) * pw.a[i][K - 1]) % MOD;
    return ret;
  }
};

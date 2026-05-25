// TCO 2010 Open Wildcard Div I 1000 - ShrooksOnTheBoard
// https://archive.topcoder.com/ProblemDetail/pm/2735
//
// Problem (short)
//   Place a positive number of K-shrooks on an H×W board. A K-shrook moves at most
//   K squares left or right on its row. Two shrooks attack if one can reach the
//   other in one move. At most one shrook per row. Answer mod 100003.
//
// High-level solution
//   Rows are independent (attacks are horizontal only, and ≤1 shrook per row).
//   Let F(W) = number of valid placements on one row of width W (empty row allowed).
//   Full board: F(W)^H − 1  (subtract the all-empty placement).
//
//   Internally we use K' = K + 1 (parameter K is the move length; the DP uses
//   states 0 … K'−1 for a K'-periodic “cooldown” along the row).
//
//   Two ways to compute F(W):
//   • K' ≤ 100: matrix exponentiation. Scanning a row is a linear recurrence on a
//     K'-vector; each column is one step of a fixed K'×K' transition matrix.
//     After W columns, read off weighted contributions from each start state.
//   • K' > 100: direct combinatorics. Placing i ≥ 1 shrooks with required gaps is
//     a stars-and-bars count; sum binomials via Lucas theorem (MOD is prime).
//
//   Edge case K' ≥ W: at most one shrook fits in a row anyway → F(W) = W + 1.

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
    K++; // work with K' = parameter K + 1 (see header)
    fac[0] = inv[0] = 1;
    for (int i = 1; i < MOD; i++) {
      fac[i] = (fac[i - 1] * 1LL * i) % MOD;
      inv[i] = power(fac[i], MOD - 2);
    }

    long long one = (K <= MAXN) ? matrix_method(K, W) : comb_method(K, W);
    // H independent rows; exclude the placement with zero shrooks overall.
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

  // Binomial mod prime when n, k may exceed MOD (Lucas theorem).
  long long bigC(int n, int k) {
    long long ret = 1;
    while (n || k) {
      ret = (ret * C(n % MOD, k % MOD)) % MOD;
      n /= MOD;
      k /= MOD;
    }
    return ret;
  }

  // F(W) for large K': empty row (1) plus i ≥ 1 shrooks with enough horizontal gap.
  long long comb_method(int K, int W) {
    long long ret = 1;
    for (int i = 1; K * (i - 1) <= W; i++)
      ret = (ret + bigC(W - K * (i - 1) + i - 1, i)) % MOD;
    return ret;
  }

  // F(W) for small K' via K'×K' transition matrix (one column = one multiply).
  long long matrix_method(int K, int W) {
    if (K >= W)
      return (W + 1) % MOD; // only 0 or 1 shrook per row

    Matrix base(K);
    // Shift phase along the row (column step without placing).
    for (int i = 1; i < K; i++)
      base.a[i][i - 1] = 1;
    // Place a shrook: reset phase, or stay in the “blocked” phase K'−1.
    base.a[0][K - 1] = base.a[K - 1][K - 1] = 1;

    Matrix pw = base.pow(W - K);
    // Weight (i + 2) encodes empty / placement accounting per start state i.
    long long ret = 0;
    for (int i = 0; i < K; i++)
      ret = (ret + 1LL * (i + 2) * pw.a[i][K - 1]) % MOD;
    return ret;
  }
};

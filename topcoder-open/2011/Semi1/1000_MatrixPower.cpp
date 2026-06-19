#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
const i64 MOD = 1000000007LL;

struct Mat2 {
  i64 a[2][2];
  Mat2() {
    memset(a, 0, sizeof(a));
  }
};

Mat2 mul(const Mat2& x, const Mat2& y) {
  Mat2 r;
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      for (int k = 0; k < 2; k++)
        r.a[i][j] = (r.a[i][j] + x.a[i][k] * y.a[k][j]) % MOD;
  return r;
}

Mat2 mpow(Mat2 base, i64 exp) {
  Mat2 res;
  res.a[0][0] = res.a[1][1] = 1;
  while (exp > 0) {
    if (exp & 1)
      res = mul(res, base);
    base = mul(base, base);
    exp >>= 1;
  }
  return res;
}

class MatrixPower {
public:
  vector<int> getElements(int d, int q, int n, int k, vector<int> rows, vector<int> columns) {
    i64 nn = n;
    i64 S1 = nn * (nn - 1) / 2 % MOD;
    i64 S2 = (nn - 1) * nn % MOD;
    S2 = S2 * (2 * nn - 1) % MOD;
    S2 = S2 * 500000004LL % MOD;

    Mat2 M;
    M.a[0][0] = d * S1 % MOD;
    M.a[0][1] = q * nn % MOD;
    M.a[1][0] = d * S2 % MOD;
    M.a[1][1] = q * S1 % MOD;

    Mat2 P = mpow(M, max(0, k - 1));

    vector<int> ret(rows.size());
    for (int t = 0; t < (int)rows.size(); t++) {
      i64 i = rows[t];
      i64 j = columns[t];
      i64 pi0 = d * i % MOD;
      i64 pi1 = q;
      i64 qj0 = 1;
      i64 qj1 = j % MOD;
      i64 v0 = (pi0 * P.a[0][0] + pi1 * P.a[1][0]) % MOD;
      i64 v1 = (pi0 * P.a[0][1] + pi1 * P.a[1][1]) % MOD;
      ret[t] = (int)((v0 * qj0 + v1 * qj1) % MOD);
    }
    return ret;
  }
};

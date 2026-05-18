#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD = 1000000007LL;

struct Mat2 {
  ll a[2][2];
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

Mat2 mpow(Mat2 base, ll exp) {
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
    ll nn = n;
    ll S1 = nn * (nn - 1) / 2 % MOD;
    ll S2 = (nn - 1) * nn % MOD;
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
      ll i = rows[t];
      ll j = columns[t];
      ll pi0 = d * i % MOD;
      ll pi1 = q;
      ll qj0 = 1;
      ll qj1 = j % MOD;
      ll v0 = (pi0 * P.a[0][0] + pi1 * P.a[1][0]) % MOD;
      ll v1 = (pi0 * P.a[0][1] + pi1 * P.a[1][1]) % MOD;
      ret[t] = (int)((v0 * qj0 + v1 * qj1) % MOD);
    }
    return ret;
  }
};

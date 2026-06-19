#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
struct BagsOfNumbers {
  i64 wonByAlice(int n, vector<i64> a, vector<i64> b, vector<i64> c, vector<i64> d) {
    i64 nimber[55];
    for (int i = 0; i < n; i++) {
      nimber[i] = getNimber(a[i], b[i], c[i], d[i]);
    }

    int mat[55][64];

    for (int i = 0; i < n; i++) {
      for (int bit = 0; bit < 64; bit++) {
        mat[i][bit] = (nimber[i] & (1LL << bit)) > 0;
      }
    }

    int rank = 0;
    for (int bit = 0; bit < 64; bit++) {
      for (int i = rank; i < n; i++)
        if (mat[i][bit] > 0) {
          for (int k = 0; k < 64; k++) {
            swap(mat[rank][k], mat[i][k]);
          }
          for (int j = rank + 1; j < n; j++)
            if (mat[j][bit]) {
              for (int k = 0; k < 64; k++) {
                mat[j][k] ^= mat[rank][k];
              }
            }
          rank++;
        }
    }

    return (1LL << n) - (1LL << (n - rank));
  }

  i64 getNimber(i64 n1, i64 n2, i64 n3, i64 n4) {
    i64 res = n4 + n2 / 2 + min(n1, n3);
    i64 extra = max(n1, n3) - min(n1, n3);

    if (n2 % 2 == 0) {
      res += extra / 4;
    } else {
      res += (extra + 2) / 4;
    }

    return res;
  }
};

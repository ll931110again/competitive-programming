#include <bits/stdc++.h>
using namespace std;

class RowOfColors {
public:
  int countWays(int w, int h, int K) {
    const int MOD = 1000000007;
    vector<vector<int>> dp(h + 2, vector<int>(K + 1)), tdp(h + 2, vector<int>(K + 1));

    for (int p = w; p >= 0; p--) {
      for (int stack = 0; stack <= h; stack++) {
        for (int k = min(K, w - p); k >= 0; k--) {
          int res = 0;
          if (p == w) {
            res = (stack == 0 && k == 0) ? 1 : 0;
          } else if (stack > h) {
            res = 0;
          } else {
            if (k > 0 && stack + 1 <= h) {
              res += tdp[stack + 1][k - 1];
              if (res >= MOD)
                res -= MOD;
              res += tdp[stack][k - 1];
              if (res >= MOD)
                res -= MOD;
            }
            if (stack > 0 && k <= w - p + 1) {
              res += tdp[stack][k];
              if (res >= MOD)
                res -= MOD;
              res += tdp[stack - 1][k];
              if (res >= MOD)
                res -= MOD;
            }
          }
          dp[stack][k] = res;
        }
      }
      swap(dp, tdp);
    }

    long long ans = tdp[0][K];
    for (int i = 2; i <= K; i++)
      ans = ans * i % MOD;
    return (int)ans;
  }
};

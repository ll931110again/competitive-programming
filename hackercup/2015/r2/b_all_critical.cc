#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <numeric>
#include <queue>
#include <vector>
#define maxn 22
#define maxl 10000
using namespace std;

int T;
double p;

int N = 20;
double dp[maxl][maxn];
double pin[maxn], pout[maxn];
long long binom[maxn][maxn];

double solve(double p) {
  pin[0] = 1;
  pout[0] = 1;
  for (int i = 1; i <= N; i++) {
    pin[i] = pin[i - 1] * p;
    pout[i] = pout[i - 1] * (1 - p);
  }

  memset(dp, 0, sizeof dp);
  dp[0][0] = 1;
  for (int i = 1; i < maxl; i++) {
    for (int j = 0; j <= N; j++) {
      for (int k = 0; k <= j; k++) {
        dp[i][j] += dp[i - 1][k] * pin[j - k] * pout[N - j] * binom[N - k][j - k];
      }
    }
  }

  double ans = 0;
  for (int i = 1; i < maxl; i++) {
    ans += i * (dp[i][N] - dp[i - 1][N]);
  }

  return ans;
}

int main() {
    binom[0][0] = 1;
    for (int i = 1; i <= N; i++) {
      for (int j = 0; j <= i; j++) {
        binom[i][j] = binom[i - 1][j];
        if (j) {
          binom[i][j] += binom[i - 1][j - 1];
        }
      }
    }
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
      cin >> p;
      printf("Case #%d: %.9lf\n", it, solve(p));
    }

    return 0;
}
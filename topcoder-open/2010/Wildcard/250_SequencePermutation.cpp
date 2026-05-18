#include <bits/stdc++.h>
using namespace std;

#define mod 1000000009

int sum[2002], next[2002];

struct SequencePermutation {
  int determineConfigurations(int N, int M) {
    for (int i = 0; i <= M; i++)
      sum[i] = 1;
    for (int i = 2; i <= N; i++) {
      for (int j = 0; j <= M; j++)
        next[j] = getSum(j - i + 1, j);
      sum[0] = next[0];
      for (int j = 1; j <= M; j++)
        sum[j] = (sum[j - 1] + next[j]) % mod;
    }

    int ret = 0;
    for (int i = M; i >= 0; i -= 2)
      ret = (ret + next[i]) % mod;
    return ret;
  }

  int getSum(int a, int b) {
    if (a > b)
      swap(a, b);
    if (b < 0)
      return 0;
    int rb = sum[b], ra = (a > 0) ? sum[a - 1] : 0;
    return (rb + mod - ra) % mod;
  }
};

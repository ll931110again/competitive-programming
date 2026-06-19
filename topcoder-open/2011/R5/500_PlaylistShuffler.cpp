#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class PlaylistShuffler {
  i64 gcdll(i64 a, i64 b) {
    while (b) {
      i64 t = a % b;
      a = b;
      b = t;
    }
    return a;
  }

public:
  i64 bestShuffle(int N, int X, int Y, i64 K1, i64 K2) {
    vector<i64> prob(K2 - K1 + 1);
    for (i64 K = K1; K <= K2; K++) {
      long double p = 0.0L;
      if (X == Y) {
        for (int L = 1; L <= N; L++)
          if (K % L == 0)
            p += 1.0L / N;
      } else {
        for (int L = 2; L <= N; L++) {
          if (K % L == 0)
            continue;
          long double add = 1.0L / N;
          add *= (long double)(L - 1) / (long double)(N - 1);
          p += add;
        }
      }
      prob[K - K1] = (i64)(p * 1000000000.0L);
    }

    i64 bestK = K1;
    i64 bestP = prob[0];
    for (i64 K = K1 + 1; K <= K2; K++) {
      i64 val = prob[K - K1];
      if (val > bestP) {
        bestP = val;
        bestK = K;
      }
    }
    return bestK;
  }
};

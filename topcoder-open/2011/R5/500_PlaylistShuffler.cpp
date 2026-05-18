#include <bits/stdc++.h>
using namespace std;

class PlaylistShuffler {
  long long gcdll(long long a, long long b) {
    while (b) {
      long long t = a % b;
      a = b;
      b = t;
    }
    return a;
  }

public:
  long long bestShuffle(int N, int X, int Y, long long K1, long long K2) {
    if (X == Y) {
      return K1;
    }

    vector<long long> prob(K2 - K1 + 1);
    for (long long K = K1; K <= K2; K++) {
      long double p = 0.0L;
      for (int L = 1; L <= N; L++) {
        if (L == 1)
          continue;
        long long g = gcdll((long long)L, K);
        long long cycle = L / g;
        if (K % cycle != 0)
          continue;
        long double add = 1.0L / N;
        if (L > 1)
          add *= (long double)(L - 1) / (long double)(N - 1);
        p += add;
      }
      prob[K - K1] = (long long)(p * 1000000000.0L);
    }

    long long bestK = K1;
    long long bestP = prob[0];
    for (long long K = K1 + 1; K <= K2; K++) {
      long long val = prob[K - K1];
      if (val > bestP) {
        bestP = val;
        bestK = K;
      }
    }
    return bestK;
  }
};

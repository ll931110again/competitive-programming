#include <bits/stdc++.h>
using namespace std;

class AllButOneDivisor {
  long long gcd(long long a, long long b) {
    while (b) {
      long long t = a % b;
      a = b;
      b = t;
    }
    return a;
  }
  long long lcm(long long a, long long b) {
    return a / gcd(a, b) * b;
  }

public:
  int getMinimum(vector<int> divisors) {
    int k = divisors.size();
    int best = -1;
    for (int e = 0; e < k; e++) {
      long long L = 1;
      for (int i = 0; i < k; i++)
        if (i != e)
          L = lcm(L, divisors[i]);
      if (L % divisors[e] != 0) {
        if (best < 0 || L < best)
          best = (int)L;
      }
    }
    return best;
  }
};

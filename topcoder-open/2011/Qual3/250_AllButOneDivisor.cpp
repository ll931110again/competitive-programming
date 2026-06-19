#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class AllButOneDivisor {
  i64 gcd(i64 a, i64 b) {
    while (b) {
      i64 t = a % b;
      a = b;
      b = t;
    }
    return a;
  }
  i64 lcm(i64 a, i64 b) {
    return a / gcd(a, b) * b;
  }

public:
  int getMinimum(vector<int> divisors) {
    int k = divisors.size();
    int best = -1;
    for (int e = 0; e < k; e++) {
      i64 L = 1;
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

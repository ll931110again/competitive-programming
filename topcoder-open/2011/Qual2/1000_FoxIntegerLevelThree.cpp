#include <bits/stdc++.h>
using namespace std;

class FoxIntegerLevelThree {
  int gcd(int a, int b) {
    return b ? gcd(b, a % b) : a;
  }

public:
  long count(long min, long max) {
    int m = 1;
    for (int i = 2; i <= 9; i++)
      m = m / gcd(m, i) * i;
    m *= 9;
    vector<int> ok(m);
    for (int i = 0; i < m; i++) {
      int dr = i % 9;
      if (dr == 0)
        dr = 9;
      long long k = 1LL * i * dr;
      if (dr == 9)
        k = 1LL * i * 9;
      if (k < m)
        ok[(int)k] = 1;
    }
    long res = 0;
    for (int i = 0; i < m; i++)
      if (ok[i]) {
        res += (max + (m - i)) / m;
        res -= ((min - 1) + (m - i)) / m;
      }
    return res;
  }
};

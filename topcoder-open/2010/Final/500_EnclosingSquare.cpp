#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
struct EnclosingSquare {
  i64 getMinimum(i64 n) {
    if (n < 2)
      return n + 1;

    n--;
    i64 ret = 1LL << 60;
    for (int d = 1; d <= n / d; d++)
      if (n % d == 0) {
        ret = min(ret, candidate(n, d));
        ret = min(ret, candidate(n, n / d));
      }

    return (ret >= (1LL << 60)) ? 0 : ret;
  }

  i64 candidate(i64 n, i64 d) {
    i64 rem = n / d + 2;
    if (rem % d)
      return 1LL << 60;
    rem /= d;
    for (int x = 0; 1LL * x * x <= rem; x++) {
      int y = (int)trunc(sqrt(rem - 1LL * x * x));
      if (y < x)
        return (1LL << 60);
      if ((1LL * x * x + 1LL * y * y == rem) && __gcd(x, y) == 1)
        return (1LL * d * d * rem);
    }
    return (1LL << 60);
  }
};

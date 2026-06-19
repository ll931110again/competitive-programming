#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class ProductsOfDigits {
public:
  i64 firstOccurrence(vector<int> prod) {
    for (int i = 1; i <= 1000; i++) {
      bool check = true;
      for (int j = 0; j < prod.size(); j++)
        if (P(i + j) != prod[j])
          check = false;
      if (check)
        return i;
    }

    i64 ret = 1LL << 62;
    for (int last = 0; last < 1000; last++) {
      bool check = true;
      i64 ratio = -1;
      for (int i = 0; i < prod.size(); i++) {
        int j = (i + last) % 1000, d = 1;
        for (int x = j, stp = 0; stp < 3; stp++, x /= 10)
          d *= (x % 10);
        if (d > prod[i])
          check = false;
        else if (d == 0) {
          if (prod[i] != 0)
            check = false;
        } else if (prod[i] % d) {
          check = false;
        } else if (prod[i] != 0) {
          i64 tmp = prod[i] / d;
          if (ratio < 0)
            ratio = tmp;
          if (ratio != tmp)
            check = false;
        }
      }
      if (check && ratio > 0)
        ret = min(ret, convert(ratio) * 1000 + last);
    }
    return ret;
  }

  i64 P(i64 x) {
    i64 d = 1;
    for (; x; x /= 10)
      d *= (x % 10);
    return d;
  }

  i64 convert(i64 ratio) {
    if (ratio == 1)
      return 1;
    vector<int> digit;
    for (int i = 9; i > 1; i--)
      while (ratio % i == 0) {
        digit.push_back(i);
        ratio /= i;
      }
    i64 ans = 0;
    for (int i = digit.size() - 1; i >= 0; i--)
      ans = ans * 10 + digit[i];
    return ans;
  }
};

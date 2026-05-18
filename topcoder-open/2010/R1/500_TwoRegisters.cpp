#include <bits/stdc++.h>
using namespace std;

class TwoRegisters {
public:
  string minProg(int r) {
    if (r == 1)
      return "";

    int bestLen = 1 << 30;
    string best = "";
    for (int y = 1; y <= r; y++) {
      if (gcd(r, y) != 1)
        continue;
      int len = cost(r, y);
      if (len > bestLen)
        continue;
      string prog = build(r, y);
      if (len < bestLen || prog < best) {
        bestLen = len;
        best = prog;
      }
    }
    return best;
  }

  int cost(int a, int b) {
    int c = 0;
    while (a != 1 || b != 1) {
      if (a > b) {
        int k = (a - 1) / b;
        c += k;
        a -= k * b;
      } else {
        int k = (b - 1) / a;
        c += k;
        b -= k * a;
      }
    }
    return c;
  }

  int gcd(int a, int b) {
    while (b) {
      int t = a % b;
      a = b;
      b = t;
    }
    return a;
  }

  string build(int a, int b) {
    string rev;
    while (a != 1 || b != 1) {
      if (a > b) {
        int k = (a - 1) / b;
        rev.append(k, 'X');
        a -= k * b;
      } else {
        int k = (b - 1) / a;
        rev.append(k, 'Y');
        b -= k * a;
      }
    }
    reverse(rev.begin(), rev.end());
    return rev;
  }
};

#include <bits/stdc++.h>
using namespace std;

#define maxn 105

static constexpr unsigned MOD = 1'000'000'007;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

Mint binom[maxn][maxn];
Mint fact[maxn];

class SkyscraperCounting {
public:
  int count(string visibility) {
    if (visibility[0] == 'X') {
      return 0;
    }

    int n = visibility.size();
    fact[0] = 1;
    for (int i = 1; i <= n; i++) {
      fact[i] = fact[i - 1] * i;
    }
    binom[0][0] = 1;
    for (int i = 1; i < maxn; i++) {
      for (int j = 0; j <= i; j++) {
        binom[i][j] = binom[i - 1][j];
        if (j) {
          binom[i][j] += binom[i - 1][j - 1];
        }
      }
    }

    Mint ret = 1;
    int last = n;
    for (int i = n - 1; i >= 0; i--)
      if (visibility[i] == 'O') {
        int x = i, y = last - i - 1;
        ret *= binom[x + y][y];
        ret *= fact[y];
        last = i;
      }
    return (int)ret.x;
  }
};

// Powered by KawigiEdit 2.1.4 (beta) modified by pivanof!

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class ArtShift {
  i64 gcdll(i64 a, i64 b) {
    while (b) {
      i64 t = a % b;
      a = b;
      b = t;
    }
    return a;
  }

  i64 lcmll(i64 a, i64 b) {
    return a / gcdll(a, b) * b;
  }

  int n, t;
  i64 best;

  void dfs(int rem, int cycles, i64 curLcm, int minSize) {
    if (cycles > t)
      return;
    best = max(best, curLcm);
    if (rem == 0 || cycles == t)
      return;
    for (int s = minSize; s <= rem; s++) {
      i64 nl = lcmll(curLcm, s);
      dfs(rem - s, cycles + 1, nl, s);
    }
  }

public:
  int maxShifts(string sequence) {
    n = (int)sequence.size();
    int w = 0, b = 0;
    for (char c : sequence) {
      if (c == 'W')
        w++;
      else
        b++;
    }
    t = min(w, b);
    if (t == 0)
      return 1;
    best = 1;
    dfs(n, 0, 1, 1);
    return (int)best;
  }
};

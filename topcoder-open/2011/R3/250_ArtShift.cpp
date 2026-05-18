#include <bits/stdc++.h>
using namespace std;

class ArtShift {
  long long gcdll(long long a, long long b) {
    while (b) {
      long long t = a % b;
      a = b;
      b = t;
    }
    return a;
  }

  long long lcmll(long long a, long long b) {
    return a / gcdll(a, b) * b;
  }

  int n, t;
  long long best;

  void dfs(int rem, int cycles, long long curLcm, int minSize) {
    if (cycles > t)
      return;
    best = max(best, curLcm);
    if (rem == 0 || cycles == t)
      return;
    for (int s = minSize; s <= rem; s++) {
      long long nl = lcmll(curLcm, s);
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

#include <bits/stdc++.h>
using namespace std;

struct EvaluatingElections {
  int evaluate(vector<int> districts) {
    int total = 0;
    for (int v : districts)
      total += v;

    const int INF = 1e9;
    vector<int> dp(total + 1, INF);
    dp[0] = 0;

    for (int v : districts) {
      int need = v / 2 + 1;
      int half = v / 2;
      vector<int> ndp(total + 1, INF);
      for (int c = 0; c <= total; c++)
        if (dp[c] < INF)
          for (int w = 0; w <= v; w++) {
            int d;
            if (w >= need)
              d = 1;
            else if (v % 2 == 0 && w == half)
              d = 0;
            else
              d = -1;
            if (c + w <= total)
              ndp[c + w] = min(ndp[c + w], dp[c] + d);
          }
      dp.swap(ndp);
    }

    for (int x = 1; x <= total; x++)
      if (dp[x] >= 1)
        return x;
    return total + 1;
  }
};

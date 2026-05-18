#include <bits/stdc++.h>
using namespace std;

struct YetAnotherRobotSimulation {
  double getMaximumProbability(int L, vector<int> locationsX, vector<int> locationsY) {
    set<pair<int, int>> targets;
    for (int i = 0; i < (int)locationsX.size(); i++)
      targets.insert(make_pair(locationsX[i], locationsY[i]));

    static long long C[51][51];
    for (int i = 0; i <= L; i++) {
      C[i][0] = 1;
      for (int j = 1; j <= i; j++)
        C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }

    long long best = 0;
    for (int u = 0; u <= L; u++)
      for (int r = 0; r <= L - u; r++)
        for (int l = 0; l <= L - u - r; l++) {
          int d = L - u - r - l;
          long long ways = 0;
          for (int pu = 0; pu <= u; pu++)
            for (int pr = 0; pr <= r; pr++)
              for (int pl = 0; pl <= l; pl++)
                for (int pd = 0; pd <= d; pd++) {
                  if (!targets.count(make_pair(pr - pl, pu - pd)))
                    continue;
                  ways += C[u][pu] * C[r][pr] * C[l][pl] * C[d][pd];
                }
          best = max(best, ways);
        }

    return (double)best / (1LL << L);
  }
};

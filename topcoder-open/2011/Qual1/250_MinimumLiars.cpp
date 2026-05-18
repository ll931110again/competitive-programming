#include <bits/stdc++.h>
using namespace std;

class MinimumLiars {
public:
  int getMinimum(vector<int> claim) {
    int n = claim.size();
    int ans = -1;
    for (int L = 0; L <= n; L++) {
      int liars = 0;
      for (int i = 0; i < n; i++)
        if (claim[i] > L)
          liars++;
      if (liars == L) {
        if (ans < 0 || L < ans)
          ans = L;
      }
    }
    return ans;
  }
};

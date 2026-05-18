#include <bits/stdc++.h>
using namespace std;

struct SellingGold {
  double minimalSum(vector<int> gold, vector<int> price) {
    const double INF = 1e100;
    int n = gold.size();
    int half = n / 2;
    int G = accumulate(gold.begin(), gold.end(), 0);
    int P = accumulate(price.begin(), price.end(), 0);

    vector<vector<double>> mn(half + 1, vector<double>(G + 1, INF));
    vector<vector<double>> mx(half + 1, vector<double>(G + 1, -INF));
    mn[0][0] = mx[0][0] = 0;

    for (int i = 0; i < n; i++)
      for (int k = half - 1; k >= 0; k--)
        for (int g = 0; g + gold[i] <= G; g++)
          if (mn[k][g] < INF / 2) {
            int nk = k + 1, ng = g + gold[i];
            double np = mn[k][g] + price[i];
            mn[nk][ng] = min(mn[nk][ng], np);
            mx[nk][ng] = max(mx[nk][ng], mx[k][g] + price[i]);
          }

    double ans = INF;
    for (int g = 1; g < G; g++) {
      if (mn[half][g] >= INF / 2)
        continue;
      int G2 = G - g;
      double p;
      if (g * 2 < G)
        p = mn[half][g];
      else if (g * 2 > G)
        p = mx[half][g];
      else
        p = mn[half][g];
      ans = min(ans, p / g + (P - p) / (double)G2);
    }
    return ans;
  }
};

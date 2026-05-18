#include <bits/stdc++.h>
using namespace std;

class JingleRingle {
public:
  int profit(vector<int> buyOffers, vector<int> sellOffers, int tax) {
    sort(buyOffers.begin(), buyOffers.end(), greater<int>());
    sort(sellOffers.begin(), sellOffers.end());
    int ret = 0;
    int n = min(buyOffers.size(), sellOffers.size());
    for (int i = 0; i < n; i++) {
      int gain = buyOffers[i] - sellOffers[i] - buyOffers[i] * tax / 100;
      if (gain <= 0)
        break;
      ret += gain;
    }
    return ret;
  }
};

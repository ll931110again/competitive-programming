#include <bits/stdc++.h>
using namespace std;

class CoinMachinesGame {
public:
  int maxGames(int coins, vector<int> need, vector<int> give) {
    int n = need.size();
    vector<int> order(n);
    for (int i = 0; i < n; i++)
      order[i] = i;
    sort(order.begin(), order.end(),
         [&](int a, int b) { return need[a] - give[a] < need[b] - give[b]; });

    auto can = [&](long long g) {
      long long c = coins;
      long long left = g;
      for (int id : order) {
        while (left > 0 && c >= need[id]) {
          c -= need[id] - give[id];
          left--;
        }
      }
      return left == 0;
    };

    long long lo = 0, hi = 1;
    while (can(hi)) {
      lo = hi;
      hi *= 2;
    }
    while (lo + 1 < hi) {
      long long mid = (lo + hi + 1) / 2;
      if (can(mid))
        lo = mid;
      else
        hi = mid - 1;
    }
    return (int)lo;
  }
};

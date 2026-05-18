#include <bits/stdc++.h>
using namespace std;

class LinearKingdomParkingLot {
  static const int INF = 1 << 30;

  int addToSide(vector<int>& side, int car, const vector<int>& rank) {
    int cost = 0;
    int suffixMin = INF;
    for (int j = (int)side.size() - 1; j >= 0; --j) {
      int oldSuffix = suffixMin;
      suffixMin = min(suffixMin, rank[car]);
      if (suffixMin < rank[side[j]] && oldSuffix >= rank[side[j]])
        cost++;
    }
    side.push_back(car);
    return cost;
  }

public:
  int borrowKeys(vector<int> exitOrder) {
    int n = exitOrder.size();
    vector<int> rank(n);
    for (int i = 0; i < n; i++)
      rank[exitOrder[i]] = i;

    const int BIG = 1 << 20;
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, BIG));
    vector<vector<vector<int>>> left(n + 1, vector<vector<int>>(n + 1));
    vector<vector<vector<int>>> right(n + 1, vector<vector<int>>(n + 1));
    dp[0][0] = 0;

    for (int i = 0; i < n; i++) {
      for (int l = 0; l <= i; l++) {
        if (dp[i][l] >= BIG)
          continue;
        int r = i - l;
        vector<int> addLeft = left[i][l];
        vector<int> addRight = right[i][l];
        int base = dp[i][l];

        {
          vector<int> L = addLeft, R = addRight;
          int cost = addToSide(L, i, rank);
          int nl = l + 1, nr = r;
          if (base + cost < dp[nl + nr][nl]) {
            dp[nl + nr][nl] = base + cost;
            left[nl + nr][nl] = L;
            right[nl + nr][nl] = R;
          }
        }
        {
          vector<int> L = addLeft, R = addRight;
          int cost = addToSide(R, i, rank);
          int nl = l, nr = r + 1;
          if (base + cost < dp[nl + nr][nl]) {
            dp[nl + nr][nl] = base + cost;
            left[nl + nr][nl] = L;
            right[nl + nr][nl] = R;
          }
        }
      }
    }

    int ans = BIG;
    for (int l = 0; l <= n; l++)
      ans = min(ans, dp[n][l]);
    return ans;
  }
};

#ifdef LOCAL_TEST
#include <cassert>
void check(vector<int> exitOrder, int expected) {
  int got = LinearKingdomParkingLot().borrowKeys(exitOrder);
  if (got != expected) {
    fprintf(stderr, "FAIL expected %d got %d\n", expected, got);
    exit(1);
  }
}
int main() {
  check({4, 1, 0, 2, 3}, 1);
  check({0, 1}, 0);
  check({1, 3, 5, 7, 0, 2, 4, 6}, 4);
  check({1, 0}, 0);
  check({2, 0, 3, 1}, 0);
  check({4, 0, 3, 2, 1, 5}, 1);
  check({1, 5, 3, 0, 6, 7, 2, 4}, 3);
  check({7, 2, 8, 6, 5, 4, 1, 0, 3}, 0);
  check({8, 2, 10, 9, 5, 0, 1, 3, 4, 6, 7}, 4);
  check({0, 3, 9, 4, 10, 6, 5, 1, 7, 8, 2, 11}, 7);
  fprintf(stderr, "ok\n");
}
#endif

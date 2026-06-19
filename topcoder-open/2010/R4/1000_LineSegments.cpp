#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
#define point pair<i64, i64>
#define PI acos(-1.0)

point P[1202];

class LineSegments {
public:
  i64 intersections(int N, int xFirst, int xAdd, int xProd, int xMod, int yFirst, int yAdd,
                    int yProd, int yMod) {
    P[0] = make_pair(xFirst, yFirst);
    for (int i = 1; i < N; i++) {
      P[i].first = (P[i - 1].first * xProd + xAdd) % xMod;
      P[i].second = (P[i - 1].second * yProd + yAdd) % yMod;
    }
    i64 total = 1LL * N * (N - 1) * (N - 2) * (N - 3) / 24, contain = 0;
    for (int center = 0; center < N; center++) {
      contain += 1LL * (N - 1) * (N - 2) * (N - 3) / 6;
      vector<double> store;
      for (int i = 0; i < N; i++)
        if (i != center) {
          double angular =
              atan2(1.0 * (P[i].second - P[center].second), 1.0 * (P[i].first - P[center].first));
          store.push_back(angular);
          store.push_back(angular + 2 * PI);
        }
      sort(store.begin(), store.end());
      int j = 0;
      for (int i = 0; i < N - 1; i++) {
        while (j < store.size() && store[j] - store[i] < PI)
          j++;
        int inside = j - i - 1;
        contain -= 1LL * inside * (inside - 1) / 2;
        if (i == j)
          j++;
      }
    }
    return total - contain;
  }
};

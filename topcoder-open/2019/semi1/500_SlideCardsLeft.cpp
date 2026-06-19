#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class SlideCardsLeft {
public:
  vector<int> getPosition(int N, i64 steps) {
    vector<int> ans(N);
    for (int i = 0; i < N; i++) {
      ans[i] = i;
    }

    for (int b = 60; b >= 0; b--) {
      if (steps & (1LL << b)) {
        if (b + 1 >= N) {
          return vector<int>();
        }
        swap(ans[b + 1], ans[0]);
      }
    }

    return ans;
  }
};

// Powered by KawigiEdit 2.1.4 (beta) modified by pivanof!

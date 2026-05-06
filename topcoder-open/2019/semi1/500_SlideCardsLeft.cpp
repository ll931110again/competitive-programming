#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

class SlideCardsLeft {
public:
  vector<int> getPosition(int N, long long steps) {
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
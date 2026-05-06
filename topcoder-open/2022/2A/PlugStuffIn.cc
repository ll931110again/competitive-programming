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

class PlugStuffIn {
public:
  vector<int> plug(vector<int> gadgets) {
    vector<int> rems = gadgets;
    int N = rems.size();
    rems.push_back(1);

    vector<int> ans(N);

    vector<bool> activated(N + 1);
    activated[N] = true;

    while (true) {
      bool proceed = false;
      for (int i = 0; i < N; i++)
        if (!activated[i] && rems[i] > 0) {
          for (int j = 0; j <= N; j++)
            if (activated[j] && rems[j] > 0) {
              activated[i] = true;
              ans[i] = j;
              rems[j]--;
              proceed = true;
              break;
            }
          if (proceed) {
            break;
          }
        }

      if (proceed) {
        continue;
      }

      for (int i = 0; i < N; i++)
        if (!activated[i]) {
          for (int j = 0; j <= N; j++)
            if (activated[j] && rems[j] > 0) {
              activated[i] = true;
              ans[i] = j;
              rems[j]--;
              proceed = true;
              break;
            }
          if (proceed) {
            break;
          }
        }

      if (!proceed) {
        break;
      }
    }
    for (int i = 0; i < N; i++)
      if (!activated[i]) {
        if (gadgets[i] == 0) {
          return vector<int>();
        } else {
          ans[i] = -1;
        }
      }
    return ans;
  }
};

// Powered by KawigiEdit 2.1.4 (beta) modified by pivanof!

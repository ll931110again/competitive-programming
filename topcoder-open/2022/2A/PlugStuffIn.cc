#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
 
using namespace std;
 
 
class PlugStuffIn {
public:
  vector <int> plug(vector <int> gadgets) {
    vector<int> rems = gadgets;
    int N = rems.size();
    rems.push_back(1);
    
    vector<int> ans(N);
 
    vector<bool> activated(N + 1);
    activated[N] = true;
    
    while (true) {
      bool proceed = false;
      for (int i = 0; i < N; i++) if (!activated[i] && rems[i] > 0) {
        for (int j = 0; j <= N; j++) if (activated[j] && rems[j] > 0) {
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
      
      for (int i = 0; i < N; i++) if (!activated[i]) {
        for (int j = 0; j <= N; j++) if (activated[j] && rems[j] > 0) {
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
    for (int i = 0; i < N; i++) if (!activated[i]) {
      if (gadgets[i] == 0) {
        return vector<int>();
      } else {
        ans[i] = -1;
      }
    }
    return ans;
  }
};
 
 
 
//Powered by KawigiEdit 2.1.4 (beta) modified by pivanof!

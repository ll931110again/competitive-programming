// {{{ Boilerplate Code <--------------------------------------------------
//
// vim:filetype=cpp foldmethod=marker foldmarker={{{,}}}
 
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
 
#define FOR(I,A,B)  for(int I = (A); I < (B); ++I)
#define REP(I,N)  FOR(I,0,N)
#define ALL(A)    (A).begin(), (A).end()
 
using namespace std;
 
// }}}
 
class BlurredDartboard
{
public:
  int minThrows(vector <int> points, int P)
  {
    int maxExist = 0,n = points.size();
    for (int i = 0; i < points.size(); i++) maxExist = max(maxExist,points[i]);
    vector<int> hidden;
    for (int i = 1; i <= n; i++) {
      bool flag = false;
      for (int j = 0; j < n; j++) if (points[j] == i) flag = true;
      if (!flag) hidden.push_back(i);
    }
    int sumHidden = 0;
    for (int i = 0; i < hidden.size(); i++) sumHidden += hidden[i];
 
    if (hidden.empty()) return (int) ceil(1.0 * P/maxExist);
    int ret = P;
    for (int remain = 0; remain < hidden.size(); remain++) {
      int score = 0;
      for (int i = 0; i < remain; i++) score += hidden[i];
      ret = min(ret,remain + minDart(maxExist,sumHidden,hidden.size(),P - score));
    }
    return ret;
  }
 
  int minDart(int a,int b,int attempt,int remain) {
    if (remain <= 0) return 0;
    if (a * attempt >= b) return (int) ceil(1.0 * remain/a);
    int ret = remain;
    for (int i = 0; i <= 100; i++) {
      int poss = max(0,remain - i * a);
      ret = min(ret,i + attempt * (int) ceil(1.0 * poss/b));
    }
    return ret;
  }
};
 
 
// Edited by VimCoder 0.3.2
// http://www.dogcows.com/vimcoder

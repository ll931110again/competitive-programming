#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstring>
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
 
bool validS[50][50];
string bestS[50][50];
bool validT[50][50];
string bestT[50][50];
int n;
 
struct TheLargestString {
  string find(string s, string t) {
    n = s.size();
    memset(validS,false,sizeof(validS));
    for (int i = 0; i <= n; i++) validS[i][0] = true;
    for (int j = 1; j <= n; j++)
      for (int i = 1; i <= n; i++) {
        bestS[i][j].clear();
        validS[i][j] = validS[i - 1][j] || validS[i - 1][j - 1];
        if (validS[i - 1][j]) bestS[i][j] = bestS[i - 1][j];
        if (validS[i - 1][j - 1]) bestS[i][j] = max(bestS[i][j],bestS[i - 1][j - 1] + s[i - 1]);
      }
    string ret = "";
    for (int len = 1; len <= n; len++)
      ret = max(ret,bestS[n][len] + fixlen(s,t,len));
    return ret;
  }
 
  string fixlen(string s, string t, int len) {
    string form = bestS[n][len];
    memset(validT,false,sizeof(validT));
    for (int i = 0; i <= n; i++) validT[i][0] = true;
    for (int j = 1; j <= len; j++)
      for (int i = 1; i <= n; i++) {
        bestT[i][j].clear();
        if (validT[i - 1][j]) {
          validT[i][j] = true;
          bestT[i][j] = bestT[i - 1][j];
        }
        if (s[i - 1] == form[j - 1] && validT[i - 1][j - 1]) {
          validT[i][j] = true;
          bestT[i][j] = max(bestT[i][j],bestT[i - 1][j - 1] + t[i - 1]);
        }
      }
    return bestT[n][len];
  }
};
 
 
// Edited by VimCoder 0.3.1
// http://www.dogcows.com/vimcoder

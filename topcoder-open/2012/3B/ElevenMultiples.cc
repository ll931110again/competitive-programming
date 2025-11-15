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
#include <cstring>
#define mod 1000000007
using namespace std;
 
long long dp1[55][55][11];
long long dp2[105][105][11];
long long fact[115];
pair<int,int> info[55];
int n,odd;
 
struct ElevenMultiples {
  int countMultiples(vector <string> pieces) {   
    fact[0] = 1;
    for (int i = 1; i <= 110; i++) fact[i] = (fact[i - 1] * i) % mod;
 
    n = pieces.size();
    for (int i = 0; i < n; i++) {
      info[i].first = 0;
      info[i].second = pieces[i].size() % 2;
      for (int j = 0; j < pieces[i].size(); j++) info[i].first = (info[i].first * 10 + pieces[i][j] - '0') % 11;
    }
    vector<int> oddPlate;
    for (int i = 0; i < n; i++) if (info[i].second) oddPlate.push_back(info[i].first);
    odd = oddPlate.size();
 
    memset(dp1,0,sizeof(dp1));
    dp1[0][0][0] = 1;
    for (int i = 0; i <= odd; i++)
      for (int j = 0; i + j < odd; j++)
        for (int k = 0; k < 11; k++) if (dp1[i][j][k]) {
    int pos = i + j;
    int _k = (k + oddPlate[pos]) % 11;
    dp1[i + 1][j][_k] = (dp1[i + 1][j][_k] + dp1[i][j][k]) % mod;
    _k = (k + 11 - oddPlate[pos]) % 11;
    dp1[i][j + 1][_k] = (dp1[i][j + 1][_k] + dp1[i][j][k]) % mod;
  }
    
    int lpos = (odd + 1)/2,lneg = odd - lpos;    
    int rpos,rneg;
    if (lpos > lneg) rpos = rneg = lpos; else rpos = lpos + 1,rneg = lpos;
 
    memset(dp2,0,sizeof(dp2));
    long long prod = (fact[lpos] * fact[lneg]) % mod;
    for (int i = 0; i < 11; i++) dp2[rpos][rneg][i] = (dp1[lpos][lneg][i] * prod) % mod;
 
    vector<int> evenPlate;
    for (int i = 0; i < n; i++) if (!info[i].second) evenPlate.push_back(info[i].first);
    for (int i = rpos; i <= 102; i++)
      for (int j = rneg; j <= 102; j++)
        for (int k = 0; k < 11; k++) if (dp2[i][j][k]) {
    int pos = i - rpos + j - rneg;
    if (pos >= evenPlate.size()) continue;
    int _k = (k + evenPlate[pos]) % 11;
    dp2[i + 1][j][_k] = (dp2[i + 1][j][_k] + dp2[i][j][k] * i) % mod;
    _k = (k + 11 - evenPlate[pos]) % 11;
    dp2[i][j + 1][_k] = (dp2[i][j + 1][_k] + dp2[i][j][k] * j) % mod;
  }
    
    long long ret = 0;
    for (int i = 0; i <= 102; i++)
      for (int j = 0; j <= 102; j++) 
        if (i - rpos + j - rneg == evenPlate.size()) ret = (ret + dp2[i][j][0]) % mod;
    return (int) ret;
  }
};
 
 
// Edited by VimCoder 0.3.2
// http://www.dogcows.com/vimcoder

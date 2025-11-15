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
 
int whose[55];
vector<int> a;
int diff[55][55],own[55][55];
int INF = 1 << 29;
 
class HeavyBooks
{
public:
  vector <int> findWeight(vector <int> books, vector <int> moves)
  {
    for (int i = 1; i <= moves[0]; i++) whose[i] = 1;
    for (int j = 1; j < moves.size(); j++) {
      int turn = j % 2;
      for (int i = moves[0],pick = moves[j]; i > 0 && pick > 0; i--)
        if (whose[i] == turn) {
          pick--;  whose[i] ^= 1;
        }
    }
 
    a.clear();
    a.push_back(0);
    for (int i = 0; i < books.size(); i++) a.push_back(books[i]);
    sort(a.begin(),a.end());
 
    for (int i = 0; i < a.size(); i++)
      for (int j = 0; j < a.size(); j++) diff[i][j] = INF;

    diff[0][0] = own[0][0] = 0;
    for (int i = 1; i < a.size(); i++)
      for (int j = 0; j <= moves[0]; j++) {
        // not pick book i
        diff[i][j] = diff[i - 1][j];  own[i][j] = own[i - 1][j];
        if (j) {
          int nextDiff = diff[i - 1][j - 1];
    if (whose[j] == 0) nextDiff += a[i]; else nextDiff -= a[i];
    int nextOwn = own[i - 1][j - 1];
    if (whose[j] == 0) nextOwn += a[i];
 
    if (diff[i][j] > nextDiff || (diff[i][j] == nextDiff && own[i][j] < nextOwn)) {
      diff[i][j] = nextDiff;
      own[i][j] = nextOwn;
    }
        }
      }
 
    vector<int> ret(2);
    ret[0] = own[a.size() - 1][moves[0]];
    ret[1] = -(diff[a.size() - 1][moves[0]] - ret[0]);
    return ret;
  }
};
 
 
// Edited by VimCoder 0.3.2
// http://www.dogcows.com/vimcoder

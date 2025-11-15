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
 
int dist[55];
bool a[55][55];
 
class DancingFoxes
{
public:
  int minimalDays(vector <string> friendship)
  {
    int n = friendship.size();
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        a[i][j] = (friendship[i][j] == 'Y');
    for (int i = 0; i < n; i++) dist[i] = -1;
    dist[0] = 0;
    queue<int> q;
    q.push(0);
    while (!q.empty()) {
      int u = q.front();  q.pop();
      for (int i = 0; i < n; i++)
        if (a[u][i] && dist[i] < 0) {
          dist[i] = 1 + dist[u];
    q.push(i);
        }
    }
    if (dist[1] < 0) return -1;
    int cnt = 0,rem = dist[1];
    while (rem > 1) {
      cnt++;
      int tmp = rem;
      for (int i = 0; i < rem; i += 3)
        if (i + 2 <= rem) tmp--;
      rem = tmp;
    }
    return cnt;
  }
};
 
 
// Edited by VimCoder 0.3.4
// http://github.com/chazmcgarvey/vimcoder

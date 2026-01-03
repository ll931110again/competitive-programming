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
 
#define FOR(I,A,B)	for(int I = (A); I < (B); ++I)
#define REP(I,N)	FOR(I,0,N)
#define ALL(A)		(A).begin(), (A).end()
 
using namespace std;
 
// }}}
 
class BankLottery
{
public:
	double expectedAmount(vector <int> accountBalance, int weeklyJackpot, int weekCount)
	{
	  int total = 0,mine = accountBalance[0];
	  for (int i = 0; i < accountBalance.size(); i++) total += accountBalance[i];
	  double ret = mine;
	  for (int i = 0; i < weekCount; i++) {
	    ret *= (1 + 1.0 * weeklyJackpot/total);
	    total += weeklyJackpot;
	  }
	  return ret;
	}
};
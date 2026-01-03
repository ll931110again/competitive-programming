 
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
 
class ProductsOfDigits
{
public:
	long long firstOccurrence(vector <int> prod)
	{
	  for (int i = 1; i <= 1000; i++) {
	    bool check = true;
	    for (int j = 0; j < prod.size(); j++)
	      if (P(i + j) != prod[j]) check = false;
	    if (check) return i;
	  }
 
	  long long ret = 1LL << 62;
	  for (int last = 0; last < 1000; last++) {
	    bool check = true;
	    long long ratio = -1;
	    for (int i = 0; i < prod.size(); i++) {
	      int j = (i + last) % 1000,d = 1;
	      for (int x = j,stp = 0; stp < 3; stp++,x /= 10) d *= (x % 10);
	      if (d > prod[i]) check = false; else
	      if (d == 0) {
	        if (prod[i] != 0) check = false;
	      } else if (prod[i] % d) {
	      	check = false;
	      } else if (prod[i] != 0) {
	        long long tmp = prod[i]/d;
			if (ratio < 0) ratio = tmp;
			if (ratio != tmp) check = false;
	      }
	    }
	    if (check && ratio > 0) ret = min(ret,convert(ratio) * 1000 + last);
	  }
	  return ret;
	}
 
	long long P(long long x) {
	  long long d = 1;
	  for (; x; x /= 10) d *= (x % 10);
	  return d;
	}
 
	long long convert(long long ratio) {
	  if (ratio == 1) return 1;
	  vector<int> digit;
	  for (int i = 9; i > 1; i--) while (ratio % i == 0) {
	    digit.push_back(i);
	    ratio /= i;
	  }
	  long long ans = 0;
	  for (int i = digit.size() - 1; i >= 0; i--) ans = ans * 10 + digit[i];
	  return ans;
	}
};
 
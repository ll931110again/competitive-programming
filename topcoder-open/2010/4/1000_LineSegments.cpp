 
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
#define point pair<long long,long long>
#define PI acos(-1.0)
 
#define FOR(I,A,B)	for(int I = (A); I < (B); ++I)
#define REP(I,N)	FOR(I,0,N)
#define ALL(A)		(A).begin(), (A).end()
 
using namespace std;
 
// }}}
 
point P[1202];
 
class LineSegments
{
public:
	long long intersections(int N, int xFirst, int xAdd, int xProd, int xMod, int yFirst, int yAdd, int yProd, int yMod)
	{
	  P[0] = make_pair(xFirst,yFirst);
	  for (int i = 1; i < N; i++) {
	    P[i].first = (P[i - 1].first * xProd + xAdd) % xMod;
	    P[i].second = (P[i - 1].second * yProd + yAdd) % yMod;
	  }
	  long long total = 1LL * N * (N - 1) * (N - 2) * (N - 3)/24,contain = 0;
	  for (int center = 0; center < N; center++) {
	    contain += 1LL * (N - 1) * (N - 2) * (N - 3)/6;
	    vector<double> store;
	    for (int i = 0; i < N; i++) if (i != center) {
	      double angular = atan2(1.0 * (P[i].second - P[center].second),1.0 * (P[i].first - P[center].first));
	      store.push_back(angular);
	      store.push_back(angular + 2 * PI);
	    }
	    sort(store.begin(),store.end());
	    int j = 0;
	    for (int i = 0; i < N - 1; i++) {
	      while (j < store.size() && store[j] - store[i] < PI) j++;
	      int inside = j - i - 1;
	      contain -= 1LL * inside * (inside - 1)/2;
	      if (i == j) j++;
	    }
	  }
	  return total - contain;
	}
};
 
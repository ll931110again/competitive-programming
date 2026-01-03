#include <algorithm>
#include <bitset>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
 
struct EnclosingSquare
{
	long long getMinimum(long long n)
	{
		if (n < 2) return n + 1;
 
		n--;
		long long ret = 1LL << 60;
		for (int d = 1; d <= n/d; d++) if (n % d == 0)
		{
			ret = min(ret,candidate(n,d));
			ret = min(ret,candidate(n,n/d));
		}
 
		return (ret >= (1LL << 60)) ? 0 : ret;
	}
 
	long long candidate(long long n,long long d)
	{
		long long rem = n/d + 2;
		if (rem % d) return 1LL << 60;
		rem /= d;
		for (int x = 0; 1LL * x * x <= rem; x++)
		{
			int y = (int) trunc(sqrt(rem - 1LL * x * x));
			if (y < x) return (1LL << 60);
			if ((1LL * x * x + 1LL * y * y == rem) && __gcd(x,y) == 1) return (1LL * d * d * rem);
		}
		return (1LL << 60);
	}
};
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
using namespace std;
 
int L,R;
 
long long revert(long long x)
{
	int tmp = x;
	long long sum = 0;
	while (tmp)
	{
		sum = sum * 10LL + 9;  tmp /= 10;
	}
	return sum - x;
}
 
int main()
{
	//freopen("a.in","r",stdin);
	
	scanf("%d %d", &L, &R);
	long long ret = max(1LL * L * revert(L),1LL * R * revert(R));
	
	for (long long sum = 9,start = 1; sum <= 99999999999LL; sum = sum * 10LL + 9,start *= 10LL)
	{
		if (L <= start && start <= R) ret = max(ret,start * revert(start));
		if (L <= sum/2 && sum/2 <= R) ret = max(ret,(sum/2) * revert(sum/2));
	}
	cout << ret << endl;
}

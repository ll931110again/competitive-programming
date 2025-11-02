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

vector< pair<int,int> > v,contain;
int n,m;

bool cover(pair<int,int> p1,pair<int,int> p2)
{
	if (p1.first > p1.second) swap(p1.first,p1.second);
	if (p2.first > p2.second) swap(p2.first,p2.second);
	return (p1.first <= p2.first && p2.second <= p1.second);
}

int main()
{
	//freopen("rijeka.in.2","r",stdin);
	
	scanf("%d %d", &n, &m);
	for (int i = 0; i < n; i++)
	{
		int start,finish;  scanf("%d %d", &start, &finish);
		if (start <= finish) continue;
		v.push_back(make_pair(start,finish));
	}
	if (v.empty())
	{
		printf("%d\n", m);  return 0;
	}
	
	sort(v.begin(),v.end());
//	for (int i = 0; i < v.size(); i++) cout << v[i].first << ' ' << v[i].second << endl;
	for (int i = 0; i < v.size(); i++)
	{
		while (!contain.empty() && cover(v[i],contain[contain.size() - 1])) contain.pop_back();
		if (contain.empty()) contain.push_back(v[i]); else
		{
			if (cover(contain[contain.size() - 1],v[i])) continue;
			if (v[i].second <= contain[contain.size() - 1].first) contain[contain.size() - 1].first = v[i].first; else contain.push_back(v[i]);
		}
	}
	long long ret = m;
	for (int i = 0; i < contain.size(); i++) ret += 2LL * abs(contain[i].first - contain[i].second);
	printf("%lld\n", ret);
}

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
#define maxn 200005
#define maxv 1000005
using namespace std;
 
struct query
{
	int x,y,z;
	query (int _x,int _y,int _z)
	{
		x = _x;  y = _y;  z = _z;
	}
};
 
bool cmp(query A,query B)
{
	return A.y < B.y;
}
 
int n,Q,block;
int a[maxn],cnt[maxv];
vector<query> bucket[maxn];
long long ret[maxn];
long long sum;
 
void modify(int pos,int delta)
{
	long long prev = 1LL * cnt[pos] * cnt[pos] * pos;
	cnt[pos] += delta;
	long long next = 1LL * cnt[pos] * cnt[pos] * pos;
	sum += (next - prev);
}
 
int main()
{
	//freopen("d.in","r",stdin);
	
	scanf("%d %d", &n, &Q);
	block = (int) floor(sqrt(n));  if (!block) block = 1;
	for (int i = 0; i < n; i++) scanf("%d", &a[i]);
	
	for (int i = 0; i < Q; i++)
	{
		int u,v;  scanf("%d %d", &u, &v); u--;  v--;
		bucket[u/block].push_back(query(u,v,i));
	}
	memset(cnt,0,sizeof(cnt));
	
	for (int it = 0; it < n; it++) if (!bucket[it].empty())
	{
		sum = 0;
		sort(bucket[it].begin(),bucket[it].end(),cmp);
		for (int i = bucket[it][0].x; i <= bucket[it][0].y; i++) modify(a[i],1);
		ret[bucket[it][0].z] = sum;
		for (int j = 1; j < bucket[it].size(); j++)
		{
			for (int i = bucket[it][j - 1].y + 1; i <= bucket[it][j].y; i++) modify(a[i],1);
			if (bucket[it][j - 1].x > bucket[it][j].x)
			  for (int i = bucket[it][j - 1].x - 1; i >= bucket[it][j].x; i--) modify(a[i],1);
			else
			  for (int i = bucket[it][j - 1].x; i < bucket[it][j].x; i++) modify(a[i],-1);
			ret[bucket[it][j].z] = sum;
		}
		int sz = bucket[it].size() - 1;
		for (int i = bucket[it][sz].x; i <= bucket[it][sz].y; i++) modify(a[i],-1);
	}
	
	for (int i = 0; i < Q; i++) printf("%I64d\n", ret[i]);
}

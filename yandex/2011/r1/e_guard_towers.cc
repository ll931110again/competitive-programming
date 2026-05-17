// Codeforces 85 (Yandex.Algorithm 2011: Round 1) — E. Guard Towers
// Submission: https://codeforces.com/contest/85/submission/462887

#include <bits/stdc++.h>
#define mod 1000000007
using namespace std;

int n;
pair<int,int> p[5002];
int side[5002];
bool flag;
long long way;

long long power(int x,int p)
{
	if (!p) return 1;
	long long q = power(x,p/2);
	q = (q * q) % mod;
	if (p & 1) q = (q * x) % mod;
	return q;
}

void DFS(int u,int mid)
{
	for (int i = 0; i < n; i++)
	{
		int dist = abs(p[u].first - p[i].first) + abs(p[u].second - p[i].second);
		if (dist <= mid) continue;
		if (side[i] >= 0 && side[i] == side[u])
		{
			flag = false;  return;
		}
		if (side[i] < 0)
		{
			side[i] = 1 ^ side[u];  DFS(i,mid);
		}
	}
}

bool ok(int mid)
{
	memset(side,-1,sizeof(side));
	int comp = 0;  flag = true;
	for (int i = 0; i < n; i++) if (side[i] < 0)
	{
		comp++;  side[i] = 0;  DFS(i,mid);
		if (!flag) return false;
	}
	way = power(2,comp);
	return true;
}

int main()

{
//	freopen("e.in","r",stdin);
//	freopen("e.ou","w",stdout);
	
	scanf("%d", &n);
	for (int i = 0; i < n; i++) scanf("%d %d", &p[i].first, &p[i].second);
	
	int low = 0,high = 10000,ans = high;
	while (low <= high)
	{
		long long mid = (low + high)/2;
		if (ok(mid))
		{
			ans = mid;  high = mid - 1;
		}
		else low = mid + 1;
	}
	cout << ans << endl;  cout << way << endl;
}

// Codeforces 85 (Yandex.Algorithm 2011: Round 1) — B. Embassy Queue
// Submission: https://codeforces.com/contest/85/submission/461040

#include <bits/stdc++.h>
using namespace std;

int num[4],wait[4];
int n;
int a[100010];
priority_queue<long long> q[4];

int main()
{
	//freopen("b.in","r",stdin);
	//freopen("b.ou","w",stdout);
	
	for (int i = 1; i <= 3; i++) scanf("%d", &num[i]);
	for (int i = 1; i <= 3; i++) scanf("%d", &wait[i]);
	scanf("%d", &n);
	for (int i = 0; i < n; i++) scanf("%d", &a[i]);
	
	for (int i = 1; i <= 3; i++)
	{
		num[i] = min(num[i],n);
		for (int j = 0; j < num[i]; j++) q[i].push(0);
	}
	
	long long ret = 0;
	for (int i = 0; i < n; i++)
	{
		long long waitTime = a[i];
		for (int j = 1; j <= 3; j++)
		{
			long long used = -q[j].top();  q[j].pop();
			waitTime = max(waitTime,used) + wait[j];
			q[j].push(-waitTime);
		}
		ret = max(ret,waitTime - a[i]);
	}
	cout << ret << endl;
}

// Codeforces 85 (Yandex.Algorithm 2011: Round 1) — D. Sum of Medians
// Submission: https://codeforces.com/contest/85/submission/468531

#include <bits/stdc++.h>
#define maxn 100005
using namespace std;

string s[maxn];
int val[maxn];
int Q,n;
vector< pair<int,int> > v;
vector<int> store;
map<int,int> mp;

struct node
{
	int num;
	long long total[5];
} tx[6 * maxn];

void add(int i,int low,int high,int pos,int value)
{
	if (low == high)
	{
		tx[i].num = 1;  tx[i].total[0] = value;  return;
	}
	int mid = (low + high)/2;
	if (pos <= mid) add(2 * i,low,mid,pos,value); else add(2 * i + 1,mid + 1,high,pos,value);
	
	tx[i].num = tx[2 * i].num + tx[2 * i + 1].num;
	for (int j = 0; j < 5; j++)
	{
		int k = (tx[2 * i].num + j) % 5;
		tx[i].total[k] = tx[2 * i].total[k] + tx[2 * i + 1].total[j];
	}
}

void del(int i,int low,int high,int pos)
{
	if (low == high)
	{
		tx[i].num = 0;  tx[i].total[0] = 0;  return;
	}
	int mid = (low + high)/2;
	if (pos <= mid) del(2 * i,low,mid,pos); else del(2 * i + 1,mid + 1,high,pos);
	
	tx[i].num = tx[2 * i].num + tx[2 * i + 1].num;
	for (int j = 0; j < 5; j++)
	{
		int k = (tx[2 * i].num + j) % 5;
		tx[i].total[k] = tx[2 * i].total[k] + tx[2 * i + 1].total[j];
	}
}

int main()
{
	//freopen("d.in","r",stdin);
	
	scanf("%d", &Q);
	for (int i = 0; i < Q; i++)
	{
		cin >> s[i];  
		if (s[i] == "sum") continue;
		cin >> val[i];
		if (s[i] == "add") v.push_back(make_pair(val[i],i));
	}
	v.push_back(make_pair(0,-1));
	sort(v.begin(),v.end());
	
	n = v.size();
	int cnt = -1;
	for (int i = 0; i < n; i++)
	{
		if (!i || v[i].first > v[i - 1].first) 
		{
			cnt++;  mp[v[i].first] = cnt;  store.push_back(v[i].first);
		}
		if (v[i].second >= 0) val[v[i].second] = cnt;
	}
			
	for (int i = 0; i < Q; i++)
	{
	  if (s[i] == "add") add(1,1,cnt,val[i],store[val[i]]);
	  else if (s[i] == "del") 
	  {
		  //cout << "delpos: " << val[i] << endl;
		  del(1,1,cnt,mp[val[i]]);
	  }
	  else printf("%I64d\n", tx[1].total[2]);
    }
}

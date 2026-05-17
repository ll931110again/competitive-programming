// Codeforces 85 (Yandex.Algorithm 2011: Round 1) — A. Domino
// Submission: https://codeforces.com/contest/85/submission/460533

#include <bits/stdc++.h>
using namespace std;

int dx[4] = {-1,0,1,0};
int dy[4] = {0,1,0,-1};
int c[5][105],n;
bool poss[26];

void place(int ax,int ay,int bx,int by)
{
	memset(poss,true,sizeof(poss));
	for (int i = 0; i < 4; i++)
	{
		int cx = ax + dx[i],cy = ay + dy[i];
		if (cx < 0 || cx >= 4 || cy < 0 || cy >= n || c[cx][cy] < 0) continue;
		poss[c[cx][cy]] = false;
	}
	for (int i = 0; i < 4; i++)
	{
		int cx = bx + dx[i],cy = by + dy[i];
		if (cx < 0 || cx >= 4 || cy < 0 || cy >= n || c[cx][cy] < 0) continue;
		poss[c[cx][cy]] = false;		
	}
	for (int i = 0; i < 26; i++) if (poss[i])
	{
		c[ax][ay] = c[bx][by] = i;  break;
	}
}

int main()
{
	//freopen("a.in","r",stdin);
	//freopen("a.ou","w",stdout);
	
	scanf("%d", &n);
	memset(c,-1,sizeof(c));
	if (n % 2 == 0)
	{
		for (int i = 0; i < n; i += 2)
		{
			place(0,i,0,i + 1);  place(3,i,3,i + 1);
		}
		place(1,0,2,0);  place(1,n - 1,2,n - 1);
		for (int i = 1; i < n - 1; i += 2) 
		{
			place(1,i,1,i + 1);  place(2,i,2,i + 1);
		}
	}
	else
	{
		for (int i = 0; i < n; i += 2) 
		{
			place(0,i,0,i + 1);  place(1,i,1,i + 1);
		}
		place(0,n - 1,1,n - 1);
		place(2,0,3,0);
		for (int i = 1; i < n; i += 2)
		{
			place(2,i,2,i + 1);  place(3,i,3,i + 1);
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < n; j++) printf("%c", 'a' + c[i][j]);
		printf("\n");
	}
}

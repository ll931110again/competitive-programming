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
#define mod 1000000009
using namespace std;
 
int m,n;
int numRed[44][44],numBlue[44][44],numGreen[44][44];
int maxGreen[1940][1940];
int c[1940][1940],sumDelta[1940][1940];
int totalRed,totalBlue,totalGreen;
long long inv;
 
struct ColorfulJewelry
{
	long long power(int x,int p)
	{
		if (!p) return 1;
		long long q = power(x,p/2);
		q = (q * q) % mod;
		if (p & 1) q = (1LL * q * x) % mod;
		return q;
	}
 
	int getChains(vector<string> s,int k)
	{
		m = s.size();  n = s[0].size();
		memset(numRed,0,sizeof(numRed));
		memset(numBlue,0,sizeof(numBlue));
		memset(numGreen,0,sizeof(numGreen));
 
		for (int i = 0; i < m; i++)
		  for (int j = 0; j < n; j++)
		    if (s[i][j] == 'R') numRed[i][j]++; else if (s[i][j] == 'G') numGreen[i][j]++; else numBlue[i][j]++;
 
		for (int i = 1; i < m; i++)
		  for (int j = 0; j < n; j++)
		  {
		  	numRed[i][j] += numRed[i - 1][j];  numBlue[i][j] += numBlue[i - 1][j];  numGreen[i][j] += numGreen[i - 1][j];
		  }
 
		for (int j = 1; j < n; j++)
		  for (int i = 0; i < m; i++)
		  {
		  	numRed[i][j] += numRed[i][j - 1];  numBlue[i][j] += numBlue[i][j - 1];  numGreen[i][j] += numGreen[i][j - 1];
		  }
 
		c[0][0] = 1;
		for (int i = 1; i <= m * n; i++)
		  for (int j = 0; j <= i; j++)
		  {
		  	c[i][j] = c[i - 1][j];
		  	if (j) c[i][j] = (c[i][j] + c[i - 1][j - 1]) % mod;
		  }
 
		for (int i = 0; i <= m * n; i++)
		{
			sumDelta[i][0] = c[i][0];
			for (int j = 1; i + j <= m * n; j++) sumDelta[i][j] = (sumDelta[i][j - 1] + c[i + j][j]) % mod;
		}
 
		memset(maxGreen,-1,sizeof(maxGreen));
		for (int a = 0; a < m; a++)
		  for (int b = 0; b < n; b++)
		    for (int c = 0; c < m; c++)
		      for (int d = 0; d < n; d++) update(a,b,c,d,k);
 
		long long ret = 0;  inv = power(2,mod - 2);
		for (int i = m * n; i >= 0; i--)
		  for (int j = m * n - i; j >= 0; j--) if (maxGreen[i][j] >= 0)
		  {
		  	ret = (ret + compute(i,j)) % mod;
		  	if (i) maxGreen[i - 1][j] = max(maxGreen[i - 1][j],maxGreen[i][j]);
		  	if (j) maxGreen[i][j - 1] = max(maxGreen[i][j - 1],maxGreen[i][j]);
		  	if (!i && !j) ret = (ret + mod - 1) % mod;  // No jewels
		  }
 
		return (int) ret;
	}
 
	void adjust(int xa,int ya,int xb,int yb,int delta)
	{
		if (xa > xb || ya > yb) return;
		int Red = numRed[xb][yb],Blue = numBlue[xb][yb],Green = numGreen[xb][yb];
		if (xa) 
		{
			Red -= numRed[xa - 1][yb];  Blue -= numBlue[xa - 1][yb];  Green -= numGreen[xa - 1][yb];
		}
		if (ya)
		{
			Red -= numRed[xb][ya - 1];  Blue -= numBlue[xb][ya - 1];  Green -= numGreen[xb][ya - 1];
		}
		if (xa && ya)
		{
			Red += numRed[xa - 1][ya - 1];  Blue += numBlue[xa - 1][ya - 1];  Green += numGreen[xa - 1][ya - 1];
		}
		totalRed += Red * delta;  totalGreen += Green * delta;  totalBlue += Blue * delta;
	}
 
	void update(int a,int b,int c,int d,int k)
	{		
		int xa = a,ya = b,xb = a + k - 1,yb = b + k - 1;
		xa = max(xa,0);  ya = max(ya,0);  xb = min(xb,m - 1);  yb = min(yb,n - 1);
		int xc = c,yc = d,xd = c + k - 1,yd = d + k - 1;
		xc = max(xc,0);  yc = max(yc,0);  xd = min(xd,m - 1);  yd = min(yd,n - 1);		
		int xe = max(xa,xc),ye = max(ya,yc),xf = min(xb,xd),yf = min(yb,yd);		
 
		totalRed = totalBlue = totalGreen = 0;
		adjust(xa,ya,xb,yb,1);
		adjust(xc,yc,xd,yd,1);
		adjust(xe,ye,xf,yf,-1);			
 
		maxGreen[totalRed][totalBlue] = max(maxGreen[totalRed][totalBlue],totalGreen);
	}
 
	long long compute(int R,int B)
	{
		int G = maxGreen[R][B];
		long long ans = (1LL * c[R + B][R] * sumDelta[R + B][G]) % mod;
		if (R % 2 && B % 2 == 0) ans = (ans + 1LL * c[R/2 + B/2][R/2] * sumDelta[R/2 + B/2][G/2]) % mod;
		if (R % 2 == 0 && B % 2) ans = (ans + 1LL * c[R/2 + B/2][R/2] * sumDelta[R/2 + B/2][G/2]) % mod;
		if (R % 2 == 0 && B % 2 == 0) 
		{
			ans = (ans + 1LL * c[R/2 + B/2][R/2] * sumDelta[R/2 + B/2][G/2]) % mod;		
			if (G % 2 == 0) G--;
			if (G >= 0) ans = (ans + 1LL * c[R/2 + B/2][R/2] * sumDelta[R/2 + B/2][G/2]) % mod;
		}
		return (ans * inv) % mod;
	}
};
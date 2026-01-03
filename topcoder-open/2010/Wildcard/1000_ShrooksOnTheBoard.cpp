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
#define mod 100003
#define LIM 100
using namespace std;
 
long long fac[mod],inv[mod];
int sz;
 
struct matrix
{
	long long d[LIM + 2][LIM + 2];
};
 
matrix multiply(matrix A,matrix B)
{
	matrix C;
	memset(C.d,0,sizeof(C.d));
	for (int k = 0; k < sz; k++)
	  for (int i = 0; i < sz; i++)
	    for (int j = 0; j < sz; j++)
	      C.d[i][j] = (C.d[i][j] + A.d[i][k] * B.d[k][j]) % mod;
	return C;
}
 
matrix divi[35],ans;		
 
matrix mat_power(matrix A,int p)
{
	bool flag = false;
	for (int i = 0; p; i++)
	{
		if (i) divi[i] = multiply(divi[i - 1],divi[i - 1]); else divi[i] = A;
		int m = p % 2;  p /= 2;		
		if (!m) continue;
		if (!flag)
		{
			ans = divi[i];  flag = true;
		}
		else ans = multiply(ans,divi[i]);
	}
	return ans;
}
 
struct ShrooksOnTheBoard
{
	int count(int K,int H,int W)
	{
		K++;
		fac[0] = inv[0] = 1;
		for (int i = 1; i < mod; i++)
		{
			fac[i] = (fac[i - 1] * 1LL * i) % mod;
			inv[i] = power(fac[i],mod - 2);
		}
 
		long long one = (K <= LIM) ? matrix_method(K,W) : comb_method(K,W);
		return (int) (power(one,H) + mod - 1) % mod;
	}
 
	long long power(long long x,long long p)
	{
		if (!p) return 1;
		long long q = power(x,p/2);
		q = (q * q) % mod;
		if (p & 1) q = (q * x) % mod;
		return q;
	}
 
	long long C(int n,int k)
	{
		if (n < k) return 0;
		long long ret = (inv[k] * inv[n - k]) % mod;
		return (ret * fac[n]) % mod;
	}
 
	long long bigC(int n,int k)
	{
		long long ret = 1;
		while (n || k)
		{
			ret = (ret * C(n % mod,k % mod)) % mod;
			n /= mod;  k /= mod;
		}
		return ret;
	}
 
	long long comb_method(int K,int W)
	{		
		long long ret = 1;
		for (int i = 1; K * (i - 1) <= W; i++) 
			ret = (ret + bigC(W - K * (i - 1) + i - 1,i)) % mod;
		return ret;
	}
 
	long long matrix_method(int K,int W)
	{
		if (K >= W) return (W + 1) % mod;		
		matrix base;
		memset(base.d,0,sizeof(base.d));		
		sz = K;						
		for (int i = 1; i < sz; i++) base.d[i][i - 1] = 1;
		base.d[0][K - 1] = base.d[K - 1][K - 1] = 1;
 
		base = mat_power(base,W - K);
		long long ret = 0;
		for (int i = 0; i < K; i++) ret = (ret + 1LL * (i + 2) * base.d[i][K - 1]) % mod;
		return ret;
	}
};
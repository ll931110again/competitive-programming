#include <bits/stdc++.h>
#define mod 998244353
#define maxm 2000005
using namespace std;

long long fact[maxm], inv[maxm];

struct SpanningNoLine {
	int countSpanning(int n, int m) {
		fact[0] = 1;
		for (int i = 1; i <= 2 * m; i++) {
			fact[i] = fact[i - 1] * i % mod;
		}
		inv[2 * m] = get_power(fact[2 * m], mod - 2);
		for (int i = 2 * m - 1; i >= 0; i--) {
			inv[i] = inv[i + 1] * (i + 1) % mod;
		}
		
		long long nx = get_power(n, n - 2), ni = get_power(n, mod - 2);
		
		long long ret = 0;
		for (int k = 0; k < m; k++) {
			long long val = binom(2 * m - k - 1, k);
			val = val * nx % mod;
			if (k % 2 == 0) {
				ret = (ret + val) % mod;
			} else {
				ret = (ret + mod - val) % mod;
			}
			nx = (nx * ni) % mod;
		}
		return ret;
	}
	
	long long binom(int x, int y) {
		if (x < y) {
			return 0;
		}
		long long up = fact[x];
		long long down = inv[y] * inv[x - y] % mod;
		return (up * down) % mod;
	}
	
	long long get_power(int x, int p) {
		if (!p) {
			return 1;
		}
		long long q = get_power(x, p/2);
		q = (q * q) % mod;
		if (p & 1) {
			q = (q * x) % mod;
		}
		return q;
	}
};
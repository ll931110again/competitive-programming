#include <bits/stdc++.h>
using namespace std;

struct BagsOfNumbers {
	long long wonByAlice(int n, vector<long long> a, vector<long long> b, vector<long long> c, vector<long long> d) {
		long long nimber[55];
		for (int i = 0; i < n; i++) {
			nimber[i] = getNimber(a[i], b[i], c[i], d[i]);
		}
		
		int mat[55][64];
		
		for (int i = 0; i < n; i++) {
			for (int bit = 0; bit < 64; bit++) {
				mat[i][bit] = (nimber[i] & (1LL << bit)) > 0;
			}
		}
		
		int rank = 0;		
		for (int bit = 0; bit < 64; bit++) {
			for (int i = rank; i < n; i++) if (mat[i][bit] > 0) {
				for (int k = 0; k < 64; k++) {
					swap(mat[rank][k], mat[i][k]);
				}
				for (int j = rank + 1; j < n; j++) if (mat[j][bit]) {
					for (int k = 0; k < 64; k++) {
						mat[j][k] ^= mat[rank][k];
					}
				}
				rank++;
			}
		}
		
		return (1LL << n) - (1LL << (n - rank));
	}
	
	long long getNimber(long long n1, long long n2, long long n3, long long n4) {
		long long res = n4 + n2 / 2 + min(n1, n3);
		long long extra = max(n1, n3) - min(n1, n3);
		
		if (n2 % 2 == 0) {
			res += extra / 4;
		} else {
			res += (extra + 2) / 4;
		}
		
		return res;
	}
};
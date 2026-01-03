#include <bits/stdc++.h>
using namespace std;

map<long long, long long> hiMap, loMap;
long long hi, lo, hi2, lo2;
string allDigits;

int maxD = 60, maxV = 15;

struct FourDistinctDigits {
	string find(long long N, int D, int B) {
		srand(time(NULL));
		for (int i = 0; i < 10; i++) {
			allDigits += ('0' + i);
		}
		for (int i = 0; i < 26; i++) {
			allDigits += ('A' + i);
		}
		for (int i = 0; i < 26; i++) {
			allDigits += ('a' + i);
		}
		
		while (true) {
			generateRandomBits(D);
			long long rem = getRemainder(N, B);
			if (hiMap.count(rem)) {
				if (hi == hiMap[rem] && lo == loMap[rem]) {
					continue;
				}
				hi2 = hiMap[rem];
				lo2 = loMap[rem];
				return constructSolution(D, B);
			} else {
				hiMap[rem] = hi;
				loMap[rem] = lo;
			}
		}		
	}
	
	string constructSolution(int D, int B) {
		if (hi2 < hi || (hi2 == hi && lo2 < lo)) {
			swap(hi2, hi);
			swap(lo2, lo);
		}
		
		int digits[maxD * 2], digits2[maxD * 2];
		for (int i = maxD - 1; i >= 0; i--) {
			digits[i] = (lo & (1LL << i)) ? 1 : 0;
			digits[i + maxD] = (hi & (1LL << i)) ? 1 : 0;
			
			digits2[i] = (lo2 & (1LL << i)) ? 1 : 0;
			digits2[i + maxD] = (hi2 & (1LL << i)) ? 1 : 0;
		}
		
		for (int i = 0; i < maxD * 2; i++) {
			digits2[i] -= digits[i];
			if (digits2[i] < 0) {
				digits2[i] += B;
				digits2[i + 1]--;
			}
		}
		
		string ans = "";
		bool nz = false;
		for (int i = maxD * 2 - 1; i >= 0; i--) {
			if (!nz && !digits2[i]) {
				continue;
			}
			nz |= digits2[i];
			ans += allDigits[digits2[i]];
		}
		while (ans.size() < D) {
			ans += '0';
		}
		return ans;
	}
	
	void generateRandomBits(int D) {
		hi = genLongRand();
		lo = genLongRand();
		
		if (D <= maxD) {
			hi = 0;
			lo &= ((1LL << D) - 1);
		} else {
			hi &= ((1LL << (D - maxD)) - 1);
		}
	}
	
	long long getRemainder(long long N, int B) {
		long long ans = 0;
		for (int i = maxD - 1; i >= 0; i--) {
			ans *= B;
			if (hi & (1LL << i)) ans++;
			ans %= N;
		}
		for (int i = maxD - 1; i >= 0; i--) {
			ans *= B;
			if (lo & (1LL << i)) ans++;
			ans %= N;
		}
		return ans;
		return ans;
	}
	
	long long genLongRand() {
		long long ans = 0;
		for (int i = 0; i < maxD / maxV; i++) {
			int bits = rand() % ((1 << maxV) - 1);
			ans <<= maxV;
			ans |= bits;
		}
		return ans;
	}
};
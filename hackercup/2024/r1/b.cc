#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#define maxn 10000005
using namespace std;

int T, n;
int dp[maxn];
bool primes[maxn];

int main() {
	memset(primes, true, sizeof primes);
	primes[0] = primes[1] = false;
	for (int i = 2; i < maxn; i++) if (primes[i]) {
		for (int j = i + i; j < maxn; j += i) {
			primes[j] = false;
		}
	}

	for (int i = 2; i < maxn; i++) {
		dp[i] = dp[i - 1];
		if (primes[i] && primes[i - 2]) {
			dp[i]++;
		}
		// special case for p = 2, i = 5
		if (i == 5) {
			dp[i]++;
		}
	}
	
	cin >> T;
	for (int it = 1; it <= T; it++) {
		cin >> n;
		printf("Case #%d: %d\n", it, dp[n]);
	}
	return 0;
}
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#define mod 998244353
using namespace std;

int T, n;
long long G, W, L;

long long solve() {
	cin >> W >> G >> L;
	long long l_value = (2LL * L + 1) % mod;
	long long dist = (W - G) % mod;
	return (dist * l_value) % mod;
}

int main() {
	cin >> T;
	for (int it = 1; it <= T; it++) {
		printf("Case #%d: %lld\n", it, solve());
	}
	return 0;
}
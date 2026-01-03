#include <bits/stdc++.h>
using namespace std;

int diff[55];

struct XorGame {
	int play(string a, string b) {
		int n = a.size();

		bool allZero = true;
		for (int i = 0; i < n; i++) {
			diff[i] = (a[i] != b[i]);
			if (diff[i]) {
				allZero = false;
			}
		}

		if (allZero) {
			return 0;
		}

		long long mask = 0;
		for (int i = 0; i < n; i++) if (diff[i]) {
			mask |= 1LL << i;
		}

		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j <= n; j++) {
				if (mask == (1LL << j) - (1LL << i)) {
					return 1;
				}
			}
		}

		if (n >= 5) {
			return -1;
		}

		// 1101 or 1011
		if (mask == 11 || mask == 13) {
			return 3;
		}

		return 2;
	}
};
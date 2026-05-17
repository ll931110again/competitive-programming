// Codeforces 755 (8VC Venture Cup 2017 - Elimination Round) — A. PolandBall and Hypothesis
// Submission: https://codeforces.com/contest/755/submission/23847229

#include <bits/stdc++.h>
#define MAXK 1000001
using namespace std;

bool is_prime[MAXK + 2];
int n;

int main() {
	memset(is_prime, true, sizeof is_prime);
	is_prime[0] = is_prime[1] = false;

	for (int i = 2; i <= MAXK; i++) if (is_prime[i]) {
		for (int j = i + i; j <= MAXK; j += i) {
			is_prime[j] = false;
		}
	}

	scanf("%d", &n);
	for (int m = 1; m <= 1000; m++) {
		if (!is_prime[n * m + 1]) {
			printf("%d\n", m);
			return 0;
		}
	}
	return 0;
}

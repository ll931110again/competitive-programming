#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#define maxn 1000005
using namespace std;

int T, n;
vector<long long> nums;

long long solve() {
	long long A, B, M;
	cin >> A >> B >> M;

	long long ret = 0;
	for (auto num : nums) {
		if (num >= A && num <= B && num % M == 0) {
			ret++;
		}
	}

	return ret;
}

void rec(int pos, int d, long long val, int last_digit) {
	if (pos >= d) {
		nums.push_back(val);
		return;
	}

	for (int next_digit = 1; next_digit < 10; next_digit++) {
		if ((pos == d/2 || pos == d/2 + 1) && (next_digit == last_digit)) {
			continue;
		}

		if (pos <= d/2 && next_digit < last_digit) {
			continue;
		}

		if (pos > d/2 && next_digit > last_digit) {
			continue;
		}

		rec(pos + 1, d, val * 10 + next_digit, next_digit);
	}
}

void generate_candidates() {
	for (int d = 1; d < 19; d += 2) {
		rec(0, d, 0, -1);
	}
}

int main() {
	generate_candidates();

	cin >> T;
	for (int it = 1; it <= T; it++) {
		printf("Case #%d: %lld\n", it, solve());
	}
	return 0;
}
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#define maxn 1000005
using namespace std;

int T, n;
pair<int,int> ts[maxn];

double solve() {
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d %d", &ts[i].first, &ts[i].second);
	}

	double lower_bound = 0, upper_bound = 1e9;
	for (int i = 0; i < n; i++) {
		// distance is (i + 1)
		lower_bound = max(lower_bound, 1.0 * (i + 1) / ts[i].second);
		if (ts[i].first > 0) {
			upper_bound = min(upper_bound, 1.0 * (i + 1) / ts[i].first);
		}
	}

	if (lower_bound > upper_bound + 1e-6) {
		return -1;
	}
	return lower_bound;
}

int main() {
	cin >> T;
	for (int it = 1; it <= T; it++) {
		printf("Case #%d: %.9lf\n", it, solve());
	}
	return 0;
}
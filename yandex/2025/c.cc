#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 105
using namespace std;

int T;
int n;
long long k;
long long fib[maxn];
long long inf = 1e18;
int fib_count;

int opt_value;
vector<int> pos, ans;

void rec(long long prod, int sum_value, int idx) {
	if (idx > fib_count) {
		return;
	}

	if (prod == 1) {
		if ((sum_value != n - 1) && (sum_value < opt_value)) {
			opt_value = sum_value;
			ans = pos;
		}
		return;
	}

	rec(prod, sum_value, idx + 1);
	if (prod % fib[idx] == 0) {
		pos.push_back(idx);
		int _sum_value = sum_value + idx;
		if (sum_value) {
			_sum_value++;
		}
		rec(prod / fib[idx], _sum_value, idx);
		pos.pop_back();
	}
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

	fib[1] = fib[2] = 1;
	for (int i = 3; ; i++) {
		fib[i] = fib[i - 1] + fib[i - 2];
		if (fib[i] >= inf) {
			fib_count = i;
			break;
		}
	}

	T = 1;
	while (T--) {
		cin >> n >> k;
		opt_value = n + 1;
		pos.clear();
		ans.clear();
		rec(k, 0, 3);

		if (opt_value > n) {
			cout << -1 << endl;
			continue;
		}

		vector<int> skipped;
		int sum_value = 0;
		for (int i = 0; i < ans.size(); i++) {
			if (i > 0) {
				sum_value++;
				skipped.push_back(sum_value);
			}
			sum_value += ans[i];
		}

		while (sum_value < n) {
			if (sum_value == 0) {
				sum_value = 1;
			}

			if (sum_value + 1 < n) {
				skipped.push_back(sum_value + 1);
			}
			sum_value += 2;
		}

		cout << skipped.size() << endl;
		if (!skipped.empty()) {
			for (auto x : skipped) {
				cout << x << ' ';
			}
			cout << endl;
		}
	}

    return 0;
}
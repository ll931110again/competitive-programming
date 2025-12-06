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
#define maxn 200005
using namespace std;

int n;
long long a[maxn];
int delta = 200;

const long long GENERATOR_A = 134775813;
const long long GENERATOR_C = 1;

const long long X = 100000000;
const long long DENOMINATOR = 1LL << 32;

bool can_generate(long long candidate) {
	long long state = candidate;
	if (state * X / DENOMINATOR != a[0]) {
		return false;
	}
	for (int i = 1; i < n; i++) {
		state = (state * GENERATOR_A + GENERATOR_C) % DENOMINATOR;
		long long ax = (state * X / DENOMINATOR);
		if (ax != a[i]) {
			return false;
		}
	}
	return true;
}

bool solve() {
	long long s = a[0] * DENOMINATOR / X;
	for (long long candidate = s - delta; candidate <= s + delta; candidate++) if (candidate >= 0 && candidate < DENOMINATOR) {
		if (can_generate(candidate)) {
			return true;
		}
	}
	return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

	cin >> n;
	n *= 2;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}

	cout << (solve() ? "RAW" : "SHUFFLED") << endl;
    return 0;
}
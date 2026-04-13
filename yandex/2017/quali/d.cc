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
#define maxk 60
using namespace std;

int T;
long long n;

bool found_solution(long long n, int k) {
	long long value = n * k;

	priority_queue<int> pq;
	for (int i = 0; i < maxk; i++) if (value & (1LL << i)) {
		pq.push(i);
	}

	if (pq.size() > k) {
		return false;
	}

	while (pq.size() < k) {
		int u = pq.top();
		pq.pop();

		pq.push(u - 1);
		pq.push(u - 1);
	}

	cout << k << ' ';
	while (!pq.empty()) {
		int u = pq.top();
		pq.pop();
		cout << u << ' ';
	}
	cout << endl;
	return true;
}

void solve(long long n) {
	for (int k = 1; ; k++) {
		if (found_solution(n, k)) {
			return;
		}
	}
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
    	cin >> n;
    	solve(n);
    }

    return 0;
}
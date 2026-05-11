// Codeforces 1656 (CodeTON Round 1 (Div. 1 + Div. 2, Rated, Prizes!)) — B. Subtract Operation
// Submission: https://codeforces.com/contest/1656/submission/150734646

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <cstdio>
#include <queue>
#include <stack>
#include <vector>
#include <unordered_set>
#define maxn 200005
using namespace std;

int T, n;
long long K;
unordered_set<long long> elems;

int main() {
	scanf("%d", &T);
	while (T--) {
		bool ok = false;
		elems.clear();
		scanf("%d %lld", &n, &K);
		for (int i = 0; i < n; i++) {
			long long x;
			scanf("%lld", &x);
			if (elems.count(x + K) || elems.count(x - K)) {
				ok = true;
			}
			elems.insert(x);
		}
		if (ok) {
			printf("YES\n");
		} else {
			printf("NO\n");
		}
	}
	return 0;
}

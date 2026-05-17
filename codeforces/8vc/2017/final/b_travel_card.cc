// Codeforces 756 (8VC Venture Cup 2017 - Final Round) — B. Travel Card
// Submission: https://codeforces.com/contest/756/submission/45384891

#include <bits/stdc++.h>
#include <iomanip>
#include <list>
#include <numeric>
#define maxn 100005
using namespace std;

int n, ts[maxn], dp[maxn];
int prices[3] = {20, 50, 120};
int timediff[3] = {0, 90, 1440};
int idx[3] = {0, 0, 0};

int main() {
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &ts[i]);
	}

	for (int i = 1; i <= n; i++) {
		dp[i] = dp[i - 1] + prices[0];
		for (int j = 1; j < 3; j++) {
			while (ts[i] - ts[idx[j] + 1] >= timediff[j]) {
				idx[j]++;
			}
			dp[i] = min(dp[i], dp[idx[j]] + prices[j]);
		}
		printf("%d\n", dp[i] - dp[i - 1]);
	}

	return 0;
}

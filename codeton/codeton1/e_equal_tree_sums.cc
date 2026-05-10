// Codeforces 1656 (CodeTON Round 1 (Div. 1 + Div. 2, Rated, Prizes!)) — E. Equal Tree Sums
// Submission: https://codeforces.com/contest/1656/submission/151134737

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <cstdio>
#include <queue>
#include <stack>
#include <vector>
#define maxn 100005
using namespace std;

int T, n;
vector<int> adj[maxn];
int ans[maxn];

void DFS(int u, int p, int w) {
	ans[u] = w * adj[u].size();
	for (auto v : adj[u]) if (v != p) {
		DFS(v, u, -w);
	}
}

int main() {
	scanf("%d", &T);
	while (T--) {
		scanf("%d", &n);
		for (int i = 1; i <= n; i++) {
			adj[i].clear();
		}
		for (int i = 1; i < n; i++) {
			int u, v;
			scanf("%d %d", &u, &v);
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		DFS(1, -1, 1);
		for (int i = 1; i <= n; i++) {
			printf("%d ", ans[i]);
		}
		printf("\n");
	}
	return 0;
}

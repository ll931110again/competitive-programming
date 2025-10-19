#include <bits/stdc++.h>
#define maxn 200005
#define maxk 20
using namespace std;
 
int T, n;
int dp[maxn][maxk];
int par[maxn];
vector<int> adj[maxn];
int deg[maxn];
 
void solve(int it) {
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		adj[i].clear();
	}
 
	memset(deg, 0, sizeof deg);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &par[i]);
		if (par[i]) {
			adj[par[i]].push_back(i);
			deg[par[i]]++;
		}
	}
 
	queue<int> q;
	for (int i = 1; i <= n; i++) if (!deg[i]) {
		q.push(i);
	}
 
	while (!q.empty()) {
		int u = q.front();
		q.pop();
 
		for (int c = 1; c < maxk; c++) {
			dp[u][c] = c;
			for (auto v : adj[u]) {
				int minCost = 1 << 30;
				for (int d = 1; d < maxk; d++) if (d != c) {
					minCost = min(minCost, dp[v][d]);
				}
				dp[u][c] += minCost;
			}
		}
 
		deg[par[u]]--;
		if (par[u] > 0 && deg[par[u]] == 0) {
			q.push(par[u]);
		}
	}
 
	int ret = dp[1][1];
	for (int i = 2; i < maxk; i++) {
		ret = min(ret, dp[1][i]);
	}
	printf("Case #%d: %d\n", it, ret);
}
 
int main() {
	scanf("%d", &T);
	for (int it = 1; it <= T; it++) {
		solve(it);
	}
	return 0;
}

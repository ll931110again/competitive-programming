// Codeforces 755 (8VC Venture Cup 2017 - Elimination Round) — C. PolandBall and Forest
// Submission: https://codeforces.com/contest/755/submission/23854630

#include <bits/stdc++.h>
#define MAXN 10005
using namespace std;

int n;
int a[MAXN];
vector<int> adj[MAXN];
bool mark[MAXN];

void DFS(int u) {
	for (auto v : adj[u]) if (!mark[v]) {
		mark[v] = true;
		DFS(v);
	}
}

int main() {
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &a[i]);
		if (a[i] != i) {
			adj[i].push_back(a[i]);
			adj[a[i]].push_back(i);
		}
	}
	int cnt = 0;
	for (int i = 1; i <= n; i++) if (!mark[i]) {
		mark[i] = true;
		DFS(i);
		cnt++;
	}
	printf("%d\n", cnt);
	return 0;
}

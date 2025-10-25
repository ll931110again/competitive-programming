#include <algorithm>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#define maxn 200005
#define maxk 20
#define maxd 10000005
using namespace std;

int T, n;
int value[maxn];
int ans[maxn];
vector<int> adj[maxn];

int divisor[maxd];

int dist[maxn];
int aentry[maxn], aexit[maxn];
int counter = 0;
int par[maxn][maxk];

struct LCA {
	void DFS(int u, int p) {
		aentry[u] = counter++;
		for (auto v : adj[u]) {
			if (v == p) {
				continue;
			}

			par[v][0] = u;
			dist[v] = 1 + dist[u];

			DFS(v, u);
		}
		aexit[u] = counter++;
	}

	bool isParent(int u, int v) {
		return aentry[u] <= aentry[v] && aexit[v] <= aexit[u];
	}

	void build() {
		counter = 0;
		dist[1] = 0;
		par[1][0] = -1;
		DFS(1, -1);

		for (int j = 1; j < maxk; j++) {
			for (int u = 1; u <= n; u++) {
				par[u][j] = -1;
				int p = par[u][j - 1];
				if (p >= 0) {
					par[u][j] = par[p][j - 1];
				}
			}
		}
	}

	int lca(int u, int v) {
		if (isParent(u, v)) {
			return u;
		}
		if (isParent(v, u)) {
			return v;
		}

		for (int j = maxk - 1; j >= 0; j--) {
			if (par[u][j] >= 0 && !isParent(par[u][j], v)) {
				u = par[u][j];
			}
		}

		return par[u][0];
	}

	int getDist(int u, int v) {
		int l = lca(u, v);
		return dist[u] + dist[v] - 2 * dist[l];
	}
};

void solve() {
	cin >> n;
	for (int i = 1; i <= n; i++) {
		adj[i].clear();
		ans[i] = -1;
	}

	for (int i = 1; i <= n; i++) {
		cin >> value[i];
	}

	for (int i = 1; i < n; i++) {
		int u, v;
		cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	LCA rep;
	rep.build();

	unordered_map<int, vector<int> > nodes;
	for (int i = 1; i <= n; i++) {
		int x = value[i];
		while (x > 1) {
			int y = divisor[x];
			nodes[y].push_back(i);
			x /= y;
		}
	}

	for (auto it : nodes) {
		auto group = it.second;

		// find the diameter of the group
		auto u = group[0], v = group[0];
		int maxDist = -1;
		for (auto c : group) {
			int lc = rep.getDist(u, c);
			if (lc > maxDist) {
				maxDist = lc;
				v = c;
			}
		}
		maxDist = -1;
		for (auto c : group) {
			int lc = rep.getDist(c, v);
			if (lc > maxDist) {
				maxDist = lc;
				u = c;
			}
		}

		// Now for each node, update distance against the diameter
		for (auto c : group) {
			ans[c] = max(ans[c], rep.getDist(u, c));
			ans[c] = max(ans[c], rep.getDist(v, c));
		}
	}

	for (int i = 1; i <= n; i++) {
		cout << ans[i] << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	for (int i = 2; i < maxd; i++) if (!divisor[i]) {
		for (int j = i; j < maxd; j += i) {
			divisor[j] = i;
		}
	}

	cin >> T;
	while (T--) {
		solve();
	}
	return 0;
}
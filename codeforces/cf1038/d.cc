#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 5005
using namespace std;

int T;
int m, n;
vector<int> adj[maxn];

int cur[maxn], nxt[maxn];
int inf = 1e9;

void solve() {
    for (int i = 1; i <= n; i++) {
        cur[i] = nxt[i] = inf;
    }
    cur[1] = 0;

    int K = 0;
    while (true) {
        K++;
        for (int i = 1; i <= n; i++) {
            nxt[i] = inf;
        }
        for (int i = 1; i <= n; i++) if (cur[i] < inf) {
            nxt[i] = min(nxt[i], cur[i] + 1);
            int v = adj[i][(K - 1) % adj[i].size()];
            nxt[v] = min(nxt[v], cur[i]);
        }
        for (int i = 1; i <= n; i++) {
            cur[i] = nxt[i];
        }

        if (nxt[n] < inf) {
            cout << K << ' ' << nxt[n] << endl;
            return;
        }
    }
}

int main() {
    cin >> T;
    while (T--) {
        cin >> n >> m;
        for (int i = 1; i <= n; i++) {
            adj[i].clear();
        }
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        solve();
    }
    return 0;
}
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
#define maxn 200005
using namespace std;
 
int T, n, m;
vector<int> adj[maxn];
int level[maxn];
 
bool vis[maxn];
 
bool chk[maxn];
int vis_count = 0;
 
pair<int,int> orders[maxn];
 
bool BFS(int u) {
    vis_count = 0;
    for (int j = 1; j <= n; j++) {
        chk[j] = false;
    }
 
    priority_queue< pair<int,int> > pq;
    pq.push({0, u});
    vis[u] = chk[u] = true;
 
    while (!pq.empty()) {
        int u = pq.top().second;
        if (level[u] > vis_count) {
            break;
        }
        pq.pop();
        vis[u] = true;
        vis_count++;
 
        for (auto v : adj[u]) if (!chk[v]) {
            chk[v] = true;
            pq.push({-level[v], v});
        }
    }
 
    return vis_count == n;
}
 
bool solve() {
    for (int i = 1; i <= n; i++) {
        vis[i] = false;
    }
 
    for (int i = 1; i <= n; i++) {
        int u = orders[i].second;
        if (orders[i].first || vis[u]) {
            continue;
        }
 
        if (BFS(u)) {
            return true;
        }
    }
    
    return false;
}
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    while (T--) {
        cin >> n >> m;
        for (int i = 1; i <= n; i++) {
            adj[i].clear();
        }
 
        for (int i = 1; i <= n; i++) {
            cin >> level[i];
            orders[i] = {level[i], i};
        }
        sort(orders + 1, orders + n + 1);
 
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
 
        cout << (solve() ? "YES" : "NO") << endl;
    }
    
    return 0;
}

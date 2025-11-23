#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif
 
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <unordered_set>
#define maxn 200005
#define maxk 20
using namespace std;
 
int T, n, K;
int color[maxn];
int weight[maxn];
 
vector<int> adj[maxn];
 
int ins[maxn], outs[maxn], cnt = 0;
int par[maxn][maxk];
 
void DFS(int u, int p) {
    par[u][0] = p;
    ins[u] = cnt++;
    for (auto v : adj[u]) if (v != p) {
        DFS(v, u);
    }
    outs[u] = cnt++;
}
 
bool is_parent(int u, int v) {
    return ins[u] < ins[v] && outs[v] < outs[u];
}
 
int lca(int u, int v) {
    if (is_parent(u, v)) {
        return u;
    }
    if (is_parent(v, u)) {
        return v;
    }
 
    for (int j = maxk - 1; j >= 0; j--) {
        int tmp = par[u][j];
        if (tmp >= 0 && !is_parent(tmp, v)) {
            u = tmp;
        }
    }
 
    return par[u][0];
}
 
void assign_down(int u, int p) {
    for (auto v : adj[u]) if (v != p) {
        assign_down(v, u);
    }
 
    if (!color[u]) {
        for (auto v : adj[u]) if (v != p && color[v]) {
            color[u] = color[v];
            break;
        }
    }
}
 
void assign_up(int u, int p) {
    for (auto v : adj[u]) if (v != p) {
        if (!color[v]) {
            color[v] = color[u];
        }
        assign_up(v, u);
    }
}
 
void solve() {
    cnt = 0;
    DFS(1, -1);
 
    for (int j = 1; j < maxk; j++) {
        for (int i = 1; i <= n; i++) {
            par[i][j] = par[i][j - 1];
            if (par[i][j] > 0) {
                par[i][j] = par[par[i][j - 1]][j - 1];
            }
        }
    }
 
    unordered_map<int, vector<int> > orders;
    for (int i = 1; i <= n; i++) if (color[i]) {
        orders[color[i]].push_back(i);
    }
 
    unordered_map<int, unordered_set<int> > color_set;
    for (auto it : orders) {
        int c = it.first;
        auto nodes = it.second;
        sort(nodes.begin(), nodes.end(), [&](int u, int v) {
            return ins[u] < ins[v];
        });
 
        for (int i = 0; i < nodes.size(); i++) {
            color_set[nodes[i]].insert(c);
            if (i + 1 < nodes.size()) {
                int l = lca(nodes[i], nodes[i + 1]);
                color_set[l].insert(c);
            }
        }
    }
 
    long long ans = 0;
    for (auto it : color_set) {
        if (it.second.size() > 1) {
            ans += weight[it.first];
        }
    }
 
    for (auto it : color_set) {
        if (color[it.first] == 0 && it.second.size() == 1) {
            color[it.first] = *it.second.begin();
        }
    }
 
    assign_down(1, -1);
    if (!color[1]) {
        color[1] = 1;
    }
    assign_up(1, -1);
 
    cout << ans << endl;
    for (int i = 1; i <= n; i++) {
        cout << color[i] << ' ';
    }
    cout << endl;
}
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
 
    cin >> T;
    while (T--) {
        cin >> n >> K;
        for (int i = 1; i <= n; i++) {
            adj[i].clear();
        }
 
        for (int i = 1; i <= n; i++) {
            cin >> weight[i];
        }
 
        for (int i = 1; i <= n; i++) {
            cin >> color[i];
        }
 
        for (int i = 1; i < n; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
 
        solve();
    }
    return 0;
}

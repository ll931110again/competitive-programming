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
#include <unordered_set>
#define maxn 1005
#define maxk 4005
using namespace std;

int T, n;
int a[maxn][maxn];

int cost[maxk];
unordered_set<int> adj[maxk];
int cnt[maxk], low[maxk], nodes = 0;

int rep[maxk], scc = 0;
long long sum_sc[maxk];
stack<int> ss;
unordered_set<int> adj_sc[maxk], rev_sc[maxk];
int deg_sc[maxk];

void DFS(int u) {
    s.push(u);
    low[u] = cnt[u] = nodes++;
    
    for (auto v : adj[u]) {
        if (cnt[v] < 0) {
            DFS(v);
            low[u] = min(low[u], cnt[v]);
        } else {
            low[u] = min(low[u], low[v]);
        }
    }

    if (low[u] == cnt[u]) {
        while (true) {
            int x = s.top();
            s.pop();

            rep[x] = scc;
            sum_sc[scc] += cost[x];

            if (x == u) {
                break;
            }
        }
    }
}

long long solve() {
    int K = 4 * n;
    for (int i = 0; i < K; i++) {
        adj[i].clear();
    }

    nodes = 0;
    memset(cnt, -1, sizeof cnt);
    memset(low, -1, sizeof low);
    
    for (int i = 0; i < n; i++) {
        for (int d = -1; d <= 1; d += 2) {
            int j = i + d;
            if (j < 0 || j >= n) {
                continue;
            }

            for (int t = 0; t < n; t++) {
                if (a[i][t] == a[j][t]) {
                    adj[2 * i].insert(2 * j + 1);
                    adj[2 * j + 1].insert(2 * i);
                } else if (a[i][t] == a[j][t] + 1) {
                    adj[2 * i + 1].insert(2 * j + 1);
                }
            }

            for (int t = 0; t < n; t++) {
                if (a[t][i] == a[t][j]) {
                    adj[2 * n + 2 * i].insert(2 * n + 2 * j + 1);
                    adj[2 * n + 2 * j + 1].insert(2 * n + 2 * i);
                } else if (a[t][i] == a[t][j] + 1) {
                    adj[2 * n + 2 * i + 1].insert(2 * n + 2 * j + 1);
                }
            }
        }
    }

    memset(rep, -1, sizeof rep);
    scc = 0;
    memset(sum_sc, 0, sizeof sum_sc);
    while (!ss.empty()) {
        ss.pop();
    }
    
    for (int i = 0; i < K; i++) {
        if (low[i] >= 0) {
            continue;
        }
        DFS(i);
    }

    for (int i = 0; i < K; i++) {
        if (rep[i] == rep[i ^ 1]) {
            return -1;
        }
    }

    memset(deg_sc, 0, sizeof deg_sc);
    for (int i = 0; i < scc; i++) {
        adj_sc[i].clear();
        rev_sc[i].clear();
    }

    for (int u = 0; u < K; u++) {
        for (auto v : adj[u]) {
            if (rep[u] != rep[v]) {
                adj_sc[rep[u]].insert(rep[v]);
                rev_sc[rep[v]].insert(rep[u]);
            }
        }
    }

    queue<int> qs;
    for (int i = 0; i < scc; i++) {
        deg_sc[i] = adj_sc[i].size();
        if (deg_sc[i] == 0) {
            qs.push(i);
        }
    }

    long long ans = 0;
    while (!qs.empty()) {
        int u = qs.front(), v = -1;
        q.pop();

        for (int i = 0; i < K; i++) if (rep[i] == u) {
            v = rep[i ^ 1];
            break;
        }

        ans += min(sum_sc[u], sum_sc[v]);
        for (auto z : rev_sc[u]) {
            deg_sc[z]--;
            if (deg_sc[z] == 0) {
                qs.push(z);
            }
        }

        for (auto z : rev_sc[v]) {
            deg_sc[z]--;
            if (deg_sc[z] == 0) {
                qs.push(z);
            }
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cin >> a[i][j];
            }
        }

        for (int i = 0; i < n; i++) {
            cin >> cost[2 * i + 1];
        }

        for (int i = 0; i < n; i++) {
            cin >> cost[2 * n + 2 * i + 1];
        }

        cout << solve() << endl;
    }
    return 0;
}
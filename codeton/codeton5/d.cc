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
#define maxn 105
using namespace std;

int n, m;
long long dist[maxn][maxn];
long long inf = 1e18;

int orders[maxn];
vector< pair<string, long long> > ops;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            dist[i][j] = ((i == j) ? 0 : inf);
        }
    }

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        dist[u][v] = dist[v][u] = w;
    }

    for (int k = 1; k <= n; k++) {
        for (int u = 1; u <= n; u++) {
            for (int v = 1; v <= n; v++) {
                dist[u][v] = min(dist[u][v], dist[u][k] + dist[k][v]);
            }
        }
    }

    if (dist[1][n] >= inf) {
        cout << "inf" << endl;
        return 0;
    }

    for (int i = 1; i <= n; i++) {
        orders[i] = i;
    }

    sort(orders + 1, orders + n + 1, [&](int u, int v) {
        return (make_pair(dist[1][u], u) < make_pair(dist[1][v], v));
    });

    string s = "";
    for (int i = 0; i < n; i++) {
        s += '0';
    }

    for (int i = 1; i < n; i++) {
        long long diffs = dist[1][orders[i + 1]] - dist[1][orders[i]];
        s[orders[i] - 1] = '1';

        if (diffs > 0) {
            ops.push_back({s, diffs});
        }

        if (orders[i + 1] == n) {
            break;
        }
    }

    cout << dist[1][n] << ' ' << ops.size() << endl;
    for (auto [s, t] : ops) {
        cout << s << ' ' << t << endl;
    }

    return 0;
}
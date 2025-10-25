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

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            dist[i][j] = (i == j) ? 0 : inf;
        }
    }
    cin >> n >> m;
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
        return (dist[1][u] < dist[1][v]);
    });

    for (int i = 1; i < n; i++) if (dist[1][orders[i]] < dist[1][orders[i + 1]]) {
        long long diffs = dist[1][orders[i + 1]] - dist[1][orders[i]];
        string s = "";
        for (int u = 0; u < n; u++) {
            s += '0';
        }

        for (int j = 1; j <= i; j++) {
            s[orders[j] - 1] = '1';
        }

        ops.push_back({s, diffs});
    }

    cout << dist[1][n] << ' ' << ops.size() << endl;
    for (auto [s, t] : ops) {
        cotu << s << ' ' << t << endl;
    }

    return 0;
}
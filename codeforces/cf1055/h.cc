#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>
#include <queue>
#include <set>
#include <unordered_set>
#include <vector>
#define maxn 250005
using namespace std;
using pi = array<long long, 2>;

struct edge {
    int w, u, v;
};

edge edges[maxn];

int T, n, Q;
int par[2 * maxn];
long long cost[2 * maxn];
pair<int,int> ch[2 * maxn];
long long dp[2 * maxn];

pair<int,int> queries[maxn];
long long ans[maxn];

int get(int u) {
    if (u != par[u]) {
        par[u] = get(par[u]);
    }
    return par[u];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 0; i + 1 < n; i++) {
            cin >> edges[i].u >> edges[i].v >> edges[i].w;
            edges[i].u--;
            edges[i].v--;
        }

        sort(edges, edges + n - 1, [&](auto x, auto y) {
            return x.w > y.w;
        });

        for (int i = 0; i < 2 * n; i++) {
            par[i] = i;
            cost[i] = 0;
        }

        for (int i = 0; i + 1 < n; i++) {
            int u = get(edges[i].u);
            int v = get(edges[i].v);

            par[u] = i + n;
            par[v] = i + n;
            ch[i + n] = {u, v};

            cost[edges[i].u] += edges[i].w;
            cost[edges[i].v] += edges[i].w;
            cost[i + n] -= edges[i].w * 2;
        }

        for (int i = n; i < 2 * n - 1; i++) {
            cost[i] += cost[ch[i].first] + cost[ch[i].second];
        }

        vector< priority_queue< pi, vector<pi>, greater<pi> > > pq(n);
        vector<int> idx(2 * n - 1);
        for (int i = 0; i < 2 * n - 1; i++) {
            idx[i] = i;
        }

        for (int i = 0; i < 2 * n - 1; i++) {
            if (i < n) {
                pq[i].push({cost[i], 1});
                continue;
            }

            if (pq[idx[ch[i].first]].size() < pq[idx[ch[i].second]].size()) {
                swap(ch[i].first, ch[i].second);
            }
            idx[i] = idx[ch[i].first];
            while (!pq[idx[ch[i].second]].empty()) {
                auto x = pq[idx[ch[i].second]].top();
                pq[idx[ch[i].second]].pop();
                pq[idx[i]].push(x);
            }

            if (cost[i] >= pq[idx[i]].top()[0]) {
                continue;
            }
            if (cost[i] >= 2e9) {
                continue;
            }

            long long A = 0, B = 0, pv = -1;
            while (!pq[idx[i]].empty()) {
                auto [point, slope] = pq[idx[i]].top();
                pq[idx[i]].pop();

                B += A * (point - pv);
                pv = point;
                A += slope;

                long long U = A * pv - B - cost[i];
                long long L = A - 1;
                if (L > 0) {
                    if (pq[idx[i]].empty() || U < pq[idx[i]].top()[0] * L) {
                        long long f1 = U / L - cost[i];
                        long long f2 = (U / L + 1 - pv) * A + B;

                        pq[idx[i]].push({cost[i], 1});
                        pq[idx[i]].push({U / L, f2 - f1 - 1});
                        pq[idx[i]].push({U / L + 1, A - (f2 - f1)});
                        break;
                    }
                } else if (pq[idx[i]].empty()) {
                    pq[idx[i]].push({cost[i], 1});
                    break;
                }
            }
        }

        cin >> Q;
        for (int i = 0; i < Q; i++) {
            cin >> queries[i].first;
            queries[i].second = i;
        }
        sort(queries, queries + Q);

        long long A = 0, B = 0, pv = -1;
        int tx = idx[2 * n - 2];
        for (int i = 0; i < Q; i++) {
            int x = queries[i].first, pos = queries[i].second;
            while (!pq[tx].empty() && pq[tx].top()[0] <= x) {
                auto [point, slope] = pq[tx].top();
                pq[tx].pop();

                B += A * (point - pv);
                pv = point;
                A += slope;
            }

            ans[pos] = (x - pv) * A + B;
        }

        for (int i = 0; i < Q; i++) {
            cout << ans[i] << endl;
        }
    }

    return 0;
}
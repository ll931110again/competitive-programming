#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge {
    int to;
    long long pr;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, Q;
    cin >> N >> M >> Q;
    vector<vector<Edge>> adj(N + 1);
    for (int i = 0; i < N - 1; i++) {
        int u, v;
        long long p;
        cin >> u >> v >> p;
        adj[u].push_back({v, p});
        adj[v].push_back({u, p});
    }

    vector<long long> thr(N + 1, -1);
    for (int i = 0; i < M; i++) {
        int j;
        long long s;
        cin >> j >> s;
        thr[j] = s;
    }

    auto walk = [&](int start, long long f) -> int {
        int parent = -1;
        int cur = start;
        while (true) {
            const int deg = (int)adj[cur].size();
            if (deg == 1) {
                if (cur != start) {
                    return cur;
                }
                const int nxt = adj[cur][0].to;
                parent = cur;
                cur = nxt;
                continue;
            }
            if (deg == 2) {
                int nxt = -1;
                for (const Edge &e : adj[cur]) {
                    if (e.to != parent) {
                        nxt = e.to;
                        break;
                    }
                }
                parent = cur;
                cur = nxt;
                continue;
            }
            // junction (degree 3)
            vector<Edge> opts;
            opts.reserve(2);
            for (const Edge &e : adj[cur]) {
                if (e.to != parent) {
                    opts.push_back(e);
                }
            }
            sort(opts.begin(), opts.end(),
                 [](const Edge &a, const Edge &b) { return a.pr < b.pr; });
            const long long s = thr[cur];
            const Edge &pick = (f <= s) ? opts[0] : opts[1];
            parent = cur;
            cur = pick.to;
        }
    };

    for (int qi = 0; qi < Q; qi++) {
        int type;
        cin >> type;
        if (type == 1) {
            int a;
            long long b;
            cin >> a >> b;
            thr[a] = b;
        } else {
            int d;
            long long f;
            cin >> d >> f;
            cout << walk(d, f) << '\n';
        }
    }
    return 0;
}

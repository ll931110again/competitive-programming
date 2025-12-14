#include <bits/stdc++.h>
#define maxn 200005
using namespace std;

int T, m, n;
vector<int> adj[maxn];

int dist[maxn];

void BFS(int u) {
    queue<int> q;
    q.push(u);

    while (!q.empty()) {
        int x = q.front();
        q.pop();

        for (auto y : adj[x]) if (dist[y] < 0) {
            dist[y] = 1 + dist[x];
            q.push(y);
        }
    }
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

        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        
        for (int i = 1; i <= n; i++) {
            dist[i] = -1;
        }

        dist[1] = 0;
        BFS(1);

        int ans = dist[n];
        for (int i = 1; i <= n; i++) {
            for (auto j : adj[i]) {
                if ((dist[j] - dist[i]) % 2 == 0) {
                    ans = -1;
                }
            }
        }

        cout << ans << endl;
    }
	return 0;
}

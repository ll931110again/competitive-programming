#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 20
#define maxk 805
using namespace std;

int T;
int m, n;
vector<string> grid;
vector< pair<int,int> > buckets[2 * maxn];

int src, sink, cnt;
int cap[maxk][maxk], flow[maxk][maxk];
int trace[maxk];
vector<int> adj[maxk];

int get_index(pair<int,int> value) {
    return 2 * (value.first * n + value.second);
}

void add(int u, int v) {
    cap[u][v] = 1;
    adj[u].push_back(v);
    adj[v].push_back(u);
}

int max_flow() {
    int ans = 0;
    while (true) {
        queue<int> q;
        q.push(src);

        memset(trace, -1, sizeof trace);
        trace[src] = -2;

        while (!q.empty() && trace[sink] == -1) {
            int u = q.front();
            q.pop();

            for (auto v : adj[u]) if (cap[u][v] > flow[u][v] && trace[v] == -1) {
                trace[v] = u;
                q.push(v);
            }
        }

        if (trace[sink] == -1) {
            break;
        }

        int v = sink, u = trace[v];
        while (u != src) {
            flow[u][v]++;
            flow[v][u]--;

            v = u;
            u = trace[v];
        }

        ans++;
    }

    return ans;
}

bool ok_bucket(string tmp, int bucket) {
    memset(cap, 0, sizeof cap);
    memset(flow, 0, sizeof flow);
    for (int i = 0; i < maxk; i++) {
        adj[i].clear();
    }

    src = 2 * m * n;
    sink = src + 1;
    cnt = src + 2;
    
    for (auto it : buckets[bucket]) {
        add(src, get_index(it));
    }

    for (int i = 0; i < m * n; i++) {
        add(2 * i, 2 * i + 1);
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            int idx = i + j;
            if (idx < bucket || idx >= bucket + tmp.size()) {
                continue;
            }

            int offset = idx - bucket;
            char ch = grid[i][j];
            if ('A' <= ch && ch <= 'Z') {
                ch = ch - 'A' + 'a';
            }
            
            if (ch < tmp[offset]) {
                continue;
            }

            if (ch > tmp[offset] || offset == tmp.size() - 1) {
                add(get_index({i, j}) + 1, sink);
                continue;
            }

            if (i + 1 < m) {
                add(get_index({i, j}) + 1, get_index({i + 1, j}));
            }

            if (j + 1 < n) {
                add(get_index({i, j}) + 1, get_index({i, j + 1}));
            }
        }
    }

    return (max_flow() == buckets[bucket].size());
}

bool ok(string tmp) {
    for (int bucket = 0; bucket < m + n; bucket++) if (!buckets[bucket].empty()) {
        if (!ok_bucket(tmp, bucket)) {
            return false;
        }
    }
    return true;
}

string solve() {
    string ans = "";
    char first_character = 'Z';
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if ('A' <= grid[i][j] && grid[i][j] <= 'Z') {
                first_character = min(first_character, grid[i][j]);
            }
        }
    }

    for (int i = 0; i < 2 * maxn; i++) {
        buckets[i].clear();
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) if (grid[i][j] == first_character) {
            buckets[i + j].push_back({i, j});
        }
    }

    ans += (first_character - 'A' + 'a');
    while (true) {
        int low = 'a', high = 'z', next_char = -1;
        while (low <= high) {
            char mid = (low + high) / 2;
            string tmp = (ans + mid);
            if (ok(tmp)) {
                next_char = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        if (next_char < 0) {
            break;
        }

        ans += next_char;
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    for (int it = 1; it <= T; it++) {
        cerr << it << endl;
        cin >> m >> n;
        grid.resize(m);
        for (int i = 0; i < m; i++) {
            cin >> grid[i];
        }
        cout << "Case #" << it << ": " << solve() << endl;
    }

    return 0;
}
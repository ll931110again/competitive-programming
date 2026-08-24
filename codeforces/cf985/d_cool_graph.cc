#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>
using namespace std;

struct DSU {
    vector<int> p, r;
    DSU(int n = 0) { init(n); }
    void init(int n) {
        p.resize(n + 1);
        r.assign(n + 1, 0);
        iota(p.begin(), p.end(), 0);
    }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (r[a] < r[b]) swap(a, b);
        p[b] = a;
        if (r[a] == r[b]) r[a]++;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<pair<int, int>> edges;
        edges.reserve(m);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            edges.emplace_back(u, v);
        }

        // Check if the graph is already cool (empty or a tree)
        bool cool = false;
        if (m == 0) {
            cool = true;
        } else if (m == n - 1) {
            DSU dsu(n);
            for (auto [u, v] : edges) dsu.unite(u, v);
            int root = dsu.find(1);
            bool connected = true;
            for (int i = 2; i <= n; i++) {
                if (dsu.find(i) != root) {
                    connected = false;
                    break;
                }
            }
            if (connected) cool = true;
        }

        if (cool) {
            cout << 0 << '\n';
            continue;
        }

        // Phase 1: make all edges incident to vertex 1
        vector<bool> connected_to_1(n + 1, false);
        for (auto [u, v] : edges) {
            if (u == 1) connected_to_1[v] = true;
            else if (v == 1) connected_to_1[u] = true;
        }

        vector<array<int, 3>> ops;
        ops.reserve(m + n);

        for (auto [u, v] : edges) {
            if (u != 1 && v != 1) {
                ops.push_back({1, u, v});
                connected_to_1[u] = !connected_to_1[u];
                connected_to_1[v] = !connected_to_1[v];
            }
        }

        // Build sets S (neighbors of 1) and T (isolated vertices)
        vector<int> S, T;
        for (int v = 2; v <= n; v++) {
            if (connected_to_1[v]) S.push_back(v);
            else T.push_back(v);
        }

        if (S.empty()) {
            // Graph became empty
            cout << ops.size() << '\n';
            for (auto &op : ops) cout << op[0] << ' ' << op[1] << ' ' << op[2] << '\n';
            continue;
        }
        if ((int)S.size() == n - 1) {
            // Star centered at 1 is a tree
            cout << ops.size() << '\n';
            for (auto &op : ops) cout << op[0] << ' ' << op[1] << ' ' << op[2] << '\n';
            continue;
        }

        // Phase 2: grow the tree by attaching isolated vertices to a fixed leaf
        int u = S[0];          // a leaf in the current star
        int parent = 1;        // its current parent
        for (int w : T) {
            ops.push_back({u, parent, w});
            parent = w;        // u's new parent becomes w
        }

        cout << ops.size() << '\n';
        for (auto &op : ops) cout << op[0] << ' ' << op[1] << ' ' << op[2] << '\n';
    }
    return 0;
}
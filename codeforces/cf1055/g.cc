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
#include <set>
#include <unordered_set>
#include <vector>
#define maxn 250005
using namespace std;

int T, n, Q;
int a[maxn];
vector<int> adj[maxn];
int ins[maxn], outs[maxn], cnt = 0;
int states[2 * maxn];

struct node {
    int flipped;
    int chain[4];
};

node tx[8 * maxn];

void DFS(int u, int p) {
    cnt++;
    states[cnt] = (a[u] > 0) ? 2 : 0;
    ins[u] = cnt;
    for (auto v : adj[u]) if (v != p) {
        DFS(v, u);
    }
    cnt++;
    states[cnt] = (a[u] > 0) ? 3 : 1;
    outs[u] = cnt;
}

void merge(int i) {
    for (int j = 0; j < 4; j++) {
        tx[i].chain[j] = tx[2 * i].chain[j];
        if (tx[i].chain[j] % 2 == 0) {
            tx[i].chain[j] += tx[2 * i + 1].chain[j];
        } else {
            tx[i].chain[j] += tx[2 * i + 1].chain[j ^ 1];
        }
    }
}

void init(int i, int low, int high) {
    tx[i].flipped = 0;
    memset(tx[i].chain, 0, sizeof tx[i].chain);
        
    if (low == high) {
        int value = states[low];
        tx[i].chain[value]++;

        return;
    }

    int mid = (low + high) / 2;
    init(2 * i, low, mid);
    init(2 * i + 1, mid + 1, high);
    merge(i);
}

void flip(int i) {
    tx[i].flipped ^= 1;
    int tmp[4];
    tmp[0] = tx[i].chain[2];
    tmp[1] = tx[i].chain[3];
    tmp[2] = tx[i].chain[0];
    tmp[3] = tx[i].chain[1];
    for (int j = 0; j < 4; j++) {
        tx[i].chain[j] = tmp[j];
    }
}

void flag(int i) {
    if (tx[i].flipped == 0) {
        return;
    }
    flip(2 * i);
    flip(2 * i + 1);
    tx[i].flipped = 0;
}

void update(int i, int low, int high, int u, int v) {
    if (v < low || high < u) {
        return;
    }
    if (u <= low && high <= v) {
        flip(i);
        return;
    }

    int mid = (low + high) / 2;
    flag(i);
    update(2 * i, low, mid, u, v);
    update(2 * i + 1, mid + 1, high, u, v);
    merge(i);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++) {
            cin >> a[i];
        }

        for (int i = 1; i <= n; i++) {
            adj[i].clear();
        }

        for (int i = 1; i < n; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        cnt = 0;
        DFS(1, -1);
        init(1, 1, 2 * n);

        cin >> Q;
        cout << (tx[1].chain[2] + 1) / 2 << endl;
        while (Q--) {
            int u;
            cin >> u;
            update(1, 1, 2 * n, ins[u], outs[u]);
            cout << (tx[1].chain[2] + 1) / 2 << endl;
        }
    }

    return 0;
}
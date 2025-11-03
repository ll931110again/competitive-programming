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
#define maxn 100005
using namespace std;

struct Node {
    int max_prefix;
    int delta;
};

Node tx[4 * maxn];
int n;
int value[maxn];

void update(int i, int low, int high, int pos, int value) {
    if (low == high) {
        if (value > 0) {
            tx[i].delta++;
        } else {
            tx[i].delta--;
        }
        tx[i].max_prefix = max(0, tx[i].delta);
        return;
    }

    int mid = (low + high) / 2;
    if (pos <= mid) {
        update(2 * i, low, mid, pos, value);
    } else {
        update(2 * i + 1, mid + 1, high, pos, value);
    }

    tx[i].delta = tx[2 * i].delta + tx[2 * i + 1].delta;
    tx[i].max_prefix = max(tx[2 * i].max_prefix, tx[2 * i].delta + tx[2 * i + 1].max_prefix);
}

int find_prefix(int i, int low, int high, int accum) {
    if (accum + tx[i].max_prefix <= 0) {
        return -1;
    }
    if (low == high) {
        return low;
    }

    int mid = (low + high) / 2;
    int value_left = find_prefix(2 * i, low, mid, accum);
    if (value_left >= 0) {
        return value_left;
    }
    return find_prefix(2 * i + 1, mid + 1, high, accum + tx[2 * i].delta);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 0; i < n; i++) {
        int pos, op, x = -1;
        cin >> pos >> op;
        if (op == 1) {
            cin >> x;
        }
        pos = n + 1 - pos;
        value[pos] = x;
        update(1, 1, n, pos, x);

        pos = find_prefix(1, 1, n, 0);
        int ans = (pos >= 0) ? value[pos] : -1;
        cout << ans << endl;
    }

    return 0;
}
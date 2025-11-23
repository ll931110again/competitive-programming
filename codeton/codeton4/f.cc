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
#include <vector>
#define maxn 200055
#define MAXD 50
using namespace std;
 
int T, n, m, Q;
int a[maxn], bits[maxn];
 
int tx[4 * maxn], carrier[4 * maxn];
 
// strips[i][0] corresponds to value 0
// strips[i][1] corresponds to value m - 1
int strips[4 * maxn][2];
int msb[4 * maxn];
 
bool is_all_zero = true;
 
void set_value(int i, int mid) {
    if (tx[2 * i] < 0 || tx[2 * i + 1] < 0) {
        tx[i] = -1;
    } else {
        tx[i] = (tx[2 * i] == tx[2 * i + 1]) ? tx[2 * i] : -1;
    }
 
    msb[i] = (msb[2 * i + 1] >= 0) ? msb[2 * i + 1] : msb[2 * i];
    for (int j = 0; j < 2; j++) {
        strips[i][j] = (strips[2 * i][j] >= mid) ? max(strips[2 * i][j], strips[2 * i + 1][j]) : strips[2 * i][j];
    }
}
 
int get_index(int val) {
    if (val == 0) {
        return 0;
    }
    if (val == m - 1) {
        return 1;
    }
    return -1;
}
 
void build(int i, int low, int high) {
    tx[i] = carrier[i] = -1;
    if (low == high) {
        tx[i] = bits[low];
 
        int idx = get_index(bits[low]);
        for (int j = 0; j < 2; j++) {
            strips[i][j] = (j == idx) ? low : -1;
        }
        msb[i] = (bits[low]) ? low : -1;
        return;
    }
 
    int mid = (low + high) / 2;
    build(2 * i, low, mid);
    build(2 * i + 1, mid + 1, high);
    set_value(i, mid);
}
 
void flag(int i, int low, int high, int mid) {
    if (carrier[i] >= 0) {
        int val = carrier[i];
        tx[2 * i] = tx[2 * i + 1] = val;
        carrier[2 * i] = carrier[2 * i + 1] = val;
        carrier[i] = -1;
 
        int idx = get_index(val);
        for (int j = 0; j < 2; j++) {
            strips[2 * i][j] = (j == idx) ? mid : -1;
            strips[2 * i + 1][j] = (j == idx) ? high : -1;
        }
 
        msb[2 * i] = (val) ? mid : -1;
        msb[2 * i + 1] = (val) ? high : -1;
    }
}
 
int get(int i, int low, int high, int pos) {
    if (low == high) {
        return tx[i];
    }
 
    int mid = (low + high) / 2;
    flag(i, low, high, mid);
    return (pos <= mid) ? get(2 * i, low, mid, pos) : get(2 * i + 1, mid + 1, high, pos);
}
 
void update(int i, int low, int high, int u, int v, int val) {
    if (v < low || high < u) {
        return;
    }
 
    if (u <= low && high <= v) {
        tx[i] = carrier[i] = val;
 
        int idx = get_index(val);
        for (int j = 0; j < 2; j++) {
            strips[i][j] = (j == idx) ? high : -1;
        }
 
        msb[i] = (val) ? high : -1;
        return;
    }
 
    int mid = (low + high) / 2;
    flag(i, low, high, mid);
    update(2 * i, low, mid, u, v, val);
    update(2 * i + 1, mid + 1, high, u, v, val);
    set_value(i, mid);
}
 
int max_right(int i, int low, int high, int lower_bound, int val) {
    if (high < lower_bound) {
        return lower_bound;
    }
    if (lower_bound <= low) {
        int idx = get_index(val);
        return strips[i][idx];
    }
    if (tx[i] >= 0) {
        return (tx[i] == val) ? high : lower_bound;
    }
 
    int mid = (low + high) / 2;
    flag(i, low, high, mid);
    int lsp = max_right(2 * i, low, mid, lower_bound, val);
    if (lsp < mid) {
        return lsp;
    }
    int rsp = max_right(2 * i + 1, mid + 1, high, lower_bound, val);
    return max(lsp, rsp);
}
 
void add(int pos) {
    int px = get(1, 0, maxn - 1, pos);
    if (px < m - 1) {
        update(1, 0, maxn - 1, pos, pos, px + 1);
        return;
    }
    int rs = max_right(1, 0, maxn - 1, pos, m - 1);
    rs = max(rs, pos);
    update(1, 0, maxn - 1, pos, rs, 0);
 
    px = get(1, 0, maxn - 1, rs + 1);
    update(1, 0, maxn - 1, rs + 1, rs + 1, px + 1);
}
 
void subtract(int pos) {
    int px = get(1, 0, maxn - 1, pos);
    if (px > 0) {
        update(1, 0, maxn - 1, pos, pos, px - 1);
        return;
    }
 
    int rs = max_right(1, 0, maxn - 1, pos, 0);
    rs = max(rs, pos);
    update(1, 0, maxn - 1, pos, rs, m - 1);
 
    px = get(1, 0, maxn - 1, rs + 1);
    update(1, 0, maxn - 1, rs + 1, rs + 1, px - 1);
}
 
void get_range(int i, int low, int high, int u, int v) {
    if (v < low || high < u) {
        return;
    }
    if (u <= low && high <= v) {
        if (tx[i] != 0) {
            is_all_zero = false;
        }
        return;
    }
    int mid = (low + high) / 2;
    flag(i, low, high, mid);
    get_range(2 * i, low, mid, u, v);
    get_range(2 * i + 1, mid + 1, high, u, v);
}
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
 
    cin >> T;
    while (T--) {
        cin >> n >> m >> Q;

        int H = 0;

        for (int i = 0; i < n; i++) {
            cin >> a[i];
            bits[a[i]]++;
            H = max(H, a[i]);
        }
 
        for (int i = 0; i < H + MAXD && i < maxn; i++) if (bits[i] >= m) {
            bits[i + 1] += bits[i] / m;
            H = max(H, i + 1);
            bits[i] %= m;
        }
 
        build(1, 0, maxn - 1);
 
        while (Q--) {
            int pos, val;
            cin >> pos >> val;
 
            pos--;
            subtract(a[pos]);
            add(val);
            a[pos] = val;
 
            is_all_zero = true;
            int x = msb[1];
 
            get_range(1, 0, maxn - 1, 0, x - 1);
            if (!is_all_zero) {
                x++;
            }
 
            cout << x << ' ';
        }
 
        cout << endl;

        for (int i = 0; i <= H; i++) {
            bits[i] = 0;
        }
    }
    return 0;
}

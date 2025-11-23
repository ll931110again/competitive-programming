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
using namespace std;

int n, Q;
int a[maxn], bits[maxn];

int tx[4 * maxn], carrier[4 * maxn];
int strips[4 * maxn][2];
int msb[4 * maxn];

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

void build(int i, int low, int high) {
    if (low == high) {
        tx[i] = bits[low];
        strips[i][bits[low]] = low;
        strips[i][bits[low] ^ 1] = -1;
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

        strips[2 * i][val] = mid;
        strips[2 * i + 1][val] = high;
        strips[2 * i][val ^ 1] = strips[2 * i + 1][val ^ 1] = -1;

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
        strips[i][val] = high;
        strips[i][val ^ 1] = -1;
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
        return strips[i][val];
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
    if (get(1, 0, maxn - 1, pos) == 0) {
        update(1, 0, maxn - 1, pos, pos, 1);
        return;
    }
    int rs = max_right(1, 0, maxn - 1, pos, 1);
    rs = max(rs, pos);
    update(1, 0, maxn - 1, pos, rs, 0);
    update(1, 0, maxn - 1, rs + 1, rs + 1, 1);
}

void subtract(int pos) {
    if (get(1, 0, maxn - 1, pos) == 1) {
        update(1, 0, maxn - 1, pos, pos, 0);
        return;
    }
    int rs = max_right(1, 0, maxn - 1, pos, 0);
    rs = max(rs, pos);
    update(1, 0, maxn - 1, pos, rs, 1);
    update(1, 0, maxn - 1, rs + 1, rs + 1, 0);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    // Note: a generalization of this problem (from base 2 to base M) appears in
    // https://codeforces.com/contest/1810/problem/F

    /*
     * For each query, the answer is the most significant bit of 2^a_1 + .. + 2^a_n.
     * To handle update of queries, note that:
        * When we add 2^k, we need to either set bit k to 1, or set bit k to 0 and perform carry over.
        * Similar formula applies for subtracting 2^k.

     * The process can be handled by segment tree.
     */

    cin >> n >> Q;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        bits[a[i]]++;
    }

    for (int i = 0; i < maxn; i++) if (bits[i] > 1) {
        bits[i + 1] += bits[i] / 2;
        bits[i] %= 2;
    }

    memset(tx, -1, sizeof tx);
    memset(carrier, -1, sizeof carrier);
    build(1, 0, maxn - 1);

    while (Q--) {
        int pos, val;
        cin >> pos >> val;

        pos--;
        subtract(a[pos]);
        add(val);
        a[pos] = val;

        cout << msb[1] << endl;
    }

    return 0;
}
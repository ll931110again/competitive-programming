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
#define maxn 200005
using namespace std;

int n, k, A;
long long inf = 1e18;

struct Point {
    int x, y, c;
};

Point pts[maxn];
vector<int> buckets[maxn];
long long dp[maxn];

struct Node {
    long long value;
    long long carry;
};

Node tx[5 * maxn];

void flag(int i) {
    tx[2 * i].value += tx[i].carry;
    tx[2 * i + 1].value += tx[i].carry;
    tx[2 * i].carry += tx[i].carry;
    tx[2 * i + 1].carry += tx[i].carry;
    tx[i].carry = 0;
}

void update(int i, int low, int high, int u, int v, int delta) {
    if (v < low || high < u) {
        return;
    }
    if (u <= low && high <= v) {
        tx[i].value += delta;
        tx[i].carry += delta;
        return;
    }
    flag(i);
    int mid = (low + high) / 2;
    update(2 * i, low, mid, u, v, delta);
    update(2 * i + 1, mid + 1, high, u, v, delta);
    tx[i].value = max(tx[2 * i].value, tx[2 * i + 1].value);
}

long long get(int i, int low, int high, int u, int v) {
    if (v < low || high < u) {
        return -inf;
    }
    if (u <= low && high <= v) {
        return tx[i].value;
    }
    flag(i);
    int mid = (low + high) / 2;
    return max(get(2 * i, low, mid, u, v), get(2 * i + 1, mid + 1, high, u, v));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long total_sum = 0;

    cin >> n >> k >> A;
    for (int i = 0; i < n; i++) {
        cin >> pts[i].x >> pts[i].y >> pts[i].c;
        buckets[k - pts[i].y].push_back(i);
        total_sum += pts[i].c;
    }

    for (int i = 1; i <= k; i++) {
        update(1, 0, k, 0, i - 1, -A);
        for (auto idx : buckets[i]) {
            update(1, 0, k, 0, pts[idx].x, pts[idx].c);
        }
        long long v = get(1, 0, k, 0, k);
        dp[i] = max(dp[i - 1], v);

        // set the value of i to v
        long long tmp = get(1, 0, k, i, i);
        update(1, 0, k, i, i, dp[i] - tmp);
    }

    cout << total_sum - dp[k] << endl;

    return 0;
}
#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif
#include <iostream>
#define maxn 1000005
using namespace std;

int n, m, g, t;

int rocks[maxn], house[maxn];
int goron[maxn];

bool ok(int mid) {
    int ix = 0, iy = 0;
    for (int i = 0; i < g; i++) {
        int remaining = mid;
        while (iy < m && house[iy] <= goron[i] && remaining >= t) {
            remaining -= t;
            iy++;
        }
        while (ix < n && rocks[ix] <= goron[i] && remaining > 0) {
            remaining--;
            ix++;
        }
    }
    return (ix >= n && iy >= m);
}

int solve() {
    if (n > 0 && rocks[n - 1] > goron[g - 1]) {
        return -1;
    }

    if (m > 0 && house[m - 1] > goron[g - 1]) {
        return -1;
    }

    if (n == 0 && m == 0) {
        return 0;
    }

    int low = 1, high = n + m * t, ans = high;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (ok(mid)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> g >> t;
    for (int i = 0; i < n; i++) {
        cin >> rocks[i];
    }
    for (int i = 0; i < m; i++) {
        cin >> house[i];
    }
    for (int i = 0; i < g; i++) {
        cin >> goron[i];
    }

    sort(rocks, rocks + n);
    sort(house, house + m);
    sort(goron, goron + g);

    cout << solve() << endl;

    return 0;
}
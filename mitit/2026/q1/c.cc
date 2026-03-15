#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#define maxn 200005
using namespace std;

int T, n, Q;
int a[maxn];

long long tx[4 * maxn];
int min_index[4 * maxn];

void init(int i, int low, int high) {
    if (low == high) {
        tx[i] = a[low];
        min_index[i] = low;
        return;
    }

    int mid = (low + high) / 2;
    init(2 * i, low, mid);
    init(2 * i + 1, mid + 1, high);
    tx[i] = tx[2 * i] + tx[2 * i + 1];
    min_index[i] = (a[min_index[2 * i]] <= a[min_index[2 * i + 1]]) ? min_index[2 * i] : min_index[2 * i + 1];
}

void update(int i, int low, int high, int pos) {
    if (low == high) {
        tx[i] = a[pos];
        min_index[i] = pos;
        return;
    }

    int mid = (low + high) / 2;
    if (pos <= mid) {
        update(2 * i, low, mid, pos);
    } else {
        update(2 * i + 1, mid + 1, high, pos);
    }

    tx[i] = tx[2 * i] + tx[2 * i + 1];
    min_index[i] = (a[min_index[2 * i]] <= a[min_index[2 * i + 1]]) ? min_index[2 * i] : min_index[2 * i + 1];
}

long long get_sum(int i, int low, int high, int u, int v) {
    if (v < low || high < u) {
        return 0;
    }
    if (u <= low && high <= v) {
        return tx[i];
    }
    int mid = (low + high) / 2;
    return get_sum(2 * i, low, mid, u, v) + get_sum(2 * i + 1, mid + 1, high, u, v);
}

int get_min_index(int i, int low, int high, int u, int v) {
    if (v < low || high < u) {
        return 0;
    }
    if (u <= low && high <= v) {
        return min_index[i];
    }
    int mid = (low + high) / 2;
    int lindex = get_min_index(2 * i, low, mid, u, v);
    int rindex = get_min_index(2 * i + 1, mid + 1, high, u, v);

    if (lindex <= 0) {
        return rindex;
    }
    if (rindex <= 0) {
        return lindex;
    }

    return (a[lindex] <= a[rindex]) ? lindex : rindex;
}

pair<long long, long long> solve() {
    int first_turn = (n + 1) / 2, second_turn = n - first_turn;
    first_turn--;
    second_turn--;

    long long first_sum = get_sum(1, 1, n, 1, first_turn + 1);
    int first_index = get_min_index(1, 1, n, 1, first_turn + 1);
    first_sum -= a[first_index];

    long long second_sum = get_sum(1, 1, n, n - second_turn, n);
    int second_index = get_min_index(1, 1, n, n - second_turn, n);
    second_sum -= a[second_index];

    if (n % 2 == 0) {
        first_sum += max(a[first_index], a[second_index]);
        second_sum += min(a[first_index], a[second_index]);
    } else {
        second_sum += max(a[first_index], a[second_index]);
        first_sum += min(a[first_index], a[second_index]);
    }

    return {first_sum, second_sum};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
        cin >> n >> Q;
        for (int i = 1; i <= n; i++) {
            cin >> a[i];
        }
        init(1, 1, n);

        auto ans = solve();
        cout << ans.first << ' ' << ans.second << endl;
        while (Q--) {
            int pos, val;
            cin >> pos >> val;
            a[pos] = val;
            update(1, 1, n, pos);

            ans = solve();
            cout << ans.first << ' ' << ans.second << endl;
        }
    }

	return 0;
}

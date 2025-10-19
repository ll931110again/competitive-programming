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
#include <unordered_map>
#include <vector>
#define maxn 1000005
using namespace std;

int T;
int n;
int a[maxn];

struct item {
    int cnt;
    long long sum;
};

item zeroes[maxn], nonzeroes[maxn];
int nxt[maxn];

long long solve() {
    unordered_map<int,int> mp;
    mp[0] = n + 1;
    int xr = 0;
    for (int i = n; i > 0; i--) {
        if (a[i] == 0) {
            nxt[i] = i + 1;
        } else {
            xr ^= a[i];
            if (mp.find(xr) == mp.end()) {
                nxt[i] = -1;
            } else {
                nxt[i] = mp[xr];
            }
        }
        mp[xr] = i;
    }

    zeroes[n + 1] = {.cnt = 0, .sum = 0};
    nonzeroes[n + 1] = {.cnt = 0, .sum = 0};

    long long ans = 0;
    for (int i = n; i > 0; i--) {
        int x_value = nxt[i];
        if (x_value >= 0) {
            nonzeroes[i].cnt = x_value - i - 1;
        } else {
            nonzeroes[i].cnt = n - i + 1;
        }
        nonzeroes[i].sum = 1LL * nonzeroes[i].cnt * (nonzeroes[i].cnt + 1) / 2;

        zeroes[i].cnt = 0;
        zeroes[i].sum = 0;
        if (x_value >= 0) {
            zeroes[i].cnt = 1;
            zeroes[i].sum = x_value - i - 1;
        }

        if (x_value >= 0) {
            zeroes[i].cnt += zeroes[x_value].cnt;
            zeroes[i].sum += zeroes[x_value].sum + 1LL * (x_value - i - 1) * zeroes[x_value].cnt;

            nonzeroes[i].cnt += nonzeroes[x_value].cnt;
            nonzeroes[i].sum += nonzeroes[x_value].sum + 1LL * (x_value - i) * nonzeroes[x_value].cnt;
        }

        ans += zeroes[i].sum;
        ans += nonzeroes[i].sum;
    }

    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
        cerr << it << endl;
        cin >> n;
        for (int i = 1; i <= n; i++) {
            cin >> a[i];
        }

        cout << "Case #" << it << ": " << solve() << endl;
    }

    return 0;
}
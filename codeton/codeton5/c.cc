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

int T, n;
int a[maxn];

int dp[maxn], min_val[maxn], min_pos[maxn];

int solve() {
    for (int i = 1; i <= n; i++) {
        dp[i] = n + 1;
        min_val[i] = n + 1;
    }

    for (int i = 1; i <= n; i++) {
        dp[i] = min(dp[i - 1] + 1, min_val[a[i]]);
        min_val[a[i]] = min(min_val[a[i]], dp[i - 1]);
    }

    return n - dp[n];
}

int main() {
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++) {
            cin >> a[i];
        }
        cout << solve() << endl;
    }
    return 0;
}
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
#include <vector>
#define maxn 6005
using namespace std;

int T;
int n;
int cost[maxn], len[maxn];

long long ss[maxn];
long long dp[maxn][maxn][2];
long long inf = 1e18;
int max_val[maxn][maxn];

long long solve() {
    for (int i = 1; i <= n; i++) {
        for (int j = i; j <= n; j++) {
            max_val[i][j] = len[j];
            if (j > i) {
                max_val[i][j] = max(max_val[i][j], max_val[i][j - 1]);
            }
        }
    }

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            for (int k = 0; k < 2; k++) {
                dp[i][j][k] = inf;
            }
        }
    }

    dp[0][0][0] = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            for (int k = 0; k < 2; k++) if (dp[i][j][k] < inf) {
                if (len[i + 1] <= j) {
                    dp[i + 1][j][0] = min(dp[i + 1][j][0], dp[i][j][k]);
                }
                
                if (k == 0) {
                    if (j == 0) {
                        if (len[i + 1] <= 1) {
                            dp[i + 1][1][1] = min(dp[i + 1][1][1], dp[i][j][k] + cost[i + 1]);
                        }
                    } else {
                        int _i = min(n, i + j);
                        if (max_val[i + 1][_i] <= j) {
                            dp[_i][j][1] = min(dp[_i][j][1], dp[i][j][k] + ss[_i] - ss[i]);
                        }
                    }
                } else {
                    if (len[i + 1] <= j + 1) {
                        dp[i + 1][j + 1][1] = min(dp[i + 1][j + 1][1], dp[i][j][k] + cost[i + 1]);
                    }
                }
            }
        }
    }

    long long ans = inf;
    for (int j = 0; j <= n; j++) {
        for (int k = 0; k < 2; k++) {
            ans = min(ans, dp[n][j][k]);
        }
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
            cin >> cost[i];
        }
        for (int i = 1; i <= n; i++) {
            cin >> len[i];
        }

        ss[0] = 0;
        for (int i = 1; i <= n; i++) {
            ss[i] = ss[i - 1] + cost[i];
        }

        cout << "Case #" << it << ": " << solve() << endl;
    }

    return 0;
}
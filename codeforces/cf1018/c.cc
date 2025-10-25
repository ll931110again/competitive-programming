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
#include <unordered_set>
#define maxn 1005
using namespace std;

int T, n;
int a[maxn][maxn];

int row_cost[maxn], col_cost[maxn];
long long dp[maxn][2];

long long solve() {
    memset(dp, -1, sizeof dp);
    dp[0][0] = 0;
    dp[0][1] = row_cost[0];

    for (int i = 0; i + 1 < n; i++) {
        for (int j = 0; j < 2; j++) if (dp[i][j] >= 0) {
            for (int k = 0; k < 2; k++) {
                bool ok = true;
                for (int t = 0; t < n; t++) {
                    if (a[i][t] + j == a[i + 1][t] + k) {
                        ok = false;
                        break;
                    }
                }

                if (ok) {
                    long long tmp = dp[i][j] + (k ? row_cost[i + 1] : 0);
                    if (dp[i + 1][k] < 0 || dp[i + 1][k] > tmp) {
                        dp[i + 1][k] = tmp;
                    }
                }
            }
        }
    }

    if (dp[n - 1][0] < 0 && dp[n - 1][1] < 0) {
        return -1;
    }

    long long row_ans = dp[n - 1][0];
    if (row_ans < 0 || (dp[n - 1][1] >= 0 && row_ans > dp[n - 1][1])) {
        row_ans = dp[n - 1][1];
    }

    memset(dp, -1, sizeof dp);
    dp[0][0] = 0;
    dp[0][1] = col_cost[0];

    for (int i = 0; i + 1 < n; i++) {
        for (int j = 0; j < 2; j++) if (dp[i][j] >= 0) {
            for (int k = 0; k < 2; k++) {
                bool ok = true;
                for (int t = 0; t < n; t++) {
                    if (a[t][i] + j == a[t][i + 1] + k) {
                        ok = false;
                        break;
                    }
                }

                if (ok) {
                    long long tmp = dp[i][j] + (k ? col_cost[i + 1] : 0);
                    if (dp[i + 1][k] < 0 || dp[i + 1][k] > tmp) {
                        dp[i + 1][k] = tmp;
                    }
                }
            }
        }
    }

    if (dp[n - 1][0] < 0 && dp[n - 1][1] < 0) {
        return -1;
    }

    long long col_ans = dp[n - 1][0];
    if (col_ans < 0 || (dp[n - 1][1] >= 0 && col_ans > dp[n - 1][1])) {
        col_ans = dp[n - 1][1];
    }

    return row_ans + col_ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cin >> a[i][j];
            }
        }

        for (int i = 0; i < n; i++) {
            cin >> row_cost[i];
        }

        for (int i = 0; i < n; i++) {
            cin >> col_cost[i];
        }

        cout << solve() << endl;
    }
    return 0;
}
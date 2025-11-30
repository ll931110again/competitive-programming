/*
 * Solution for problem B: Streetlamp Safety
 * 
 * We start with the O(N^3) solution: define dp(i, max_len, current_len) be the minimum cost
 * of turning on lights such that at position i, the maximum segment is max_len, and the
 * current segment is current_len. From here, we can transition to different states:
    - If we don't turn on the (i + 1)-th light, then update
        dp(i + 1, max_len, 0) = min(dp(i + 1, max_len, 0), dp(i, max_len, current_len))
        This state is only valid if B[i + 1] <= max_len
    - If we do turn on the (i + 1)-th, then update
        _max_len = max(max_len, current_len + 1)
        _current_len = current_len + 1
        dp(i + 1, _max_len, _current_len) = min(dp(i + 1, _max_len, _current_len), dp(i, max_len, current_len) + cost(i + 1))
        This state is only valid if B[i + 1] <= _max_len

 * To optimize the above solution, observe a sequence of lights
    ...***..**..***...****...
    where . corresponds to light turning off, and * corresponds to light turning on.

    We note that, in an optimal solution, the segments of lights turning on must have increasing length.
    This is easy to see, since we can simply turn off all the lights on any segment that violates the constraints, and get a
    valid solution with lower cost.

    Consequently, if we are at light i, and current maximum segment is max_len, then either:
    - We do not turn on the light i + 1.
    - If i belongs to the current maximum segment, then we can extend i + 1 (and increment max_len).
    - If i does not belong to the current maximum segment, then we turn on all the lights from i + 1 to i + max_len + 1 (and increment max_len).

    With that, we can define the dp(i, max_len, is_active_segment) where is_active_segment indicates whether i belongs to the maximal length segment.
    And we run the DP as stated above.

    Time complexity: O(N^2).
 */

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
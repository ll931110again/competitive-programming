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
#define maxn 250005
using namespace std;

int T, n, Q;
int a[maxn];
int nc[maxn][2];
int next_same[maxn];

int solve(int L, int R) {
    int nzeros = nc[R][0] - nc[L - 1][0];
    int nones = nc[R][1] - nc[L - 1][1];
    if (nzeros % 3 || nones % 3) {
        return -1;
    }

    int ans = nzeros / 3 + nones / 3;
    if (next_same[L] > R) {
        ans++;
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> T;
    while (T--) {
        cin >> n >> Q;
        for (int i = 1; i <= n; i++) {
            cin >> a[i];
        }

        nc[0][0] = nc[0][1] = 0;
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < 2; j++) {
                nc[i][j] = nc[i - 1][j] + (a[i] == j);
            }
        }

        next_same[n] = n + 1;
        for (int i = n - 1; i > 0; i--) {
            next_same[i] = next_same[i + 1];
            if (a[i] == a[i + 1]) {
                next_same[i] = i + 1;
            }
        }

        while (Q--) {
            int L, R;
            cin >> L >> R;
            cout << solve(L, R) << endl;
        }
    }
    return 0;
}
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
int a[maxn][4];

long long solve() {
    long long ans = 0;
    for (int i = 0; i < n; i++) {
        if (a[i][1] > a[i][3]) {
            ans += a[i][0] + a[i][1] - a[i][3];
        } else {
            ans += max(0, a[i][0] - a[i][2]);
        }
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < 4; j++) {
                cin >> a[i][j];
            }
        }
        cout << solve() << endl;
    }
   return 0;
}
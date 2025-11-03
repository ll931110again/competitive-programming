#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#include <queue>
#define maxn 200005
#define mod 1000000007
using namespace std;

int T, n, K;
int a[maxn];

int solve() {
    
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
        cin >> n >> K;
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        cout << solve() << endl;
    }

	return 0;
}

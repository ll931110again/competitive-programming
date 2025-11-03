#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#define maxn 200005
using namespace std;

int T, n;
long long a[maxn];

long long solve() {
    long long ans = 1e18;
    long long minDelta = a[1] - a[0];
    for (int i = 3; i < n; i++) {
        ans = min(ans, a[i] - a[i - 1] + minDelta);
        minDelta = min(minDelta, a[i - 1] - a[i - 2]);
    }

    return ans;
}
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int T;
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        sort(a, a + n);

        cout << solve() << endl;
    }
	return 0;
}

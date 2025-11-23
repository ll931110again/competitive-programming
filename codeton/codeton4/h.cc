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
using namespace std;

int T, n;

const long double k = (1 + sqrt((long double) 5)) / 2;

void rec(__int128 n, __int128 a, __int128 b, __int128 c, __int128& f) {
    if (n < 0) {
        f = 0;
        return;
    }

    if (a >= c || b >= c) {
        rec(n, a % c, b % c, c, f);
        f += (a / c) * (n * (n + 1) / 2) + (b / c) * (n + 1);
    } else if (a) {
        __int128 m = (a * n + b) / c;
        rec(m - 1, c, c - b - 1, a, f);
        f = n * m - f;
    }
}

long long solve(int n) {
    // Find the first moment that (n - x/phi <= 2x)
    int low = 0, high = n, pv = 0;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (n - floor(mid / k) <= 2 * mid) {
            pv = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    __int128 x = (__int128) 2000000000000000000ll * 1000000000;
    __int128 y = (__int128) 1000000000000000000ll * 1000000000;
    __int128 z = (__int128) 1618033988749894848ll * 1000000000 + 204586834;

    __int128 a, b, c, d;
    rec((n - 1) / 2, x, 0, z, a);
    rec((pv - 1) / 2, x, 0, z, b);
    rec((n - 2) / 2, x, y, z, c);
    rec((pv - 2) / 2, x, y, z, d);
    
    long long ans = pv + (pv % 2 ? -1 : 1) * ((a - b) - (c - d));
    if ((n - pv) % 2) {
        ans = n - ans;
    }

    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    while (T--) {
        cin >> n;
        cout << solve(n) << endl;
    }
    return 0;
}

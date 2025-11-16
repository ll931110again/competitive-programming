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
using namespace std;

int T;
int n, m;

bool solve(int n, int m) {
    // need at least m points
    if (n < m) {
        return false;
    }

    // One player gets m points, the other gets <= m - 2 points
    if (n <= 2 * m - 2) {
        return true;
    }

    // One player gets k points, the other gets k - 2 points (for k > m)
    return (n % 2 == 0);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
        cerr << it << endl;
        cin >> n >> m;

        cout << "Case #" << it << ": " << (solve(n, m) ? "YES" : "NO") << endl;
    }

    return 0;
}
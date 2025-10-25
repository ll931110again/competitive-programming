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

int T;
int n, rk, ck, rd, cd;

int main() {
    cin >> T;
    while (T--) {
        cin >> n >> rk >> ck >> rd >> cd;
        int ans = 0;
        if (rk > rd) {
            ans = max(ans, n - rd);
        } else if (rk < rd) {
            ans = max(ans, rd);
        }
        if (ck > cd) {
            ans = max(ans, n - cd);
        } else if (ck < cd) {
            ans = max(ans, cd);
        }
        cout << ans << endl;
    }
    return 0;
}
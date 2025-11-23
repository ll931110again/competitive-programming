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
#define maxn 105
using namespace std;
 
int T, n;
int a[maxn];
 
bool solve() {
    int found = -1;
    for (int i = 0; i < n; i++) {
        if (a[i] >= 0) {
            if (found < 0) {
                found = a[i];
            }
            if (found != a[i]) {
                return false;
            }
        }
    }
 
    if (found == 0) {
        return false;
    }
 
    return true;
}
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
 
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        cout << (solve() ? "YES" : "NO") << endl;
    }
    return 0;
}

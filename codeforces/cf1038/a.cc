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

int T, m, n;

bool solve(int m, int n) {
    if (m > n) {
        swap(m, n);
    }
    if (m == 1) {
        return false;
    }
    if (m == 2 && n == 2) {
        return false;
    }
    return true;
}

int main() {
    cin >> T;
    while (T--) {
        cin >> m >> n;
        cout << (solve(m, n) ? "YES" : "NO") << endl;
    }
    return 0;
}
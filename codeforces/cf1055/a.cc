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
#define maxn 105
using namespace std;

int T, n;
int a[maxn];

int main() {
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }

        set<int> s;
        for (int i = 0; i < n; i++) {
            s.insert(a[i]);
        }

        cout << 2 * s.size() - 1 << endl;
    }
    return 0;
}
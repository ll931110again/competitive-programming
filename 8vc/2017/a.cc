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

int n;
int a[maxn];
bool marked[maxn];
int flips;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    int cyc = 0;
    for (int i = 1; i <= n; i++) if (!marked[i]) {
        cyc++;
        int x = i;
        while (!marked[x]) {
            marked[x] = true;
            x = a[x];
        }
    }

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        flips ^= x;
    }

    int ans = ((cyc > 1) ? cyc : 0) + (flips ^ 1);
    cout << ans << endl;

    return 0;
}
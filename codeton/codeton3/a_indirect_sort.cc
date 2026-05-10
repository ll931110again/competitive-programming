// Codeforces 1750 (CodeTON Round 3 (Div. 1 + Div. 2, Rated, Prizes!)) — A. Indirect Sort
// Submission: https://codeforces.com/contest/1750/submission/334470282

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
#define maxn 20
using namespace std;

int T, n;
int a[maxn];

int main() {
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++) {
            cin >> a[i];
        }
        if (a[1] == 1) {
            cout << "Yes" << endl;
        } else {
            cout << "No" << endl;
        }
    }
    return 0;
}

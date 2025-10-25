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
string s;
int a[maxn];

int main() {
    cin >> T;
    while (T--) {
        cin >> n;
        cin >> s;
        int low = 1, high = n;
        for (int i = n - 2; i >= 0; i--) {
            if (s[i] == '<') {
                a[i + 1] = low++;
            } else {
                a[i + 1] = high--;
            }
        }
        a[0] = low;

        for (int i = 0; i < n; i++) {
            cout << a[i] << ' ';
        }
        cout << endl;
    }
    return 0;
}
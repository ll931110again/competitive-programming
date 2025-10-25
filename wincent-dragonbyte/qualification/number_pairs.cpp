#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#include <queue>
using namespace std;

int T, d;

int digits_sum(int x) {
    int dx = 0;
    while (x) {
        dx += x % 10;
        x /= 10;
    }
    return dx;
}

void solve(int d) {
    for (int y = 1; y <= 1e9; y++) {
        if (digits_sum(y + d) == digits_sum(y)) {
            cout << y + d << ' ' << y << endl;
            return;
        }
    }
    cout << "NONE" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
        cerr << T << endl;
        cin >> d;
        solve(d);
    }
	return 0;
}

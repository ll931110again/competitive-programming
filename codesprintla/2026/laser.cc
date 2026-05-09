#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif
#include <iostream>
using namespace std;

int t, a, b;

int gcd(int x, int y) {
    if (x == 0) {
        return y;
    }
    if (x > y) {
        swap(x, y);
    }
    return gcd(y % x, x);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> t;
    while (t--) {
        cin >> a >> b;
        if (a % (2 * gcd(a, b))) {
            cout << "A" << endl;
        } else {
            cout << "B" << endl;
        }
    }

    return 0;
}
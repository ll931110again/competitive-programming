// Codeforces 1896 (CodeTON Round 7 (Div. 1 + Div. 2, Rated, Prizes!)) — D. Ones and Twos
// Submission: https://codeforces.com/contest/1896/submission/336705976

#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <set>
#include <map>
#include <queue>
#include <vector>
#define maxn 100005
using namespace std;

int T, n, q;
int a[maxn];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> T;
    while (T--) {
        cin >> n >> q;
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }

        set<int> ones;
        for (int i = 0; i < n; i++) if (a[i] == 1) {
            ones.insert(i);
        }

        int sum_value = 0;
        for (int i = 0; i < n; i++) {
            sum_value += a[i];
        }

        auto check = [&](int val) {
            if (sum_value < val) {
                return false;
            }

            if ((sum_value - val) % 2 == 0) {
                return true;
            }

            if (ones.empty()) {
                return false;
            }

            auto it = ones.begin();
            int pos = *it;

            if (sum_value - 2 * pos - 1 >= val) {
                return true;
            }

            it = ones.end();
            it--;
            pos = *it;
            if (sum_value - 2 * (n - 1 - pos) - 1 >= val) {
                return true;
            }

            return false;
        };

        while (q--) {
            int op;
            cin >> op;

            if (op == 1) {
                int val;
                cin >> val;
                cout << (check(val) ? "YES" : "NO") << endl;
            } else {
                int pos, val;
                cin >> pos >> val;
                pos--;

                sum_value -= a[pos];
                sum_value += val;
                a[pos] = val;

                ones.erase(pos);
                if (val == 1) {
                    ones.insert(pos);
                }
            }
        }
   }
    
    return 0;
}

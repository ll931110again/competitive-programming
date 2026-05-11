// Codeforces 1896 (CodeTON Round 7 (Div. 1 + Div. 2, Rated, Prizes!)) — C. Matching Arrays
// Submission: https://codeforces.com/contest/1896/submission/336567371

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

int T, n, k;
int a[maxn], b[maxn];

struct Element {
    int value;
    int pos;
};

Element sorted_a[maxn], sorted_b[maxn], arranged_b[maxn];
int ans[maxn];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> T;
    while (T--) {
        cin >> n >> k;
        for (int i = 0; i < n; i++) {
            cin >> a[i];
            sorted_a[i] = {.value = a[i], .pos = i};
        }
        sort(sorted_a, sorted_a + n, [&](auto x, auto y) {
            return x.value > y.value;
        });

        for (int i = 0; i < n; i++) {
            cin >> b[i];
            sorted_b[i] = {.value = b[i], .pos = i};   
        }
        sort(sorted_b, sorted_b + n, [&](auto x, auto y) {
            return x.value > y.value;
        });

        for (int i = 0; i < n; i++) {
            if (i < k) {
                arranged_b[i] = sorted_b[i + n - k];
            } else {
                arranged_b[i] = sorted_b[i - k];
            }
        }

        int score = 0;
        for (int i = 0; i < n; i++) if (sorted_a[i].value > arranged_b[i].value) {
            score++;
        }

        if (score != k) {
            cout << "No" << endl;
            continue;
        }

        cout << "Yes" << endl;
        for (int i = 0; i < n; i++) {
            ans[sorted_a[i].pos] = arranged_b[i].pos;
        }

        for (int i = 0; i < n; i++) {
            cout << b[ans[i]] << ' ';
        }
        cout << endl;
    }
    
    return 0;
}

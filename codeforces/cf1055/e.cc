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
#include <unordered_set>
#include <vector>
#define maxn 105
using namespace std;

int T, n;

set<int> a;
vector< vector<int> > b;

vector<int> query() {
    cout << "? ";
    cout << a.size() << " ";
    for (auto x : a) {
        cout << x << ' ';
    }
    cout << endl;
    cout << flush;

    int sz;
    cin >> sz;

    vector<int> indices(sz);
    for (int i = 0; i < sz; i++) {
        cin >> indices[i];
    }

    return indices;
}

void solve() {
    a.clear();
    b.clear();

    for (int i = 1; i <= n * n + 1; i++) {
        a.insert(i);
    }

    for (int i = 0; i < n; i++) {
        auto ans = query();
        b.push_back(ans);

        if (ans.size() > n) {
            cout << "! ";
            for (int j = 0; j <= n; j++) {
                cout << ans[j] << " ";
            }
            cout << endl;
            cout << flush;
            return;
        }

        for (auto x : ans) {
            a.erase(x);
        }
    }

    vector<int> leftover;
    for (auto x : a) {
        leftover.push_back(x);
    }
    b.push_back(leftover);

    vector<int> ans;
    int last = 1e9;
    for (int i = n; i >= 0; i--) {
        for (int j = b[i].size() - 1; j >= 0; j--) if (b[i][j] < last) {
            ans.push_back(b[i][j]);
            last = b[i][j];
            break;
        }
    }
    reverse(ans.begin(), ans.end());
    cout << "! ";
    for (auto x : ans) {
        cout << x << ' ';
    }
    cout << endl;
    cout << flush;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> T;
    while (T--) {
        cin >> n;
        solve();
    }
    
    return 0;
}
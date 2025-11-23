#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif
 
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>
#include <set>
#include <queue>
#include <vector>
#define maxn 200005
using namespace std;
 
int T, n, K;
pair<int,int> a[maxn];
 
multiset< pair<int,int> > s;
 
long long delta(pair<int,int> x, pair<int,int> y) {
    long long old_value = x.second - x.first + y.second - y.first;
    vector<int> new_values = {x.first, x.second, y.first, y.second};
    sort(new_values.begin(), new_values.end());
    long long new_value = new_values[2] - new_values[0] + new_values[3] - new_values[1];
 
    return new_value - old_value;
}
 
long long solve() {
    long long value = 0;
    for (int i = 0; i < n; i++) {
        value += a[i].second - a[i].first;
    }
 
    int max_right = -1;
    for (auto it : s) {
        if (it.first < max_right) {
            return value;
        }
        max_right = max(max_right, it.second);
    }
 
    long long ans = 1e18;
    for (int i = 0; i < n; i++) {
        auto it = s.find(a[i]);
        if (it != s.begin()) {
            auto prev = it;
            prev--;
 
            ans = min(ans, delta(*it, *prev));
        }
 
        auto nxt = it;
        nxt++;
        if (nxt != s.end()) {
            ans = min(ans, delta(*it, *nxt));
        }
    }
 
    return ans + value;
}
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
 
    cin >> T;
    while (T--) {
        cin >> n >> K;
        for (int i = 0; i < n; i++) {
            cin >> a[i].first;
        }
        for (int i = 0; i < n; i++) {
            cin >> a[i].second;
        }
 
        for (int i = 0; i < n; i++) {
            if (a[i].first > a[i].second) {
                swap(a[i].first, a[i].second);
            }
        }
        s.clear();
        for (int i = 0; i < n; i++) {
            s.insert(a[i]);
        }
 
        cout << solve() << endl;
    }
    return 0;
}

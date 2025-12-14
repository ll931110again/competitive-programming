#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <map>
#include <stack>
#include <set>
#include <vector>
#define maxn 200005
using namespace std;

int T, n;
int a[maxn];
map<int,int> mp;

bool ok() {
    sort(a, a + n);
    mp.clear();
    for (int i = 0; i < n; i++) {
        mp[a[i]]++;
    }

    if (a[0] == a[n - 1]) {
        return true;
    }

    // put minimum and maximum in both partitions
    if (mp[a[0]] >= 2 && mp[a[n - 1]] >= 2) {
        return true;
    }

    // put a[0] - a[i] and a[i + 1] - a[n - 1]
    for (int i = 0; i + 1 < n; i++) {
        if (a[i] - a[0] == a[n - 1] - a[i + 1]) {
            return true;
        }
    }

    for (int i = 1; i + 1 < n; i++) {
        int delta = a[n - 1] - (a[i] - a[0]);
        if (delta > a[i]) {
            continue;
        }
        mp[a[0]]--;
        mp[a[i]]--;
        mp[a[n - 1]]--;
        if (mp.find(delta) != mp.end() && mp[delta] > 0) {
            return true;
        }

        mp[a[0]]++;
        mp[a[i]]++;
        mp[a[n - 1]]++;
    }

    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }

        cout << (ok() ? "Yes" : "No") << endl;
    }
	return 0;
}

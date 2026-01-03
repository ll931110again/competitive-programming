#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#define maxn 200005
using namespace std;

int T, n;
pair<int,int> a[maxn];

bool ok(int mid) {
    int maxy = 0;
    int j = 0;

    for (int i = 0; i < n; i++) {
        while (j < n && a[j].first < a[i].first - mid) {
            maxy = max(maxy, a[j].second);
            j++;
        }
        if (a[i].second + mid < maxy) {
            return false;
        }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> a[i].first >> a[i].second;
        }
        sort(a, a + n);

        int low = 0, high = 1e9, ans = high;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (ok(mid)) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        cout << ans << endl;
    }
	return 0;
}

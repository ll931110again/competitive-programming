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
#define maxn 5005
using namespace std;

int T, n;
int a[maxn];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }

        int ans = 0;
        for (int i = 0; i < n; i++) {
            bool is_prefix = true, is_suffix = true;
            for (int j = 0; j < i; j++) if (a[i] > a[j]) {
                is_prefix = false;
            }
            for (int j = i + 1; j < n; j++) if (a[i] < a[j]) {
                is_suffix = false;
            }
            if (is_prefix || is_suffix) {
                ans++;
            }
        }

        cout << ans << endl;
    }
	return 0;
}

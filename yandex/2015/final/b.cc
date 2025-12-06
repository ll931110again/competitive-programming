#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 105
#define maxk 6
using namespace std;

int n;
int a[maxn];
int occupy_cnt[maxk];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	sort(a, a + n);
	int ans = 0;
	for (int i = n - 1; i >= 0; i--) {
		bool filled = false;
		for (int j = a[i]; j < maxk; j++) if (occupy_cnt[j]) {
			occupy_cnt[j]--;
			occupy_cnt[j - a[i]]++;
			filled = true;
			break;
		}
		if (!filled) {
			ans++;
			occupy_cnt[maxk - a[i]]++;
		}
	}

	cout << ans << endl;

    return 0;
}
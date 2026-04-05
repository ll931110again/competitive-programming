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
#define maxn 200005
using namespace std;

int n, a[maxn], pos[maxn];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> a[i];
		pos[a[i]] = i;
	}

	int runs = 0, current_pos = n + 1;
	for (int value = 1; value <= n; value++) {
		if (pos[value] < current_pos) {
			runs++;
		}
		current_pos = pos[value];
	}

	cout << runs << endl;
    
    return 0;
}
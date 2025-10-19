#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
using namespace std;

int T;
int n, a, b;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
        cerr << it << endl;
		cin >> n >> a >> b;
		cout << "Case #" << it << ": ";

		for (int i = 0; i < 2 * n - 1; i++) {
			cout << 1 << ' ';
		}
		cout << b << endl;
    }

    return 0;
}
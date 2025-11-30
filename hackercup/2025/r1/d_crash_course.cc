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
int n;
string s;

bool solve() {
	int delta = 0;
	for (int i = n - 1; i >= 0; i--) {
		if (s[i] == 'A') {
			delta++;
		} else {
			delta--;
		}
		if (delta > 0) {
			return true;
		}
	}
	return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
        cerr << it << endl;
        cin >> n;
		cin >> s;

        cout << "Case #" << it << ": " << (solve() ? "Alice" : "Bob") << endl;
    }

    return 0;
}
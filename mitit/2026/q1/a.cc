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
using namespace std;

int T, n;
string s;

bool is_valid(string s) {
    if (s[0] != 'M') {
        return false;
    }
    for (int i = 1; i < s.size(); i++) {
        if (i % 2 && s[i] != 'I') {
            return false;
        }
        if (i % 2 == 0 && s[i] != 'T') {
            return false;
        }
    }
    return true;
}

int solve(string s) {
    if (is_valid(s)) {
        return 0;
    }
    for (int i = 0; i < s.size(); i++) if (s[i] == 'M') {
        if (is_valid(s.substr(i, s.size() - i) + s.substr(0, i))) {
            return 1;
        }
    }
    return -1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
        cin >> n;
        cin >> s;
        cout << solve(s) << endl;
    }

	return 0;
}

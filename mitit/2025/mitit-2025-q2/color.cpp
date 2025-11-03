#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#include <queue>
using namespace std;

int T, n;
string s;

string solve(string s) {
    string corpus = "rgby";

    string ans = "";
    for (int i = 0; i < n - 2; i++) {
        set<char> available;
        for (int j = i; j < i + 3; j++) {
            available.insert(s[j]);
        }

        for (auto c : corpus) {
            if (available.find(c) == available.end()) {
                ans += c;
                break;
            }
        }
    }

    ans += "gg";
    return ans;
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
}
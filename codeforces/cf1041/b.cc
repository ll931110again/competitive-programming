#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif
 
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 105
using namespace std;
 
int T, n, x;
string s;
 
int solve_boundary(int ls, int rs) {
    if (ls < 0 || rs < 0) {
        return 1;
    }
 
    return 1 + min(ls + 1, n - rs);
}
 
int solve() {
    // if no wall found, ans = 1
    bool no_wall = true;
    for (auto c : s) if (c == '#') {
        no_wall = false;
    }
 
    if (no_wall) {
        return 1;
    }
 
    // wall on one side
    int left_side = -1, right_side = -1;
    int tmp = x - 1;
    while (tmp >= 0 && s[tmp] == '.') {
        tmp--;
    }
 
    if (tmp >= 0) {
        left_side = tmp;
    }
 
    tmp = x + 1;
    while (tmp < n && s[tmp] == '.') {
        tmp++;
    }
    if (tmp < n) {
        right_side = tmp;
    }
 
    int ans = 0;
    ans = max(ans, solve_boundary(left_side, right_side));
    ans = max(ans, solve_boundary(x - 1, right_side));
    ans = max(ans, solve_boundary(left_side, x + 1));
    return ans;
}
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
 
    cin >> T;
    while (T--) {
        cin >> n >> x;
        x--;
        cin >> s;
        cout << solve() << endl;
    }
    return 0;
}

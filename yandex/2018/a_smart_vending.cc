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
#include <unordered_set>
using namespace std;
 
int M = 1e6;
long long b, c, r, d;
 
long long solve() {
    long long z = (b * M + c) / r;
    if (c + d >= M - 1) {
        return z;
    }
 
    unordered_set<long long> seen;
    seen.insert(c);
 
    for (int it = 0; ; it++) {
        if (b * M + c < r) {
            return it;
        }
 
        long long new_b = b - r/M, new_c = c, new_d = d;
        if (c >= r % M) {
            new_c = c - r % M;
            new_d = d + r % M;
        } else {
            new_b--;
            int changes = M - r % M;
            new_c = c + changes;
            new_d = d - changes;
        }
 
        if (new_b < 0 || new_c < 0 || new_d < 0) {
            return it;
        }
 
        b = new_b;
        c = new_c;
        d = new_d;
 
        if (seen.find(c) != seen.end()) {
            return z;
        }
 
        seen.insert(c);
    }
 
    return z;
}
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
 
    cin >> b >> c >> r >> d;
    cout << solve() << endl;
    return 0;
}

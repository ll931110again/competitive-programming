#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
using namespace std;
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        
        string ans;
        if (n % 2 == 0) {
            ans = "2025";
        } else {
            ans = "42025";
        }
        for (int i = ans.size(); i < n; i++) {
            ans += '0';
        }

        cout << ans << endl;
    }
	return 0;
}

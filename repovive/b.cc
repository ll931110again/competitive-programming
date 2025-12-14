#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <map>
#include <stack>
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
            stack<int> s;
            for (int j = i + 1; j < n; j++) if (a[j] > a[i]) {
                while (!s.empty() && s.top() > a[j]) {
                    s.pop();
                }
                if (s.empty()) {
                    ans++;
                }
                s.push(a[j]);
            }
        }

        cout << ans << endl;
    }
	return 0;
}

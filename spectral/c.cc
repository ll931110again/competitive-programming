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
#define maxn 5005
using namespace std;

int T, n;
int a[maxn], elements[maxn];
int dp[maxn];
int inf = 1e6;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> a[i];
            elements[i] = a[i];
        }
        sort(elements, elements + n);
        int median = elements[n/2];

        for (int i = 0; i <= n; i++) {
            dp[i] = -inf;
        }
        dp[0] = 0;

        for (int i = 1; i <= n; i++) {
            int below = 0, above = 0, equal = 0;
            for (int j = i; j > 0; j--) {
                if (a[j - 1] < median) {
                    below++;
                } else if (a[j - 1] == median) {
                    equal++;
                } else {
                    above++;
                }

                int num_elements = below + equal + above;
                if (equal > 0 && num_elements % 2 && below < (num_elements + 1) / 2 && (below + equal) >= (num_elements + 1) / 2) {
                    dp[i] = max(dp[i], 1 + dp[j - 1]);
                }
            }
        }

        cout << dp[n] << endl;
    }

	return 0;
}

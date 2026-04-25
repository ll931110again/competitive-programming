/*
 * Solution for E.

 * There are a couple of simple insights:
    We start by inserting 0 into the set, and then add(0). Then we query(0).

    If the set only has 0 (count = 1), then we know the operation of interest is AND operation.
    We can recover k by checking each bit one by one.

    Otherwise (count = 2), the set contains 0 and c. Binary search to find the value c.
    
 */

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

void initial_insert(long long a) {
    cout << a << endl;
    cout << flush;
}

int insert(long long x) {
    cout << "I " << x << endl;
    cout << flush;

    int cnt;
    cin >> cnt;
    return cnt;
}

int query(long long y) {
    cout << "Q " << y << endl;
    cout << flush;

    int cnt;
    cin >> cnt;
    return cnt;
}

void answer(int k, long long c) {
    cout << "A " << k << ' ' << c << endl;
    cout << flush;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
        cin >> n;
        long long all = (1LL << n) - 1;

        initial_insert(0);
        int cnt = insert(0);
        int ans_k = -1;
        long long ans_c = -1;

        if (cnt == 1) {
            ans_k = 1;  // and operation
            ans_c = 0;
            int prev_cnt = cnt;
            for (int i = 0; i < n; i++) {
                cnt = insert(1LL << i);
                if (prev_cnt < cnt) {
                    ans_c |= (1LL << i);
                }
                prev_cnt = cnt;
            }
        } else {
            // binary search the answer
            long long low = 1, high = all;
            while (low <= high) {
                long long mid = (low + high) / 2;
                if (query(mid) >= 1) {
                    ans_c = mid;
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }

            // if c = 2^n - 1, then insert(1) and check if we get a new element
            if (ans_c == all) {
                cnt = insert(1);
                ans_k = (cnt == 2) ? 2 : 3;
            } else {
                // Otherwise, insert(all) and query(all) to see if we get all inserted
                insert(all);
                cnt = query(all);
                ans_k = (cnt == 1) ? 2 : 3;
            }
        }

        answer(ans_k, ans_c);
    }

	return 0;
}

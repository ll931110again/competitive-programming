// G – Hextech Ordnance
// https://codesprintla25.kattis.com/problems/hextechordnance
//
// Count subarrays (length ≥ 2) with max−min ≤ K using monotone deques in O(n).
// For K ≥ global spread D = max(a)−min(a), every subarray qualifies, so count = n(n−1)/2.

#include <bits/stdc++.h>
using namespace std;

static long long count_at_most_K(const vector<int> &a, long long K) {
    const int n = (int)a.size();
    deque<int> dq_min, dq_max;
    long long ans = 0;
    int l = 0;
    for (int r = 0; r < n; ++r) {
        while (!dq_min.empty() && a[dq_min.back()] >= a[r]) {
            dq_min.pop_back();
        }
        dq_min.push_back(r);
        while (!dq_max.empty() && a[dq_max.back()] <= a[r]) {
            dq_max.pop_back();
        }
        dq_max.push_back(r);

        while (!dq_min.empty() && !dq_max.empty() &&
               (long long)a[dq_max.front()] - (long long)a[dq_min.front()] > K) {
            if (dq_min.front() == l) {
                dq_min.pop_front();
            }
            if (dq_max.front() == l) {
                dq_max.pop_front();
            }
            ++l;
        }
        const int len = r - l + 1;
        if (len >= 2) {
            ans += (long long)(len - 1);
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) {
        return 0;
    }
    vector<int> a(n);
    int mn = INT_MAX, mx = INT_MIN;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        mn = min(mn, a[i]);
        mx = max(mx, a[i]);
    }

    const long long D = (long long)mx - (long long)mn;
    const long long total_pairs = 1LL * n * (n - 1) / 2;

    for (long long k = 1; k <= n; ++k) {
        long long cnt = (k >= D) ? total_pairs : count_at_most_K(a, k);
        if (cnt < k) {
            cout << "unstable\n";
            return 0;
        }
    }
    cout << "stable\n";
    return 0;
}

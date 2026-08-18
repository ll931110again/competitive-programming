#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <functional>
using namespace std;

using ll = long long;
const ll INF = (1LL << 60);

struct Fenwick {
    int n;
    vector<ll> bit;
    Fenwick(int n = 0) {
        this->n = n;
        bit.assign(n + 1, 0);
    }
    void add(int idx, ll delta) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += delta;
    }
    ll sum(int idx) {
        ll s = 0;
        for (; idx > 0; idx -= idx & -idx) s += bit[idx];
        return s;
    }
    ll range_sum(int l, int r) {
        if (l > r) return 0;
        return sum(r) - sum(l - 1);
    }
    // smallest idx such that prefix sum >= k (1-indexed)
    int find_kth(ll k) {
        int idx = 0;
        int bitMask = 1;
        while (bitMask << 1 <= n) bitMask <<= 1;
        for (; bitMask; bitMask >>= 1) {
            int t = idx + bitMask;
            if (t <= n && bit[t] < k) {
                idx = t;
                k -= bit[t];
            }
        }
        return idx + 1;
    }
};

struct Solver {
    int n;
    vector<ll> a;
    vector<ll> vals;
    Fenwick cntBit, sumBit;
    ll totalCnt = 0, totalSum = 0;
    int curL = 1, curR = 0;

    Solver(int n, vector<ll> a, vector<ll> vals)
        : n(n), a(a), vals(vals), cntBit((int)vals.size()), sumBit((int)vals.size()) {}

    int get_idx(ll x) const {
        return (int)(lower_bound(vals.begin(), vals.end(), x) - vals.begin()) + 1;
    }

    void add(int pos) {
        ll v = a[pos];
        int idx = get_idx(v);
        cntBit.add(idx, 1);
        sumBit.add(idx, v);
        totalCnt++;
        totalSum += v;
    }

    void remove(int pos) {
        ll v = a[pos];
        int idx = get_idx(v);
        cntBit.add(idx, -1);
        sumBit.add(idx, -v);
        totalCnt--;
        totalSum -= v;
    }

    void ensure(int L, int R) {
        while (curL > L) { add(--curL); }
        while (curR < R) { add(++curR); }
        while (curL < L) { remove(curL++); }
        while (curR > R) { remove(curR--); }
    }

    ll currentCost() {
        if (totalCnt == 0) return 0;
        ll k = (totalCnt + 1) / 2;
        int idxMed = cntBit.find_kth(k);
        ll med = vals[idxMed - 1];
        ll freqMed = cntBit.range_sum(idxMed, idxMed);
        ll cntLess = cntBit.sum(idxMed - 1);
        ll sumLess = sumBit.sum(idxMed - 1);
        ll cntGreater = totalCnt - cntLess - freqMed;
        ll sumGreater = totalSum - sumLess - freqMed * med;
        return med * cntLess - sumLess + sumGreater - med * cntGreater;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<ll> a(n + 1);
        vector<ll> vals;
        for (int i = 1; i <= n; i++) {
            cin >> a[i];
            vals.push_back(a[i]);
        }
        // coordinate compression
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());

        Solver solver(n, a, vals);

        vector<ll> dp(n + 1, INF);
        dp[0] = 0;
        dp[1] = INF; // impossible

        function<void(int, int, int, int)> compute = [&](int l, int r, int optL, int optR) {
            if (l > r) return;
            int mid = (l + r) >> 1;
            ll best = INF;
            int bestK = -1;
            int start = optL;
            int end = min(optR, mid - 2);
            if (start <= end) {
                solver.ensure(start + 1, mid);
                int ptrL = start + 1;
                for (int j = start; j <= end; j++) {
                    if (j > start) {
                        // remove leftmost element
                        solver.remove(ptrL);
                        ptrL++;
                    }
                    ll val = dp[j] + solver.currentCost();
                    if (val < best) {
                        best = val;
                        bestK = j;
                    }
                }
                dp[mid] = best;
            } else {
                dp[mid] = INF;
            }
            compute(l, mid - 1, optL, bestK == -1 ? optL : bestK);
            compute(mid + 1, r, bestK == -1 ? optL : bestK, optR);
        };

        compute(2, n, 0, n - 2);

        // cost to make all equal
        solver.ensure(1, n);
        ll costAll = solver.currentCost();
        ll ans = min(costAll, dp[n]);
        cout << ans << '\n';
    }
    return 0;
}
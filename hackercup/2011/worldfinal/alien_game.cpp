#include <algorithm>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

// Parity (mod 2) of number of pairs (i < j) with a[j] > a[i].
static int parityIncreasingPairs(const vector<long long>& a) {
    vector<long long> xs = a;
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());

    int n = (int)xs.size();
    vector<int> bit(n + 1, 0); // stores counts mod 2

    auto add = [&](int idx) {
        for (; idx <= n; idx += idx & -idx) bit[idx] ^= 1;
    };
    auto sum = [&](int idx) {
        int r = 0;
        for (; idx > 0; idx -= idx & -idx) r ^= bit[idx];
        return r;
    };

    int parity = 0;
    for (long long v : a) {
        int id = (int)(lower_bound(xs.begin(), xs.end(), v) - xs.begin()) + 1;
        // count of prior elements strictly smaller than v, mod 2
        parity ^= sum(id - 1);
        add(id);
    }
    return parity;
}

static long long countWinningX(long long F,
                              int N,
                              int P,
                              const vector<long long>& buckets) {
    // Only buckets 2..N-1 (1-indexed) can be played; endpoints never affect legality.
    // Let internal array v[1..m] correspond to buckets[2..N-1] (1-indexed).
    int m = max(0, N - 2);
    vector<long long> v(m + 1, 0); // 1..m
    for (int i = 1; i <= m; i++) v[i] = buckets[i + 1];

    auto winningForFixedInternal = [&]() -> bool {
        // total moves parity = parity of number of positive subarray sums in v[1..m]
        // which equals parity of number of pairs (i<j) with prefix[j] > prefix[i]
        // where prefix[0]=0, prefix[k]=sum_{t=1..k} v[t]
        vector<long long> pref(m + 1, 0);
        for (int i = 1; i <= m; i++) pref[i] = pref[i - 1] + v[i];
        int parity = parityIncreasingPairs(pref);
        return parity == 1;
    };

    // If unknown is on an endpoint, it cannot influence the game at all.
    if (P == 1 || P == N || m == 0) {
        return winningForFixedInternal() ? (2 * F + 1) : 0;
    }

    // Unknown is an internal position: internal index p = P-1 in [1..m]
    int p = P - 1;

    // Build T[k] = prefix sums with v[p] treated as 0
    vector<long long> T(m + 1, 0);
    for (int i = 1; i <= m; i++) {
        long long add = (i == p) ? 0LL : v[i];
        T[i] = T[i - 1] + add;
    }

    // Constant parities A (within first part 0..p-1) and B (within second part p..m)
    vector<long long> leftPref;
    leftPref.reserve(p);
    for (int i = 0; i <= p - 1; i++) leftPref.push_back(T[i]);

    vector<long long> rightPref;
    rightPref.reserve(m - p + 1);
    for (int j = p; j <= m; j++) rightPref.push_back(T[j]);

    int A = parityIncreasingPairs(leftPref);
    int B = parityIncreasingPairs(rightPref);
    int K = A ^ B; // C(x) parity = K xor D(x) parity

    // Cross-pair parity D(x): for each (i in [0..p-1], j in [p..m]),
    // contributes 1 iff x > (T[i] - T[j]).
    // We only need toggle points where D(x) changes parity: at x = (T[i]-T[j]) + 1.
    unordered_map<long long, int> odd; // store parity of multiplicity for each h = T[i]-T[j]
    odd.reserve((size_t)p * (size_t)(m - p + 1) * 2);
    odd.max_load_factor(0.7f);

    for (int i = 0; i <= p - 1; i++) {
        long long Li = T[i];
        for (int j = p; j <= m; j++) {
            long long h = Li - T[j];
            odd[h] ^= 1;
        }
    }

    vector<long long> toggles;
    toggles.reserve(odd.size());
    for (auto& kv : odd) {
        if (kv.second & 1) {
            // toggle at b = h+1, but be careful about overflow (values are within +/- 2e15 here)
            toggles.push_back(kv.first + 1);
        }
    }
    sort(toggles.begin(), toggles.end());

    auto parityAt = [&](long long x) -> int {
        // D(x) parity = (#toggle <= x) mod2 because each odd-mult threshold h contributes for x>=h+1
        long long cnt = (long long)(upper_bound(toggles.begin(), toggles.end(), x) - toggles.begin());
        return (int)(cnt & 1LL);
    };

    long long lo = -F, hi = F;
    long long ans = 0;

    long long curX = lo;
    int curD = parityAt(curX);
    int curWin = (K ^ curD);

    size_t idx = upper_bound(toggles.begin(), toggles.end(), curX) - toggles.begin();
    while (curX <= hi) {
        long long nextToggle = (idx < toggles.size()) ? toggles[idx] : (hi + 1);
        long long segR = min(hi, nextToggle - 1);
        if (segR >= curX && curWin == 1) ans += (segR - curX + 1);
        curX = nextToggle;
        if (curX > hi) break;
        curD ^= 1;
        curWin ^= 1;
        idx++;
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    for (int tc = 1; tc <= T; tc++) {
        int N, P;
        cin >> N >> P;
        vector<long long> buckets(N + 1, 0); // 1-indexed
        long long F = 0;
        for (int i = 1; i <= N; i++) {
            long long x;
            cin >> x;
            if (i == P) F = x;
            else buckets[i] = x;
        }

        long long ans = countWinningX(F, N, P, buckets);
        cout << "Case #" << tc << ": " << ans << "\n";
    }
    return 0;
}


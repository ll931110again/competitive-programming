#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

static long long simulate_order(const vector<int> &order, int B) {
    long long hp = 0;
    int block = B;
    for (int d : order) {
        const int absorb = min(block, d);
        block -= absorb;
        const int raw = d - absorb;
        if (raw > 0) {
            hp += max(raw, 5);
        }
    }
    return hp;
}

static long long brute_best(vector<int> arrows, int B) {
    sort(arrows.begin(), arrows.end());
    long long best = 0;
    do {
        best = max(best, simulate_order(arrows, B));
    } while (next_permutation(arrows.begin(), arrows.end()));
    return best;
}

static long long hill_climb(vector<int> ord, int B, uint64_t seed) {
    long long cur = simulate_order(ord, B);
    mt19937 rng(seed);
    const int n = (int)ord.size();

    for (int round = 0; round < 2; round++) {
        bool improved = true;
        while (improved) {
            improved = false;
            for (int i = 0; i + 1 < n; i++) {
                swap(ord[i], ord[i + 1]);
                const long long v = simulate_order(ord, B);
                if (v > cur) {
                    cur = v;
                    improved = true;
                } else {
                    swap(ord[i], ord[i + 1]);
                }
            }
        }
        shuffle(ord.begin(), ord.end(), rng);
        cur = max(cur, simulate_order(ord, B));
    }

    for (int it = 0; it < 40000; it++) {
        const int i = uniform_int_distribution<int>(0, n - 1)(rng);
        const int j = uniform_int_distribution<int>(0, n - 1)(rng);
        if (i == j) {
            continue;
        }
        swap(ord[i], ord[j]);
        const long long v = simulate_order(ord, B);
        if (v >= cur) {
            cur = v;
        } else {
            swap(ord[i], ord[j]);
        }
    }

    return cur;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, B;
    cin >> N >> B;
    vector<int> cnt(N + 1, 0);
    for (int i = 1; i <= N; i++) {
        cin >> cnt[i];
    }

    vector<int> arrows;
    arrows.reserve(200000);
    int total = 0;
    for (int d = 1; d <= N; d++) {
        total += cnt[d];
        for (int k = 0; k < cnt[d]; k++) {
            arrows.push_back(d);
        }
    }

    if (total == 0) {
        cout << 0 << '\n';
        return 0;
    }

    if (total <= 11) {
        cout << brute_best(arrows, B) << '\n';
        return 0;
    }

    vector<int> asc = arrows;
    sort(asc.begin(), asc.end());
    vector<int> desc = asc;
    reverse(desc.begin(), desc.end());

    long long ans = simulate_order(asc, B);
    ans = max(ans, simulate_order(desc, B));

    const vector<uint64_t> seeds = {0x9e3779b97f4a7c15ULL, 0xbf58476d1ce4e5b9ULL,
                                    0x94d049bb133111ebULL};
    for (uint64_t s : seeds) {
        ans = max(ans, hill_climb(asc, B, s));
        ans = max(ans, hill_climb(desc, B, s ^ 1ULL));
    }

    cout << ans << '\n';
    return 0;
}

#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Char at position p after left-cyclic-shifting S[ls..rs] by 1.
static inline char char_after_shift(const string &S, int ls, int rs, int p) {
    if (p < ls || p > rs) return S[(size_t)p];
    const int m = rs - ls + 1;
    const int off = p - ls;
    return S[(size_t)(ls + (off + 1) % m)];
}

// suffix_even_ub[i] = max even palindrome length centered at any j >= i (loose bound 2*min(j, n-j)).
// Lets Manacher abort early when no remaining center can beat the running best.
static vector<int> build_suffix_ub(int n) {
    vector<int> ub((size_t)n + 1, 0);
    for (int i = n - 1; i >= 0; --i) {
        const int cap = 2 * min(i, n - i);
        ub[(size_t)i] = max(cap, ub[(size_t)(i + 1)]);
    }
    return ub;
}

// Manacher (even-length only). Returns max(floor_best, true max even palindrome length).
// Initializing best_here to floor_best lets us prune any center whose maximum possible
// even palindrome length (suffix_even_ub[i]) cannot exceed the global best so far.
template <class F>
static int longest_even_manacher(int n, F &&getch, const vector<int> &suffix_even_ub,
                                 vector<int> &d2, int floor_best) {
    d2.assign((size_t)n, 0);
    int best_here = floor_best;
    int l = 0, r = -1;

    for (int i = 0; i < n; i++) {
        if (suffix_even_ub[(size_t)i] <= best_here) break;

        int k = (i > r) ? 0 : min(d2[(size_t)(l + r - i + 1)], r - i + 1);
        while (i + k < n && i - k - 1 >= 0 &&
               getch(i + k) == getch(i - k - 1)) {
            ++k;
        }
        d2[(size_t)i] = k;
        if (2 * k > best_here) best_here = 2 * k;
        if (i + k - 1 > r) {
            l = i - k;
            r = i + k - 1;
        }
    }
    return best_here;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    string S;
    cin >> N >> S;
    const int n = (int)S.size();

    const vector<int> suffix_even_ub = build_suffix_ub(n);
    vector<int> d2_buf;

    int best = longest_even_manacher(
        n, [&](int p) -> char { return S[(size_t)p]; },
        suffix_even_ub, d2_buf, 0);

    int out_L = -1, out_R = -1;

    // Try every left-cyclic shift; feed current best as the lower bound so each call
    // prunes positions that cannot improve on it (and short-circuits entirely once
    // suffix_even_ub[0] <= best).
    for (int ls = 0; ls < n; ++ls) {
        if (suffix_even_ub[0] <= best) break;
        for (int rs = ls + 1; rs < n; ++rs) {
            const int prev = best;
            const int v = longest_even_manacher(
                n,
                [&](int p) -> char { return char_after_shift(S, ls, rs, p); },
                suffix_even_ub, d2_buf, best);
            if (v > prev) {
                best = v;
                out_L = ls;
                out_R = rs;
                if (best == n) break;  // can't do better than full length
            }
        }
        if (best == n) break;
    }

    if (best == 0) {
        cout << -1 << '\n';
        return 0;
    }

    cout << best << '\n';
    if (out_L < 0) {
        cout << -1 << '\n';
    } else {
        cout << (out_L + 1) << ' ' << (out_R + 1) << '\n';
    }
    return 0;
}

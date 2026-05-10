#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static inline char char_after_shift(const string &S, int /*n*/, int ls, int rs, int p) {
    if (p < ls || p > rs) {
        return S[(size_t)p];
    }
    const int m = rs - ls + 1;
    const int off = p - ls;
    const int idx = ls + ((off + 1) % m);
    return S[(size_t)idx];
}

static vector<int> build_suffix_ub(int n) {
    vector<int> suffix_even_ub((size_t)n + 1, 0);
    for (int i = n - 1; i >= 0; --i) {
        const int cap = 2 * min(i, n - i);
        suffix_even_ub[(size_t)i] = max(cap, suffix_even_ub[(size_t)(i + 1)]);
    }
    return suffix_even_ub;
}

// Exists even palindrome of length >= target?
template <class F>
static bool has_even_palindrome_ge(int n, F &&getch, int target,
                                   const vector<int> &suffix_even_ub,
                                   vector<int> &d2) {
    if (target <= 0) {
        return true;
    }

    d2.assign((size_t)n, 0);
    int best_here = 0;
    int l = 0, r = -1;

    for (int i = 0; i < n; i++) {
        if (max(best_here, suffix_even_ub[(size_t)i]) < target) {
            break;
        }

        int k = (i > r) ? 0 : min(d2[(size_t)(l + r - i + 1)], r - i + 1);

        while (i + k < n && i - k - 1 >= 0 &&
               getch(i + k) == getch(i - k - 1)) {
            ++k;
            if (2 * k >= target) {
                return true;
            }
        }

        d2[(size_t)i] = k;
        best_here = max(best_here, 2 * d2[(size_t)i]);
        if (best_here >= target) {
            return true;
        }
        if (i + k - 1 > r) {
            l = i - k;
            r = i + k - 1;
        }
    }
    return false;
}

// Longest even palindrome; if stop_at >= 0, stop once max length reaches stop_at (optional micro-opt).
template <class F>
static int longest_even_manacher(int n, F &&getch, const vector<int> &suffix_even_ub,
                                 vector<int> &d2, int stop_at) {
    d2.assign((size_t)n, 0);
    int best_here = 0;
    int l = 0, r = -1;

    for (int i = 0; i < n; i++) {
        const int prune_below =
            (stop_at >= 0) ? (stop_at - 1) : -1;
        if (prune_below >= 0 &&
            max(best_here, suffix_even_ub[(size_t)i]) <= prune_below) {
            break;
        }

        int k = (i > r) ? 0 : min(d2[(size_t)(l + r - i + 1)], r - i + 1);
        while (i + k < n && i - k - 1 >= 0 &&
               getch(i + k) == getch(i - k - 1)) {
            ++k;
        }
        d2[(size_t)i] = k;
        best_here = max(best_here, 2 * d2[(size_t)i]);
        if (stop_at >= 0 && best_here >= stop_at) {
            return best_here;
        }
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

    auto can_ge = [&](int L) -> bool {
        if (has_even_palindrome_ge(
                n, [&](int p) -> char { return S[(size_t)p]; }, L,
                suffix_even_ub, d2_buf)) {
            return true;
        }
        for (int ls = 0; ls < n; ++ls) {
            for (int rs = ls + 1; rs < n; ++rs) {
                if (has_even_palindrome_ge(
                        n,
                        [&](int p) -> char {
                            return char_after_shift(S, n, ls, rs, p);
                        },
                        L, suffix_even_ub, d2_buf)) {
                    return true;
                }
            }
        }
        return false;
    };

    int lo_half = 0, hi_half = n / 2;
    while (lo_half < hi_half) {
        const int mid = (lo_half + hi_half + 1) / 2;
        if (can_ge(2 * mid)) {
            lo_half = mid;
        } else {
            hi_half = mid - 1;
        }
    }

    const int best = 2 * lo_half;
    if (best == 0) {
        cout << -1 << '\n';
        return 0;
    }

    const int base_best = longest_even_manacher(
        n, [&](int p) -> char { return S[(size_t)p]; }, suffix_even_ub, d2_buf,
        -1);

    bool use_shift = base_best < best;
    int out_L = 0, out_R = 0;

    if (use_shift) {
        bool found = false;
        for (int ls = 0; ls < n && !found; ++ls) {
            for (int rs = ls + 1; rs < n; ++rs) {
                const int v = longest_even_manacher(
                    n,
                    [&](int p) -> char {
                        return char_after_shift(S, n, ls, rs, p);
                    },
                    suffix_even_ub, d2_buf, best);
                if (v >= best) {
                    out_L = ls;
                    out_R = rs;
                    found = true;
                    break;
                }
            }
        }
    }

    cout << best << '\n';
    if (!use_shift) {
        cout << -1 << '\n';
    } else {
        cout << (out_L + 1) << ' ' << (out_R + 1) << '\n';
    }
    return 0;
}

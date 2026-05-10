#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <class F>
static int longest_even_manacher(int n, F getch) {
    vector<int> d2(n, 0);
    int best = 0;
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n &&
               getch(i - k - 1) == getch(i + k)) {
            k++;
        }
        d2[i] = k--;
        best = max(best, 2 * d2[i]);
        if (i + k > r) {
            l = i - k;
            r = i + k;
        }
    }
    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    string S;
    cin >> N >> S;

    auto base_get = [&](int p) -> char { return S[p]; };
    const int base_best = longest_even_manacher(N, base_get);

    int best = base_best;
    bool use_shift = false;
    int out_L = 0, out_R = 0;

    string T;
    T.resize((size_t)N);

    for (int L = 0; L < N; L++) {
        for (int R = L + 1; R < N; R++) {
            // Left cyclic shift by 1 on S[L..R]: T[L..R-1]=S[L+1..R], T[R]=S[L].
            for (int i = 0; i < L; i++) {
                T[i] = S[i];
            }
            for (int i = L; i < R; i++) {
                T[i] = S[i + 1];
            }
            T[R] = S[L];
            for (int i = R + 1; i < N; i++) {
                T[i] = S[i];
            }

            const int v = longest_even_manacher(N, [&](int p) -> char { return T[p]; });
            if (v > best) {
                best = v;
                use_shift = true;
                out_L = L;
                out_R = R;
            }
        }
    }

    if (best == 0) {
        cout << -1 << '\n';
        return 0;
    }

    cout << best << '\n';
    if (!use_shift) {
        cout << -1 << '\n';
    } else {
        cout << (out_L + 1) << ' ' << (out_R + 1) << '\n';
    }
    return 0;
}

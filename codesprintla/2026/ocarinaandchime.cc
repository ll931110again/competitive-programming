/**
 * Ocarina and Chime — harmonious starts.
 *
 * The solution is that, for a distribution to be uniform
 * we need to check that sum w^j (mel[i] + ch[s+i]) is 0 for j = 1..k-1.
 */

#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#else
#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>
#endif

#ifdef __GNUC__
#pragma GCC optimize("O3")
#endif

using namespace std;

using cf = complex<float>;
using cd = complex<double>;
static const float PI = acosf(-1.0f);

static inline int mod_k(int x, int k) {
    x %= k;
    return x < 0 ? x + k : x;
}

static vector<vector<cf>> tw_fwd, tw_inv;

static void build_twiddles(int n) {
    tw_fwd.assign(n + 1, {});
    tw_inv.assign(n + 1, {});
    for (int len = 2; len <= n; len <<= 1) {
        const int half = len / 2;
        tw_fwd[len].resize(half);
        tw_inv[len].resize(half);
        for (int j = 0; j < half; ++j) {
            float ang = 2 * PI * (float)j / (float)len;
            tw_fwd[len][j] = cf(cosf(ang), sinf(ang));
            ang = -ang;
            tw_inv[len][j] = cf(cosf(ang), sinf(ang));
        }
    }
}

static vector<int> rev_perm;

static void fft_inplace(vector<cf>& a, bool invert) {
    const int n = (int)a.size();
    for (int i = 0; i < n; ++i)
        if (i < rev_perm[i]) swap(a[i], a[rev_perm[i]]);

    for (int len = 2; len <= n; len <<= 1) {
        const int half = len / 2;
        const cf* tw = invert ? tw_inv[len].data() : tw_fwd[len].data();
        for (int i = 0; i < n; i += len) {
            cf* ai = a.data() + i;
            for (int j = 0; j < half; ++j) {
                cf u = ai[j];
                cf v = ai[j + half] * tw[j];
                ai[j] = u + v;
                ai[j + half] = u - v;
            }
        }
    }

    if (invert) {
        const float inv_n = 1.0f / (float)n;
        for (cf& x : a) x *= inv_n;
    }
}

static void convolution_complex(int n, vector<cf>& a, vector<cf>& b) {
    fft_inplace(a, false);
    fft_inplace(b, false);
    for (int i = 0; i < n; ++i) a[i] *= b[i];
    fft_inplace(a, true);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int M, N, k;
    cin >> M >> N >> k;
    vector<int> ch(M), mel(N);
    for (int i = 0; i < M; i++) cin >> ch[i];
    for (int i = 0; i < N; i++) cin >> mel[i];

    if (N > M) {
        cout << "0\n\n";
        return 0;
    }

    if (k == 1) {
        const int cnt = M - N + 1;
        cout << cnt << '\n';
        for (int i = 0; i < cnt; i++) {
            if (i) cout << ' ';
            cout << (i + 1);
        }
        cout << '\n';
        return 0;
    }

    if (N % k != 0) {
        cout << "0\n\n";
        return 0;
    }

    for (int& x : mel) x = mod_k(x, k);
    for (int& x : ch) x = mod_k(x, k);

    // Filter threshold for float FFT. Must be large enough to avoid false negatives
    // (true harmonious => 0, but numerical error can be nontrivial at size ~1e6).
    // We use an uncertainty band: if |S| is medium, recompute exactly (double) before eliminating.
    const float FILTER_BAD_HI = 0.08f;
    const float FILTER_BAD_LO = 0.02f;

    vector<vector<cf>> W(k, vector<cf>(k));
    vector<vector<cd>> Wd(k, vector<cd>(k));
    for (int j = 1; j < k; ++j) {
        for (int r = 0; r < k; ++r) {
            float ang = 2 * PI * (float)j * (float)r / (float)k;
            W[j][r] = cf(cosf(ang), sinf(ang));
            Wd[j][r] = cd((double)W[j][r].real(), (double)W[j][r].imag());
        }
    }

    int fft_len = 1;
    while (fft_len < M + N) fft_len <<= 1;

    build_twiddles(fft_len);
    {
        const int lg = __builtin_ctz(fft_len);
        rev_perm.resize(fft_len);
        for (int i = 0; i < fft_len; ++i)
            rev_perm[i] = (rev_perm[i >> 1] >> 1) | ((i & 1) << (lg - 1));
    }

    const int S = M - N + 1;
    const int J = k / 2;
    const int off = N - 1;

    // 1) FFT filter over j, eliminate only when clearly nonzero.
    vector<char> cand(S, 1);
    int alive = S;
    vector<cf> a_fft(fft_len), b_fft(fft_len);

    for (int j = 1; j <= J && alive > 0; ++j) {
        fill(a_fft.begin(), a_fft.end(), cf(0.0f, 0.0f));
        fill(b_fft.begin(), b_fft.end(), cf(0.0f, 0.0f));
        for (int i = 0; i < N; ++i) a_fft[i] = W[j][mel[N - 1 - i]];
        for (int t = 0; t < M; ++t) b_fft[t] = W[j][ch[t]];
        convolution_complex(fft_len, a_fft, b_fft);
        for (int s = 0; s < S && alive > 0; ++s) {
            if (!cand[s]) continue;
            float mag = abs(a_fft[s + off]);
            if (mag <= FILTER_BAD_LO) continue; // definitely keep
            if (mag > FILTER_BAD_HI) {
                cand[s] = 0;
                --alive;
                continue;
            }
            // Uncertain: verify this single (s,j) sum exactly in double before eliminating.
            cd sum(0.0, 0.0);
            for (int i = 0; i < N; ++i) {
                int r = mel[i] + ch[s + i];
                if (r >= k) r -= k;
                sum += Wd[j][r];
            }
            if (abs(sum) > 1e-5) {
                cand[s] = 0;
                --alive;
            }
        }
    }

    // 2) Exact verification for survivors.
    vector<int> ans;
    ans.reserve(alive);
    const int need = N / k;
    vector<int> cnt(k);
    for (int s = 0; s < S; ++s) {
        if (!cand[s]) continue;
        fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i < N; ++i) {
            int r = mel[i] + ch[s + i];
            if (r >= k) r -= k;
            cnt[r]++;
        }
        bool ok = true;
        for (int r = 0; r < k; ++r) {
            if (cnt[r] != need) { ok = false; break; }
        }
        if (ok) ans.push_back(s + 1);
    }

    cout << ans.size() << '\n';
    if (!ans.empty()) {
        for (size_t i = 0; i < ans.size(); i++) {
            if (i) cout << ' ';
            cout << ans[i];
        }
    }
    cout << '\n';
    return 0;
}

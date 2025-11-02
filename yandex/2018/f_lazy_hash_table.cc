#include <bits/stdc++.h>
using namespace std;
using ll = long long;
 
const int MOD = 998244353;
const int G = 3;
 
int mod_pow(ll a, ll e) {
    ll r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return int(r);
}
 
void ntt(vector<int> &a, bool invert) {
    int n = a.size();
    static vector<int> rev;
    if ((int)rev.size() != n) {
        int lg = __builtin_ctz(n);
        rev.assign(n, 0);
        for (int i = 0; i < n; ++i)
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (lg - 1));
    }
    for (int i = 0; i < n; ++i) if (i < rev[i]) swap(a[i], a[rev[i]]);
 
    for (int len = 1; len < n; len <<= 1) {
        int wlen = mod_pow(G, (MOD - 1) / (len << 1));
        if (invert) wlen = mod_pow(wlen, MOD - 2);
        for (int i = 0; i < n; i += (len << 1)) {
            int w = 1;
            for (int j = 0; j < len; ++j) {
                int u = a[i + j];
                int v = (int)((1LL * a[i + j + len] * w) % MOD);
                int uu = u + v;
                if (uu >= MOD) uu -= MOD;
                int vv = u - v;
                if (vv < 0) vv += MOD;
                a[i + j] = uu;
                a[i + j + len] = vv;
                w = (int)((1LL * w * wlen) % MOD);
            }
        }
    }
 
    if (invert) {
        int inv_n = mod_pow(n, MOD - 2);
        for (int &x : a) x = (int)(1LL * x * inv_n % MOD);
    }
}
 
vector<int> convolution(const vector<int> &a, const vector<int> &b) {
    int need = (int)a.size() + (int)b.size() - 1;
    int n = 1;
    while (n < need) n <<= 1;
    vector<int> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    fa.resize(n);
    fb.resize(n);
    ntt(fa, false);
    ntt(fb, false);
    for (int i = 0; i < n; ++i) fa[i] = (int)(1LL * fa[i] * fb[i] % MOD);
    ntt(fa, true);
    fa.resize(need);
    return fa;
}
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n);
    int maxA = 0;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        maxA = max(maxA, a[i]);
    }
 
    vector<int> P(maxA + 1, 0);
    for (int x : a) P[x] = 1;
 
    vector<int> R(maxA + 1);
    for (int i = 0; i <= maxA; ++i) R[i] = P[maxA - i];
 
    vector<int> C = convolution(P, R); // size ~ 2*(maxA+1)-1
 
    // hasDiff[d] = true if there exists a pair with absolute difference d (d>=1)
    vector<char> hasDiff(maxA + 1, 0);
    for (int d = 1; d <= maxA; ++d) {
        int idx = maxA - d;
        if (idx >= 0 && idx < (int)C.size() && C[idx]) hasDiff[d] = 1;
    }
 
    int M = maxA + 1;
    vector<char> bad(M + 1, 0);
 
    // Correct marking: m is bad if there exists a multiple x = k*m (1 <= x <= maxA) with hasDiff[x]
    for (int m = 1; m <= M; ++m) {
        for (int x = m; x <= maxA; x += m) {
            if (hasDiff[x]) { bad[m] = 1; break; }
        }
        // Note: when m == M (maxA+1), loop body never runs, so bad[M] stays 0.
    }
 
    int ans = -1;
    for (int m = max(n, 1); m <= M; ++m) {
        if (!bad[m]) { ans = m; break; }
    }
    if (ans == -1) ans = M;
    cout << ans << '\n';
    return 0;
}

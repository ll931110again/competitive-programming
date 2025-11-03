#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#include <queue>
#define maxn 900005
#define mod 1000000007
using namespace std;

int T, n, m;
int a[maxn], b[maxn];

long long fact[maxn], inv[maxn];

long long get_power(long long x, long long p) {
    if (!p) {
        return 1;
    }
    long long q = get_power(x, p/2);
    q = (q * q) % mod;
    if (p & 1) {
        q = (q * x) % mod;
    }

    return q;
}

long long binom(int x, int y) {
    long long up = fact[x];
    long long down = inv[y] * inv[x - y] % mod;
    return up * down % mod;
}

long long solve() {
    int X = (3 * n + 1) / 2;
    int pos = -1;

    for (int i = 0; i < m; i++) if (b[i] == X) {
        pos = a[i];
    }

    int segment = -1;
    if (pos >= 0) {
        if (pos <= n) {
            segment = 1;
        } else if (pos <= 2 * n) {
            segment = 2;
        } else {
            segment = 3;
        }
    }

    long long ret = 0;
    for (int value = 1; value <= 3; value++) if (segment < 0 || segment == value) {
        int low = (value - 1) * n + 1, high = low + n - 1;

        int lc = n / 2, hc = n / 2, slots_outside = 2 * n;
        for (int i = 0; i < m; i++) {
            if (low <= a[i] && a[i] <= high) {
                if (b[i] < X) {
                    lc--;
                } else if (b[i] > X) {
                    hc--;
                }
            } else {
                slots_outside--;
            }
        }

        long long count_segment = 0;
        if (lc >= 0 && hc >= 0) {
            count_segment = binom(X - 1, lc) * binom(X - 1, hc) % mod;
            count_segment = (count_segment * fact[n / 2 - lc]) % mod;
            count_segment = (count_segment * fact[n / 2 - hc]) % mod;
            if (segment < 0) {
                count_segment = count_segment * n % mod;
            }
        }
        long long count_remains = fact[slots_outside];
        ret += (count_segment * count_remains) % mod;
    }

    return ret % mod;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    fact[0] = 1;
    for (int i = 1; i < maxn; i++) {
        fact[i] = fact[i - 1] * i % mod;
    }

    for (int i = 0; i < maxn; i++) {
        inv[i] = get_power(fact[i], mod - 2);
    }

    cin >> T;
    while (T--) {
        cin >> n >> m;
        for (int i = 0; i < m; i++) {
            cin >> a[i] >> b[i];
        }
        cout << solve() << endl;
    }

	return 0;
}

#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#define maxn 1005
using namespace std;

int n, b;
int num_arrows[maxn];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> b;
    for (int i = 1; i <= n; i++) {
        cin >> num_arrows[i];
    }

    long long cnt_small = 0;
    for (int i = 1; i < 5; i++) {
        cnt_small += num_arrows[i];
    }

    long long total_large = 0;
    for (int i = 5; i <= n; i++) {
        total_large += i * num_arrows[i];
    }

    int bound = 2 * maxn - 1;

    bool stopped = false;
    for (int i = n; i >= 5; i--) {
        if (stopped) {
            break;
        }

        while (num_arrows[i] > 0) {
            if (b >= bound) {
                b -= i;
                total_large -= i;
                num_arrows[i]--;
            } else {
                stopped = true;
                break;
            }
        }
    }

    bool cur[2 * maxn], nxt[2 * maxn];
    memset(cur, false, sizeof cur);
    memset(nxt, false, sizeof nxt);
    cur[0] = true;

    for (int i = 5; i <= n; i++) {
        for (int j = 0; j < num_arrows[i]; j++) {
            for (int t = 0; t <= bound; t++) {
                nxt[t] = cur[t];
                if (t >= i) {
                    nxt[t] |= cur[t - i];
                }
            }
            for (int t = 0; t <= n; t++) {
                cur[t] = nxt[t];
            }
        }
    }

    long long total_damage = 0;
    for (int t = 0; t <= bound && t <= total_large; t++) if (cur[t]) {
        // use smallest number of light arrows to exceed b - t damages
        long long rem = b - t, cnt = 0;
        for (int j = 4; j > 0; j--) {
            if (rem < 0) {
                break;
            }
            int amount = min(rem, (long long) num_arrows[j] * j) / j;
            cnt += amount;
            rem -= amount * j;
            if (amount < num_arrows[j]) {
                cnt++;
                rem -= j;
            }
        }

        if (rem >= 0) {
            continue;
        }

        long long damage = 0;
        if (t > b) {
            damage = (total_large - t) + 5 + cnt_small * 5;
        } else {
            if (cnt == 0) {
                damage = (total_large - t) + cnt_small * 5;
            } else {
                damage = (total_large - t) + (cnt_small - cnt) * 5 + 5;
            }
        }

        total_damage = max(total_damage, damage);
    }

    cout << total_damage << endl;
    return 0;
}
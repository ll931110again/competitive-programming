#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int M, N, k;
    cin >> M >> N >> k;
    vector<int> a(M), b(N);
    for (int i = 0; i < M; i++) {
        cin >> a[i];
    }
    for (int i = 0; i < N; i++) {
        cin >> b[i];
    }

    if (N % k != 0 || M < N) {
        cout << "0\n\n";
        return 0;
    }

    const int need = N / k;
    vector<int> cnt(k, 0);

    auto add = [&](int x) { cnt[x]++; };
    auto rem = [&](int x) { cnt[x]--; };
    auto ok = [&]() {
        for (int t = 0; t < k; t++) {
            if (cnt[t] != need) {
                return false;
            }
        }
        return true;
    };

    for (int j = 0; j < N; j++) {
        add((a[j] + b[j]) % k);
    }

    vector<int> ans;
    if (ok()) {
        ans.push_back(1);
    }

    for (int start = 2; start <= M - N + 1; start++) {
        const int i = start - 1;
        rem((a[i - 1] + b[0]) % k);
        add((a[i + N - 1] + b[N - 1]) % k);
        if (ok()) {
            ans.push_back(start);
        }
    }

    cout << ans.size() << '\n';
    if (!ans.empty()) {
        for (size_t i = 0; i < ans.size(); i++) {
            if (i) {
                cout << ' ';
            }
            cout << ans[i];
        }
    }
    cout << '\n';
    return 0;
}

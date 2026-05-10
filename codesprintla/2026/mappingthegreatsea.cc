#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif
#include <iostream>
#include <vector>

using namespace std;

struct Rect {
    int r1, r2, c1, c2;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<vector<int>> g(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> g[i][j];
        }
    }

    auto maximal_rect = [&](int r1, int r2, int c1, int c2) -> bool {
        for (int i = r1; i <= r2; i++) {
            for (int j = c1; j <= c2; j++) {
                if (!g[i][j]) {
                    return false;
                }
            }
        }
        auto all_up = [&]() -> bool {
            if (r1 == 0) {
                return false;
            }
            for (int j = c1; j <= c2; j++) {
                if (!g[r1 - 1][j]) {
                    return false;
                }
            }
            return true;
        };
        auto all_down = [&]() -> bool {
            if (r2 + 1 >= n) {
                return false;
            }
            for (int j = c1; j <= c2; j++) {
                if (!g[r2 + 1][j]) {
                    return false;
                }
            }
            return true;
        };
        auto all_left = [&]() -> bool {
            if (c1 == 0) {
                return false;
            }
            for (int i = r1; i <= r2; i++) {
                if (!g[i][c1 - 1]) {
                    return false;
                }
            }
            return true;
        };
        auto all_right = [&]() -> bool {
            if (c2 + 1 >= n) {
                return false;
            }
            for (int i = r1; i <= r2; i++) {
                if (!g[i][c2 + 1]) {
                    return false;
                }
            }
            return true;
        };
        return !all_up() && !all_down() && !all_left() && !all_right();
    };

    vector<Rect> rects;
    rects.reserve((size_t)n * n);

    for (int r1 = 0; r1 < n; r1++) {
        vector<char> good(n, 0);
        for (int j = 0; j < n; j++) {
            good[j] = (g[r1][j] == 1);
        }
        for (int r2 = r1; r2 < n; r2++) {
            if (r2 > r1) {
                for (int j = 0; j < n; j++) {
                    good[j] = (good[j] && (g[r2][j] == 1));
                }
            }
            int c = 0;
            while (c < n) {
                if (!good[c]) {
                    c++;
                    continue;
                }
                const int c0 = c;
                while (c < n && good[c]) {
                    c++;
                }
                const int c1 = c - 1;
                if (maximal_rect(r1, r2, c0, c1)) {
                    rects.push_back({r1, r2, c0, c1});
                }
            }
        }
    }

    const int R = (int)rects.size();
    const int S = n + 2;
    vector<vector<int>> d(S, vector<int>(S, 0));
    for (const Rect &re : rects) {
        d[re.r1][re.c1] += 1;
        d[re.r1][re.c2 + 1] -= 1;
        d[re.r2 + 1][re.c1] -= 1;
        d[re.r2 + 1][re.c2 + 1] += 1;
    }

    vector<vector<int>> cov(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int v = d[i][j];
            if (i > 0) {
                v += cov[i - 1][j];
            }
            if (j > 0) {
                v += cov[i][j - 1];
            }
            if (i > 0 && j > 0) {
                v -= cov[i - 1][j - 1];
            }
            cov[i][j] = v;
        }
    }

    auto rect_has_exclusive = [&](const Rect &re) -> bool {
        for (int i = re.r1; i <= re.r2; i++) {
            for (int j = re.c1; j <= re.c2; j++) {
                if (cov[i][j] == 1) {
                    return true;
                }
            }
        }
        return false;
    };

    long long type1 = R;
    long long type2 = 0;
    for (const Rect &re : rects) {
        if (rect_has_exclusive(re)) {
            type2++;
        }
    }

    cout << type1 << ' ' << type2 << '\n';
    return 0;
}

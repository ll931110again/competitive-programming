#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif
#include <iostream>
#include <vector>
#include <tuple>
#include <map>

using namespace std;

struct Cut {
    char t;
    int a, b, c;
};

static int N, M, Kglobal;
static int query_budget = 0;

static int ask(int x1, int x2, int y1, int y2) {
    ++query_budget;
    cout << "? " << x1 << ' ' << x2 << ' ' << y1 << ' ' << y2 << '\n';
    cout.flush();
    int r;
    if (!(cin >> r)) {
        exit(0);
    }
    if (r < 0) {
        exit(0);
    }
    return r;
}

// Memoize oracle answers for axis-aligned boxes we actually query.
static map<tuple<int, int, int, int>, int> memo_q;

static int Q(int x1, int x2, int y1, int y2) {
    const auto key = make_tuple(x1, x2, y1, y2);
    auto it = memo_q.find(key);
    if (it != memo_q.end()) {
        return it->second;
    }
    const int v = ask(x1, x2, y1, y2);
    memo_q[key] = v;
    return v;
}

static vector<Cut> cuts_out;

static bool dfs(int xl, int xr, int yb, int yt, int k) {
    const int qfull = Q(xl, xr, yb, yt);
    if (k == 1) {
        return qfull == 0;
    }
    if (qfull == 0) {
        return false;
    }

    // Vertical split: line x for xl < x < xr; cut segment from (x, yb) to (x, yt).
    for (int x = xl + 1; x < xr; x++) {
        const int qL = Q(xl, x, yb, yt);
        const int qR = Q(x, xr, yb, yt);

        vector<pair<int, int>> cand;
        const bool zL = (qL == 0);
        const bool zR = (qR == 0);
        if (zL && zR) {
            continue;
        }
        if (zL && !zR) {
            cand.push_back({1, k - 1});
        } else if (!zL && zR) {
            cand.push_back({k - 1, 1});
        } else {
            for (int kL = 2; kL <= k - 2; kL++) {
                cand.push_back({kL, k - kL});
            }
        }

        for (auto [kL, kR] : cand) {
            if (kL <= 0 || kR <= 0 || kL + kR != k) {
                continue;
            }
            const bool okL = (kL == 1) == (qL == 0);
            const bool okR = (kR == 1) == (qR == 0);
            if (!okL || !okR) {
                continue;
            }
            const size_t before = cuts_out.size();
            cuts_out.push_back({'v', x, yb, yt});
            if (dfs(xl, x, yb, yt, kL) && dfs(x, xr, yb, yt, kR)) {
                return true;
            }
            cuts_out.resize(before);
        }
    }

    // Horizontal split: line y for yb < y < yt; cut from (xl, y) to (xr, y).
    for (int y = yb + 1; y < yt; y++) {
        const int qD = Q(xl, xr, yb, y);
        const int qU = Q(xl, xr, y, yt);

        vector<pair<int, int>> cand;
        const bool zD = (qD == 0);
        const bool zU = (qU == 0);
        if (zD && zU) {
            continue;
        }
        if (zD && !zU) {
            cand.push_back({1, k - 1});
        } else if (!zD && zU) {
            cand.push_back({k - 1, 1});
        } else {
            for (int kD = 2; kD <= k - 2; kD++) {
                cand.push_back({kD, k - kD});
            }
        }

        for (auto [kD, kU] : cand) {
            if (kD <= 0 || kU <= 0 || kD + kU != k) {
                continue;
            }
            const bool okD = (kD == 1) == (qD == 0);
            const bool okU = (kU == 1) == (qU == 0);
            if (!okD || !okU) {
                continue;
            }
            const size_t before = cuts_out.size();
            cuts_out.push_back({'h', xl, xr, y});
            if (dfs(xl, xr, yb, y, kD) && dfs(xl, xr, y, yt, kU)) {
                return true;
            }
            cuts_out.resize(before);
        }
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> N >> M >> Kglobal)) {
        return 0;
    }

    cuts_out.clear();
    memo_q.clear();
    query_budget = 0;

    if (!dfs(0, N, 0, M, Kglobal)) {
        // Should not happen on judge data if invariants hold.
        cout << "! 0\n";
        cout.flush();
        return 0;
    }

    cout << "! " << cuts_out.size() << '\n';
    for (const Cut &ce : cuts_out) {
        if (ce.t == 'h') {
            cout << 'h' << ' ' << ce.a << ' ' << ce.b << ' ' << ce.c << '\n';
        } else {
            cout << 'v' << ' ' << ce.a << ' ' << ce.b << ' ' << ce.c << '\n';
        }
    }
    cout.flush();
    return 0;
}

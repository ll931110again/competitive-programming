#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>
#define maxn 200005
using namespace std;

namespace {
    int T, n, k;
    int a[maxn];
};

bool solve() {
    int pos = 0;
    for (int mex = 0; mex <= n; mex++) {
        int cnt = 0;
        while (pos < n && a[pos] == mex) {
            pos++;
            cnt++;
        }
        if (cnt == 2 * k - 1) {
            return true;
        }
        if (cnt <= 2 * k - 2) {
            return false;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> T;
    while (T--) {
        cin >> n >> k;
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        sort(a, a + n);
        cout << (solve() ? "YES" : "NO") << endl;
    }
    return 0;
}
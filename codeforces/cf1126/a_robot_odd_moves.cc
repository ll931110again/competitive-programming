#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>
using namespace std;

namespace {
    int T;
    int a, b;
};

int solve(int a, int b) {
    if (a + 1 < b) {
        return -1;
    }
    if ((a + b) % 2 == 0) {
        return a;
    }
    return a + 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> T;
    while (T--) {
        cin >> a >> b;
        cout << solve(a, b) << endl;
    }
    return 0;
}
// Codeforces Spectral::Cup 2026 Round 2 — Slimes on a Line
// https://codeforces.com/contest/2229/problem/A
//
// Sketch
// ------
// After sorting, the minimum possible max gap after equalizing toward the
// middle is ceil((max - min) / 2). O(n log n).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 1005;
int T, n;
int a[k_max_n];

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    sort(a, a + n);
    int ans = (a[n - 1] - a[0] + 1) / 2;
    cout << ans << endl;
  }

  return 0;
}

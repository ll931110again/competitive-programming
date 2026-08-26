// Codeforces Spectral::Cup 2026 Round 3 — Delete and Concatenate
// https://codeforces.com/contest/2245/problem/B
//
// Sketch
// ------
// Shift values by c, sort, and take the largest half (and any remaining
// positives). O(n log n).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int kMaxN = 200005;

int T, n, c;
int a[kMaxN];

int64_t solve() {
  for (int i = 0; i < n; i++) {
    a[i] -= c;
  }
  sort(a, a + n);

  int m = (n + 1) / 2;
  int64_t ans = 0;
  for (int i = n - 1; (i >= n - m) || (a[i] > 0); i--) {
    ans += a[i];
  }

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> c;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }

    cout << solve() << endl;
  }

  return 0;
}

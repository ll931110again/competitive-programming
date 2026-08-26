// Codeforces Spectral::Cup 2026 Round 1 — Wonderful Contest
// https://codeforces.com/contest/2222/problem/A
//
// Sketch
// ------
// YES iff some rating equals 100. O(n) per test.

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 15;
int T, n;

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    bool found = false;
    for (int i = 0; i < n; i++) {
      int x;
      cin >> x;
      if (x == 100) {
        found = true;
      }
    }
    cout << (found ? "Yes" : "No") << endl;
  }

  return 0;
}

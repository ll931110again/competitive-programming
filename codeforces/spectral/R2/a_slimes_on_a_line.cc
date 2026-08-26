// Codeforces Spectral::Cup 2026 Round 2 — Slimes on a Line
// https://codeforces.com/contest/2229/problem/A
//
// Sketch
// ------
// Every operation with the same x moves every slime one step toward x (or
// leaves it if it is already there). The final common position y is therefore
// reached most quickly by always choosing x = y, which takes max(y - min a,
// max a - y) operations. That quantity is minimized by taking y in the middle
// of [min a, max a], so the answer is ceil((max a - min a) / 2).
//
// Complexity: O(n) per test.

#include <bits/stdc++.h>
using namespace std;

namespace {

int min_operations(const vector<int>& positions) {
  const int lo = *min_element(positions.begin(), positions.end());
  const int hi = *max_element(positions.begin(), positions.end());
  return (hi - lo + 1) / 2;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int test_count;
  cin >> test_count;
  while (test_count--) {
    int n;
    cin >> n;
    vector<int> positions(n);
    for (int& x : positions) {
      cin >> x;
    }
    cout << min_operations(positions) << '\n';
  }
  return 0;
}

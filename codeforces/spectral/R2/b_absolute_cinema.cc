// Codeforces Spectral::Cup 2026 Round 2 — Absolute Cinema
// https://codeforces.com/contest/2229/problem/B
//
// Sketch
// ------
// Swapping at index i chooses which of (a_i, b_i) contributes to max(a) and
// which contributes to the sum of b. For every index the larger value should
// sit in b: it is always counted, whereas the smaller value is counted only if
// it is the unique contributor to max(a). The answer is therefore
// sum_i max(a_i, b_i) plus the largest of the per-index minima.
//
// Complexity: O(n) per test.

#include <bits/stdc++.h>
using namespace std;

namespace {

int64_t max_score(const vector<int>& a, const vector<int>& b) {
  int64_t sum_of_maxes = 0;
  int best_min = 0;
  for (size_t i = 0; i < a.size(); i++) {
    sum_of_maxes += max(a[i], b[i]);
    best_min = max(best_min, min(a[i], b[i]));
  }
  return sum_of_maxes + best_min;
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
    vector<int> a(n), b(n);
    for (int& x : a) {
      cin >> x;
    }
    for (int& x : b) {
      cin >> x;
    }
    cout << max_score(a, b) << '\n';
  }
  return 0;
}

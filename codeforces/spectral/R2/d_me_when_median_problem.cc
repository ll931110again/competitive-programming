// Codeforces Spectral::Cup 2026 Round 2 — Me When Median Problem
// https://codeforces.com/contest/2229/problem/D
//
// Sketch
// ------
// Binary-search the threshold m. After replacing every value by 1 if it is
// >= m and 0 otherwise, the question is whether both surviving entries can be
// 1. Each operation discards the min and max of four values, so the difference
// (# of 1s) - (# of 0s) changes only when all four bits are equal: a 0000
// increases it by 2 and a 1111 decreases it by 2.
//
// To maximize the difference, collapse every maximal run of columns that
// contain no 11 (only 00 and 01) into a single leftover 00 or 01 — each 00/00
// operation inside such a run is a profitable 0000. After that, the difference
// can no longer increase: any two 00 columns have a 11 between them. We win
// iff the number of 11 columns strictly exceeds the number of leftover 00
// groups, because then we can always operate without creating a 1111 until the
// two remaining values are 1.
//
// Complexity: O(n log A) per test.

#include <bits/stdc++.h>
using namespace std;

namespace {

bool can_reach(int threshold, const vector<int>& a, const vector<int>& b) {
  int ones = 0;
  int zeros = 0;
  int prev = -1;
  for (size_t i = 0; i < a.size(); i++) {
    const int type = (a[i] >= threshold) + (b[i] >= threshold);
    if (type == 1) {
      continue;
    }
    if (type == 2) {
      ones++;
      prev = 1;
    } else {
      if (prev != 0) {
        zeros++;
      }
      prev = 0;
    }
  }
  return ones > zeros;
}

int max_min_pair(const vector<int>& a, const vector<int>& b) {
  int lo = 0;
  int hi = 2'000'000'005;
  while (lo < hi) {
    const int mid = lo + (hi - lo) / 2;
    if (can_reach(mid, a, b)) {
      lo = mid + 1;
    } else {
      hi = mid;
    }
  }
  return lo - 1;
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
    cout << max_min_pair(a, b) << '\n';
  }
  return 0;
}

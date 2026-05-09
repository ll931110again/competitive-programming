// G – Hextech Ordnance
// https://codesprintla25.kattis.com/problems/hextechordnance
//
// Full constraints n ≤ 10^6 need an O(n log n) or O(n) characterization;
// this version uses an O(n^2) two-pointer count per k (fine only for small n).

#include <bits/stdc++.h>
using namespace std;

static long long count_range_le(const vector<int> &a, int k) {
  const int n = (int)a.size();
  long long cnt = 0;
  for (int i = 0; i < n; ++i) {
    int mn = a[i], mx = a[i];
    for (int j = i + 1; j < n; ++j) {
      mn = min(mn, a[j]);
      mx = max(mx, a[j]);
      if (mx - mn <= k)
        ++cnt;
      else
        break;
    }
  }
  return cnt;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  if (!(cin >> n))
    return 0;
  vector<int> a(n);
  for (int i = 0; i < n; ++i)
    cin >> a[i];

  for (int k = 1; k <= n; ++k) {
    if (count_range_le(a, k) < k) {
      cout << "unstable\n";
      return 0;
    }
  }
  cout << "stable\n";
  return 0;
}

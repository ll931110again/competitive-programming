// I – Hextech Ordnance
// https://open.kattis.com/problems/hextechordnance
//
// For each k ∈ [1, n], count subarrays of length ≥ 2 with (max − min) ≤ k.
// Stable iff every count ≥ k.
//
// Each count is O(n) via monotone deques + two pointers; overall O(n^2) worst case.
// Full constraints (n = 10^6) may require a tighter characterization — not implemented here.

#include <bits/stdc++.h>
using namespace std;

static long long count_range_le(const vector<int> &a, int K) {
  const int n = (int)a.size();
  long long cnt = 0;
  deque<int> maxq, minq;
  int j = 0;
  for (int i = 0; i < n; ++i) {
    while (j < n) {
      while (!maxq.empty() && a[maxq.back()] <= a[j])
        maxq.pop_back();
      maxq.push_back(j);
      while (!minq.empty() && a[minq.back()] >= a[j])
        minq.pop_back();
      minq.push_back(j);
      int mx = a[maxq.front()], mn = a[minq.front()];
      if (mx - mn <= K)
        ++j;
      else
        break;
    }
    long long len = j - i;
    if (len >= 2)
      cnt += len - 1;
    if (!maxq.empty() && maxq.front() == i)
      maxq.pop_front();
    if (!minq.empty() && minq.front() == i)
      minq.pop_front();
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
    if (count_range_le(a, k) < (long long)k) {
      cout << "unstable\n";
      return 0;
    }
  }
  cout << "stable\n";
  return 0;
}

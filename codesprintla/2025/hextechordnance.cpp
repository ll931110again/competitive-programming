// I – Hextech Ordnance
// https://open.kattis.com/problems/hextechordnance
//
// Stable iff for every k ∈ [1, n], there are ≥ k subarrays [i, j] with i < j,
// length ≥ 2, and max(a[i..j]) − min(a[i..j]) ≤ k.
//
// Monotone deques count F(k) in O(n). If k ≥ D = max(a) − min(a), every
// subarray qualifies: F(k) = n(n−1)/2.
//
// Early stop: if F(k) ≥ n, then for any k' ∈ (k, n], F(k') ≥ F(k) ≥ n ≥ k',
// so remaining checks are automatic once k ≤ n.

#include <bits/stdc++.h>
using namespace std;

namespace {

long long count_at_most_k(const vector<int>& a, long long K) {
  const int n = (int)a.size();
  deque<int> dq_min, dq_max;
  long long ans = 0;
  int l = 0;
  for (int r = 0; r < n; ++r) {
    while (!dq_min.empty() && a[dq_min.back()] >= a[r])
      dq_min.pop_back();
    dq_min.push_back(r);
    while (!dq_max.empty() && a[dq_max.back()] <= a[r])
      dq_max.pop_back();
    dq_max.push_back(r);

    while (!dq_min.empty() && !dq_max.empty() &&
           (long long)a[dq_max.front()] - (long long)a[dq_min.front()] > K) {
      if (dq_min.front() == l)
        dq_min.pop_front();
      if (dq_max.front() == l)
        dq_max.pop_front();
      ++l;
    }
    const int len = r - l + 1;
    if (len >= 2)
      ans += (long long)(len - 1);
  }
  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  if (!(cin >> n))
    return 0;
  vector<int> a(n);
  int mn = INT_MAX, mx = INT_MIN;
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
    mn = min(mn, a[i]);
    mx = max(mx, a[i]);
  }

  const long long D = (long long)mx - (long long)mn;
  const long long total_pairs = 1LL * n * (n - 1) / 2;

  for (long long k = 1; k <= n; ++k) {
    long long cnt = (k >= D) ? total_pairs : count_at_most_k(a, k);
    if (cnt < k) {
      cout << "unstable\n";
      return 0;
    }
    if (cnt >= n) {
      cout << "stable\n";
      return 0;
    }
  }
  assert(false);
  return 0;
}

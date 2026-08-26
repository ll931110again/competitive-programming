// Codeforces Spectral::Cup 2026 Round 2 — We Be Flipping (Hard Version)
// https://codeforces.com/contest/2229/problem/C2
//
// Sketch
// ------
// Let idx be the rightmost index ever operated on (or -1 if none). Then a[idx]
// starts positive and ends negative, and later indices are never flipped, so
// they keep their original signs. Every index before idx can be given either
// sign; the easy-version greedy makes them all negative, and one last operation
// at idx then makes them all positive.
//
// The reachable arrays are therefore the original array, or
//   [|a_0|, ..., |a_{idx-1}|, -a_idx, a_{idx+1}, ..., a_{n-1}]
// for some idx with a[idx] > 0. The first index cannot improve on "do nothing"
// (it would only negate a[0]), so it is enough to try idx = 1..n-1. Reconstruct
// by making the prefix negative, then operating on idx.
//
// Complexity: O(n) per test.

#include <bits/stdc++.h>
using namespace std;

namespace {

vector<int> make_prefix_negative(vector<int> a, int end_exclusive) {
  vector<int> ops;
  int parity = 0;
  for (int i = end_exclusive - 1; i >= 0; i--) {
    if (parity == 1) {
      a[i] = -a[i];
    }
    if (a[i] > 0) {
      ops.push_back(i);
      parity ^= 1;
    }
  }
  return ops;
}

vector<int> maximize_sum(const vector<int>& a) {
  const int n = static_cast<int>(a.size());
  vector<int64_t> abs_pref(n), suffix(n + 1);
  abs_pref[0] = abs(a[0]);
  for (int i = 1; i < n; i++) {
    abs_pref[i] = abs_pref[i - 1] + abs(a[i]);
  }
  suffix[n] = 0;
  suffix[n - 1] = a[n - 1];
  for (int i = n - 2; i >= 0; i--) {
    suffix[i] = suffix[i + 1] + a[i];
  }

  int64_t best = suffix[0];
  int idx = -1;
  for (int i = 1; i < n; i++) {
    if (a[i] <= 0) {
      continue;
    }
    const int64_t score = abs_pref[i - 1] + suffix[i + 1] - a[i];
    if (score > best) {
      best = score;
      idx = i;
    }
  }

  if (idx == -1) {
    return {};
  }
  vector<int> ops = make_prefix_negative(a, idx);
  ops.push_back(idx);
  return ops;
}

void write_ops(const vector<int>& ops) {
  cout << ops.size() << '\n';
  for (size_t i = 0; i < ops.size(); i++) {
    cout << ops[i] + 1 << (i + 1 == ops.size() ? '\n' : ' ');
  }
  if (ops.empty()) {
    cout << '\n';
  }
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
    vector<int> a(n);
    for (int& x : a) {
      cin >> x;
    }
    write_ops(maximize_sum(a));
  }
  return 0;
}

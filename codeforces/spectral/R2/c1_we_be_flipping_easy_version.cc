// Codeforces Spectral::Cup 2026 Round 2 — We Be Flipping (Easy Version)
// https://codeforces.com/contest/2229/problem/C1
//
// Sketch
// ------
// An operation at i flips the prefix a[0..i] and is legal only while a[i] is
// currently positive. Processing indices from right to left, whenever the
// current value (after later prefix-flips) is positive, operate there. Each
// such operation makes that index negative and never touches it again, so the
// whole array becomes negative — the unique minimum possible sum.
//
// Complexity: O(n) per test.

#include <bits/stdc++.h>
using namespace std;

namespace {

vector<int> minimize_sum(vector<int> a) {
  vector<int> ops;
  int parity = 0;
  for (int i = static_cast<int>(a.size()) - 1; i >= 0; i--) {
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
    write_ops(minimize_sum(std::move(a)));
  }
  return 0;
}

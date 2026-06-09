// Codeforces 1810 (CodeTON Round 4 (Div. 1 + Div. 2, Rated, Prizes!)) — A. Beautiful Sequence
// Submission: https://codeforces.com/contest/1810/submission/201603208

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 105;
int T, n;
int a[k_max_n];

bool solve() {
  cin >> n;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  for (int i = 1; i <= n; i++) {
    if (a[i] <= i) {
      return true;
    }
  }
  return false;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    if (solve()) {
      cout << "YES";
    } else {
      cout << "NO";
    }
    cout << endl;
  }
  return 0;
}

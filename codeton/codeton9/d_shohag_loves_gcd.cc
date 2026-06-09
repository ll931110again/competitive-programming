// Codeforces 2039 (CodeTON Round 9 (Div. 1 + Div. 2, Rated, Prizes!)) — D. Shohag Loves GCD
// Submission: https://codeforces.com/contest/2039/submission/295467002

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 100005;
int T;
int n, m;
int a[k_max_n];
int value[k_max_n];

void solve() {
  for (int i = 1; i <= n; i++) {
    value[i] = m - 1;
  }

  for (int i = 1; i <= n; i++) {
    for (int j = i + i; j <= n; j += i) {
      value[j] = min(value[j], value[i] - 1);
    }
  }

  for (int i = 1; i <= n; i++) {
    if (value[i] < 0) {
      cout << -1 << endl;
      return;
    }
  }

  for (int i = 1; i <= n; i++) {
    cout << a[value[i]] << ' ';
  }
  cout << endl;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
      cin >> a[i];
    }

    solve();
  }
  return 0;
}

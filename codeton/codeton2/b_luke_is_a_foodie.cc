// Codeforces 1704 (CodeTON Round 2 (Div. 1 + Div. 2, Rated, Prizes!)) — B. Luke is a Foodie
// Submission: https://codeforces.com/contest/1704/submission/211845813

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int T, n, x;
int a[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> x;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }

    int cnt = 0;
    for (int i = 0; i < n;) {
      cnt++;
      int j = i, maxval = a[i], minval = a[i];
      while (j < n) {
        minval = min(minval, a[j]);
        maxval = max(maxval, a[j]);
        if (maxval - minval <= 2 * x) {
          j++;
        } else {
          break;
        }
      }
      i = j;
    }
    cout << cnt - 1 << endl;
  }
  return 0;
}

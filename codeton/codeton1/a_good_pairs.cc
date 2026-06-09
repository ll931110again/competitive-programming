// Codeforces 1656 (CodeTON Round 1 (Div. 1 + Div. 2, Rated, Prizes!)) — A. Good Pairs
// Submission: https://codeforces.com/contest/1656/submission/150726089

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 100005;
int T, n;
int a[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }

    int min_idx = 1, max_idx = 1;
    for (int i = 2; i <= n; i++) {
      if (a[min_idx] > a[i]) {
        min_idx = i;
      }
      if (a[max_idx] < a[i]) {
        max_idx = i;
      }
    }

    printf("%d %d\n", min_idx, max_idx);
  }
  return 0;
}

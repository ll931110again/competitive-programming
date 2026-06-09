// Codeforces 1750 (CodeTON Round 3 (Div. 1 + Div. 2, Rated, Prizes!)) — A. Indirect Sort
// Submission: https://codeforces.com/contest/1750/submission/334470282

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 20;
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
    if (a[1] == 1) {
      cout << "Yes" << endl;
    } else {
      cout << "No" << endl;
    }
  }
  return 0;
}

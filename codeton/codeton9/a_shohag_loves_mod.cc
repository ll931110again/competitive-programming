// Codeforces 2039 (CodeTON Round 9 (Div. 1 + Div. 2, Rated, Prizes!)) — A. Shohag Loves Mod
// Submission: https://codeforces.com/contest/2039/submission/293000694

#include <bits/stdc++.h>
using namespace std;

namespace {

int T, n;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 1; i <= n; i++) {
      cout << 2 * i - 1 << ' ';
    }
    cout << endl;
  }
  return 0;
}

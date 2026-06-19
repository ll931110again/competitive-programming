// Codeforces 1750 (CodeTON Round 3 (Div. 1 + Div. 2, Rated, Prizes!)) — B. Maximum Substring
// Submission: https://codeforces.com/contest/1750/submission/207782192

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int T, n;
string s;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    cin >> s;

    int c0 = 0, c1 = 0, max_gap = 0, gap = 0, last = -1;
    for (auto c : s) {
      if (c == '0') {
        c0++;
      } else {
        c1++;
      }

      if (last != c) {
        last = c;
        gap = 1;
      } else {
        gap++;
      }

      max_gap = max(max_gap, gap);
    }

    i64 ret = max(1LL * c0 * c1, 1LL * max_gap * max_gap);
    cout << ret << endl;
  }
  return 0;
}

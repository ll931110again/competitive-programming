// Codeforces 2039 (CodeTON Round 9 (Div. 1 + Div. 2, Rated, Prizes!)) — C1. Shohag Loves XOR (Easy
// Version) Submission: https://codeforces.com/contest/2039/submission/295468719

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int T;
i64 x, m;

i64 solve(i64 x, i64 m) {
  i64 ret = 0;
  for (i64 y = 1; y <= m && y <= x * 2; y++) {
    if (x == y) {
      continue;
    }
    i64 d = x ^ y;
    if (x % d == 0 || y % d == 0) {
      ret++;
    }
  }

  return ret;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> x >> m;
    cout << solve(x, m) << endl;
  }
  return 0;
}

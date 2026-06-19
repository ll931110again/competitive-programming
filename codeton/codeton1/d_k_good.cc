// Codeforces 1656 (CodeTON Round 1 (Div. 1 + Div. 2, Rated, Prizes!)) — D. K-good
// Submission: https://codeforces.com/contest/1656/submission/150763303

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int T;
i64 n;

i64 solve(i64 n) {
  i64 even = 2, odd = n;
  while (odd % 2 == 0) {
    even *= 2;
    odd /= 2;
  }
  if (odd == 1) {
    return -1;
  }
  if (n * 2 / even >= even + 1) {
    return even;
  }
  return odd;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    scanf("%lld\n", &n);
    cout << solve(n << '\n');
  }
  return 0;
}

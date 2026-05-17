// Codeforces 2039 (CodeTON Round 9 (Div. 1 + Div. 2, Rated, Prizes!)) — C1. Shohag Loves XOR (Easy
// Version) Submission: https://codeforces.com/contest/2039/submission/295468719

#include <algorithm>
#include <iostream>
using namespace std;

int T;
long long x, m;

long long solve(long long x, long long m) {
  long long ret = 0;
  for (long long y = 1; y <= m && y <= x * 2; y++) {
    if (x == y) {
      continue;
    }
    long long d = x ^ y;
    if (x % d == 0 || y % d == 0) {
      ret++;
    }
  }

  return ret;
}

int main() {
  cin >> T;
  while (T--) {
    cin >> x >> m;
    cout << solve(x, m) << endl;
  }
  return 0;
}

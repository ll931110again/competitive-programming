// Codeforces 1750 (CodeTON Round 3 (Div. 1 + Div. 2, Rated, Prizes!)) — B. Maximum Substring
// Submission: https://codeforces.com/contest/1750/submission/207782192

#include <cmath>
#include <iostream>
using namespace std;

int T, n;
string s;

int main() {
  cin >> T;
  while (T--) {
    cin >> n;
    cin >> s;

    int c0 = 0, c1 = 0, maxGap = 0, gap = 0, last = -1;
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

      maxGap = max(maxGap, gap);
    }

    long long ret = max(1LL * c0 * c1, 1LL * maxGap * maxGap);
    cout << ret << endl;
  }
  return 0;
}

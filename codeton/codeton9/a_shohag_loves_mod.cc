// Codeforces 2039 (CodeTON Round 9 (Div. 1 + Div. 2, Rated, Prizes!)) — A. Shohag Loves Mod
// Submission: https://codeforces.com/contest/2039/submission/293000694

#include <algorithm>
#include <iostream>
using namespace std;

int T, n;

int main() {
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

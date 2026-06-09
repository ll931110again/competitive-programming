// Codeforces 1870 (CodeTON Round 6 (Div. 1 + Div. 2, Rated, Prizes!)) — A. MEXanized Array
// Submission: https://codeforces.com/contest/1870/submission/307441829

#include <bits/stdc++.h>
using namespace std;

namespace {

int T;
int n, k, x;

int solve(int n, int k, int x) {
  if (k > n || k > x + 1) {
    return -1;
  }

  int ss = 0;
  int ns = n;

  for (int i = 0; i < k; i++) {
    ss += i;
    ns--;
  }

  int xs = (x == k) ? (x - 1) : x;
  for (int i = 0; i < ns; i++) {
    ss += xs;
  }

  return ss;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> k >> x;
    cout << solve(n, k, x) << endl;
  }
  return 0;
}

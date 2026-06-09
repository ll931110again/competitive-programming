/*
 * Solution for problem A: ASCII Art.
 *
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 105;
int m, n;
string s[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("ascii.in", "r", stdin);
  freopen("ascii.out", "w", stdout);

  cin >> m >> n;
  for (int i = 0; i < m; i++) {
    cin >> s[i];
  }

  int slashes = 0, dots = 0, parity = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (s[i][j] == '/' || s[i][j] == '\\') {
        slashes++;
        parity ^= 1;
      } else if (parity) {
        dots++;
      }
    }
  }

  cout << slashes / 2 + dots << endl;
  return 0;
}

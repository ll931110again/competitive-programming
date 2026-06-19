// Codeforces 1842 (CodeTON Round 5 (Div. 1 + Div. 2, Rated, Prizes!)) — A. Tenzing and Tsondu
// Submission: https://codeforces.com/contest/1842/submission/210977293

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 55;
int T, n, m;
int a[k_max_n], b[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    for (int i = 0; i < m; i++) {
      cin >> b[i];
    }

    i64 ans = 0;
    for (int i = 0; i < n; i++) {
      ans += a[i];
    }
    for (int i = 0; i < m; i++) {
      ans -= b[i];
    }

    if (ans > 0) {
      cout << "Tsondu" << endl;
    } else if (ans < 0) {
      cout << "Tenzing" << endl;
    } else {
      cout << "Draw" << endl;
    }
  }
  return 0;
}

// Codeforces 1842 (CodeTON Round 5 (Div. 1 + Div. 2, Rated, Prizes!)) — B. Tenzing and Books
// Submission: https://codeforces.com/contest/1842/submission/210977526

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 100005;
int T, n, x;
int a[3][k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> x;
    for (int it = 0; it < 3; it++) {
      for (int i = 0; i < n; i++) {
        cin >> a[it][i];
      }
    }

    int idx[3];
    idx[0] = idx[1] = idx[2] = 0;

    int gap = 0;
    for (int i = 0; i < 3; i++) {
      while (idx[i] < n) {
        int tmp = a[i][idx[i]] | gap;
        if ((x & tmp) == tmp) {
          gap = tmp;
          idx[i]++;
        } else {
          break;
        }
      }
    }

    if (x == gap) {
      cout << "Yes" << endl;
    } else {
      cout << "No" << endl;
    }
  }
  return 0;
}

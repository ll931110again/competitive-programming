
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 250005;
int T, n, Q;
int a[k_max_n];
int nc[k_max_n][2];
int next_same[k_max_n];

int solve(int L, int R) {
  int nzeros = nc[R][0] - nc[L - 1][0];
  int nones = nc[R][1] - nc[L - 1][1];
  if (nzeros % 3 || nones % 3) {
    return -1;
  }

  int ans = nzeros / 3 + nones / 3;
  if (next_same[L] > R) {
    ans++;
  }

  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> Q;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }

    nc[0][0] = nc[0][1] = 0;
    for (int i = 1; i <= n; i++) {
      for (int j = 0; j < 2; j++) {
        nc[i][j] = nc[i - 1][j] + (a[i] == j);
      }
    }

    next_same[n] = n + 1;
    for (int i = n - 1; i > 0; i--) {
      next_same[i] = next_same[i + 1];
      if (a[i] == a[i + 1]) {
        next_same[i] = i + 1;
      }
    }

    while (Q--) {
      int L, R;
      cin >> L >> R;
      cout << solve(L, R) << endl;
    }
  }
  return 0;
}

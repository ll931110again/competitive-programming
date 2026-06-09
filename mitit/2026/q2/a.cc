
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 500005;
int T;
int n, k, x, y;
int a[k_max_n];

int solve(int st) {
  for (int i = 0; i < k; i++) {
    if (st == x) {
      st = y;
    }
    st = (st + a[i]) % n;
    if (st == x) {
      st = y;
    }
  }
  return st;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n >> k >> x >> y;
    for (int i = 0; i < k; i++) {
      cin >> a[i];
    }
    for (int i = 0; i < n; i++)
      if (i != x) {
        cout << solve(i) << ' ';
      }
    cout << endl;
  }

  return 0;
}


#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 1005;
int T, n;
int a[k_max_n];

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    sort(a, a + n);
    int ans = (a[n - 1] - a[0] + 1) / 2;
    cout << ans << endl;
  }

  return 0;
}

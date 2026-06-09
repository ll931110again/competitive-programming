
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 105;
int T;
int n, a[k_max_n];

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  for (int it = 1; it <= T; it++) {
    cerr << it << endl;
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }

    int ans = 0;
    for (int i = 0; i + 1 < n; i++) {
      ans = max(ans, abs(a[i] - a[i + 1]));
    }

    cout << "Case #" << it << ": " << ans << endl;
  }

  return 0;
}

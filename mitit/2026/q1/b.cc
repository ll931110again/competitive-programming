
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 200005;
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
    i64 ans = 0;
    for (int i = 0; i + 1 < n; i++) {
      ans += max(a[i] - a[0], a[n - 1] - a[i]);
    }
    cout << ans << endl;
  }

  return 0;
}

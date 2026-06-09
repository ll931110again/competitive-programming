#include <bits/stdc++.h>
using namespace std;

namespace {

int T;
int m, n, x, y;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> m >> n >> x >> y;
    int ans = max(x, m - x - 1) * n;
    ans = max(ans, m * max(y, n - y - 1));
    cout << ans << endl;
  }
  return 0;
}

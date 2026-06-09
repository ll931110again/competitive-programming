
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 250005;
int T;
int n, rk, ck, rd, cd;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> rk >> ck >> rd >> cd;
    int ans = 0;
    if (rk > rd) {
      ans = max(ans, n - rd);
    } else if (rk < rd) {
      ans = max(ans, rd);
    }
    if (ck > cd) {
      ans = max(ans, n - cd);
    } else if (ck < cd) {
      ans = max(ans, cd);
    }
    cout << ans << endl;
  }
  return 0;
}

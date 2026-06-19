
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 200005;
int T, n;
int a[k_max_n][4];

i64 solve() {
  i64 ans = 0;
  for (int i = 0; i < n; i++) {
    if (a[i][1] > a[i][3]) {
      ans += a[i][0] + a[i][1] - a[i][3];
    } else {
      ans += max(0, a[i][0] - a[i][2]);
    }
  }
  return ans;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < 4; j++) {
        cin >> a[i][j];
      }
    }
    cout << solve() << endl;
  }
  return 0;
}

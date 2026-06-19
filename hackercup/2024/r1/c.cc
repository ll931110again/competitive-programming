#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
#define mod 998244353

namespace {

int T, n;
i64 G, W, L;

i64 solve() {
  cin >> W >> G >> L;
  i64 l_value = (2LL * L + 1) % mod;
  i64 dist = (W - G) % mod;
  return (dist * l_value) % mod;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  for (int it = 1; it <= T; it++) {
    cout << "Case #" << it << ": " << solve() << '\n';
  }
  return 0;
}

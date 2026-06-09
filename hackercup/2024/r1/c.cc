#include <bits/stdc++.h>
using namespace std;

#define mod 998244353

namespace {

int T, n;
long long G, W, L;

long long solve() {
  cin >> W >> G >> L;
  long long l_value = (2LL * L + 1) % mod;
  long long dist = (W - G) % mod;
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

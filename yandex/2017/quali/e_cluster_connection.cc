#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr unsigned MOD = 1'000'000'007;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

int n;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  Mint ans = 1;
  for (int i = 1; i <= n; i++) {
    ans *= i;
  }
  ans = ans * (n * n + 2 * n - 18) / 24;
  cout << ans << endl;
  return 0;
}

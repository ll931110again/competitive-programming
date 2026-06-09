#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 100005;
constexpr unsigned MOD = 1'000'000'007;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

int n, k;
Mint fact[k_max_n], inv[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> k;
  fact[0] = 1;
  for (int i = 1; i < k_max_n; i++) {
    fact[i] = fact[i - 1] * i;
  }

  inv[k_max_n - 1] = fact[k_max_n - 1].inv();
  for (int i = k_max_n - 2; i >= 0; i--) {
    inv[i] = inv[i + 1] * (i + 1);
  }

  auto binom = [&](int x, int y) {
    auto top = fact[x];
    auto bot = inv[y] * inv[x - y];
    return top * bot;
  };

  auto ans = Mint(k);

  auto ss = Mint(1), ps = Mint(2).pow(k);
  for (int i = 1; i <= k; i++) {
    ans -= (ss / ps).pow(n - 1);
    ss += binom(k, i);
  }

  cout << ans << endl;

  return 0;
}

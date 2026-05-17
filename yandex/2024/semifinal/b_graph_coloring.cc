#include <bits/stdc++.h>
using namespace std;

static constexpr int maxn = 100005;
static constexpr unsigned MOD = 1'000'000'007;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

int n, k;
Mint fact[maxn], inv[maxn];

int main() {
  cin >> n >> k;
  fact[0] = 1;
  for (int i = 1; i < maxn; i++) {
    fact[i] = fact[i - 1] * i;
  }

  inv[maxn - 1] = fact[maxn - 1].inv();
  for (int i = maxn - 2; i >= 0; i--) {
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
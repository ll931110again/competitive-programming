
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 400005;
constexpr unsigned MOD = 1'000'000'007;
#include "../../lib/modint.h"
using Mint = ModInt<MOD>;

Mint fact[k_max_n], inv[k_max_n];

Mint binom(int x, int y) {
  if (x < y) {
    return 0;
  }
  return fact[x] * inv[y] * inv[x - y];
}

Mint g(int n, int m, int x) {
  if (n < 0 || m < 0) {
    return 0;
  }

  if (n == 0) {
    return (m == 0);
  }

  Mint ans = 0;
  for (int t = 0; t * (x + 1) <= m; t++) {
    Mint value = binom(n, t) * binom(m + n - 1 - t * (x + 1), n - 1);
    if (t & 1) {
      ans -= value;
    } else {
      ans += value;
    }
  }
  return ans;
}

Mint solve_fn(int n, int m, int x) {
  Mint g1 = g(n - 1, m - x, x);
  Mint g2 = g(n - 2, m - 2 * x, x);

  Mint x_sum = g1 * x + g2 * x * (n - 1);
  Mint a_sum = g1 * (m - x) / (n - 1) + g2 * x;
  if (n > 2) {
    a_sum += g2 * (m - 2 * x);
  }

  return x_sum - a_sum;
}

Mint solve(int n, int m) {
  Mint ans = 0;
  for (int x = 1; x <= m; x++) {
    ans += solve_fn(n, m, x);
  }
  return ans;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  fact[0] = 1;
  for (int i = 1; i < k_max_n; i++) {
    fact[i] = fact[i - 1] * i;
  }
  inv[k_max_n - 1] = fact[k_max_n - 1].inv();
  for (int i = k_max_n - 2; i >= 0; i--) {
    inv[i] = inv[i + 1] * (i + 1);
  }

  int T, n, m;

  cin >> T;
  while (T--) {
    cin >> n >> m;
    cout << solve(n, m) << endl;
  }
  return 0;
}

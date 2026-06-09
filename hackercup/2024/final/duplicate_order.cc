#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 10005;
constexpr unsigned MOD = 1'000'000'007;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

int T;
int n, m1, m2, h, sigma;

Mint fact[k_max_n], inv[k_max_n];
Mint prefixes[k_max_n];

Mint binom(int x, int y) {
  if (x < y) {
    return 0;
  }
  return fact[x] * inv[y] * inv[x - y];
}

Mint solve() {
  for (int i = 0; i <= n; i++) {
    prefixes[i] = Mint(sigma - 1).pow(i) * binom(n - h, i);
    if (i) {
      prefixes[i] += prefixes[i - 1];
    }
  }

  Mint ans = 0;
  for (int a = 0; a <= h; a++)
    if (h - a <= m1) {
      for (int b = 0; a + b <= h; b++)
        if (h - b <= m2) {
          Mint arrangement = binom(h, a) * binom(h - a, b) * Mint(sigma - 2).pow(h - a - b);
          int bound = min(m1 - (h - a), m2 - (h - b));
          arrangement *= prefixes[bound];

          ans += arrangement;
        }
    }

  Mint ways = Mint(sigma).pow(n) * binom(n, h) * Mint(sigma - 1).pow(h);
  return ans * ways;
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

  cin >> T;
  for (int it = 1; it <= T; it++) {
    cerr << it << endl;
    cin >> n >> m1 >> m2 >> h >> sigma;
    cout << "Case #" << it << ": " << solve() << endl;
  }

  return 0;
}

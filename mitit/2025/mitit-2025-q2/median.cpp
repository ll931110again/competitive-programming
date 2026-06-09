#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 900005;
int T, n, m;
int a[k_max_n], b[k_max_n];

constexpr unsigned MOD = 1'000'000'007;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

Mint fact[k_max_n], inv[k_max_n];

Mint binom(int x, int y) {
  Mint up = fact[x];
  Mint down = inv[y] * inv[x - y];
  return up * down;
}

int solve() {
  int X = (3 * n + 1) / 2;
  int pos = -1;

  for (int i = 0; i < m; i++)
    if (b[i] == X) {
      pos = a[i];
    }

  int segment = -1;
  if (pos >= 0) {
    if (pos <= n) {
      segment = 1;
    } else if (pos <= 2 * n) {
      segment = 2;
    } else {
      segment = 3;
    }
  }

  Mint ret = 0;
  for (int value = 1; value <= 3; value++)
    if (segment < 0 || segment == value) {
      int low = (value - 1) * n + 1, high = low + n - 1;

      int lc = n / 2, hc = n / 2, slots_outside = 2 * n;
      for (int i = 0; i < m; i++) {
        if (low <= a[i] && a[i] <= high) {
          if (b[i] < X) {
            lc--;
          } else if (b[i] > X) {
            hc--;
          }
        } else {
          slots_outside--;
        }
      }

      Mint count_segment = 0;
      if (lc >= 0 && hc >= 0) {
        count_segment = binom(X - 1, lc) * binom(X - 1, hc);
        count_segment = count_segment * fact[n / 2 - lc];
        count_segment = count_segment * fact[n / 2 - hc];
        if (segment < 0) {
          count_segment = count_segment * n;
        }
      }
      Mint count_remains = fact[slots_outside];
      ret += count_segment * count_remains;
    }

  return (int)ret.x;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  fact[0] = 1;
  for (int i = 1; i < k_max_n; i++) {
    fact[i] = fact[i - 1] * i;
  }

  for (int i = 0; i < k_max_n; i++) {
    inv[i] = fact[i].inv();
  }

  cin >> T;
  while (T--) {
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
      cin >> a[i] >> b[i];
    }
    cout << solve() << endl;
  }

  return 0;
}

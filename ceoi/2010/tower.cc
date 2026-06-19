/*
 * Solution for CEOI 2010 Day 2: A Huge Tower.
 *
 * Sort blocks by size. When adding the i-th smallest block to any valid tower of
 * the previous i-1 blocks, there are X(i) insertion positions, so
 * C(i) = X(i) * C(i-1).
 */

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
#include "../../lib/modint.h"

namespace {

static constexpr unsigned k_mod = 1'000'000'009;
using Mint = ModInt<k_mod>;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  i64 d;
  cin >> n >> d;
  vector<i64> sizes(n);
  for (int i = 0; i < n; i++) {
    cin >> sizes[i];
  }

  sort(sizes.begin(), sizes.end());

  Mint ways = 1;
  int left = 0;
  for (int i = 0; i < n; i++) {
    while (left < i && sizes[left] + d < sizes[i]) {
      left++;
    }
    i64 insert_positions = 1 + (i - left);
    ways *= Mint(static_cast<unsigned>(insert_positions));
  }

  cout << ways.x << '\n';
  return 0;
}

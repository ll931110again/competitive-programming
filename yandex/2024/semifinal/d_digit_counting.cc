#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr unsigned MOD = 1'000'000'007;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

Mint solve(i64 R) {
  if (R < 2) {
    return 0;
  }

  // special case: two digits
  auto ans = Mint(R - 1) * Mint(R) / 2 * 2;

  // three digits
  i64 n = sqrt(R);
  auto threes = Mint(R + 1) * (n - 1);
  // subtract 2^2 + .. + n^2
  threes -= Mint(n) * Mint(n + 1) * Mint(n * 2 + 1) / 6 - 1;

  ans += threes;

  // four digits
  i64 low = 1, high = 1e6;
  while (low <= high) {
    i64 mid = (low + high) / 2;
    if (mid * mid * mid <= R) {
      n = mid;
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  auto fours = Mint(R + 1) * (n - 1);
  // subtract 2^3 + ... + n^3
  auto sums = Mint(n) * Mint(n + 1) / 2;
  sums = sums * sums;
  fours -= (sums - 1);

  ans += fours;

  // five digits
  low = 1;
  high = 32000;
  while (low <= high) {
    i64 mid = (low + high) / 2;
    if (mid * mid * mid * mid <= R) {
      n = mid;
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  auto fives = Mint(R + 1) * (n - 1);
  // subtract 2^4 + .. + n^4
  sums = Mint(n) * Mint(n + 1) * Mint(n * 2 + 1) * Mint(n * n * 3 + n * 3 - 1) / 30 - 1;
  fives -= sums;
  ans += fives;

  for (int x = 2;; x++) {
    bool found = false;
    i64 value = 1, d = 0;

    while (true) {
      if (R / x < value) {
        break;
      }

      d++;
      value *= x;

      if (d >= 5) {
        found = true;
        ans += (R - value + 1);
      }
    }

    if (!found) {
      break;
    }
  }

  return ans;
}

int T;

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> T;
  while (T--) {
    i64 L, R;
    cin >> L >> R;
    cout << solve(R) - solve(L - 1) << endl;
  }
  return 0;
}

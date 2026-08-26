// Codeforces Spectral::Cup 2026 Round 3 — Familiar?
// https://codeforces.com/contest/2245/problem/F
//
// Interval DP on relative permutations with stack-height constraints (editorial).
// Complexity: O(n^3) per test, sum n^3 <= 500^3.

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int kMod = 998244353;
constexpr int kMaxN = 505;

int binom[kMaxN][kMaxN];
vector<int> down[kMaxN][kMaxN];
int up[kMaxN][kMaxN];
int a[kMaxN];

int ways(int left, int mid) {
  if (left == mid) {
    return a[mid] <= 0 ? 1 : 0;
  }
  if (a[mid] >= 0) {
    if (a[mid] >= static_cast<int>(down[left][mid - 1].size())) {
      return 0;
    }
    return down[left][mid - 1][a[mid]];
  }
  return up[left][mid - 1];
}

int solve_one(int n) {
  int sum_known = 0;
  for (int i = 1; i <= n; i++) {
    if (a[i] >= 0) {
      sum_known += a[i];
    }
  }
  if (sum_known >= n) {
    return 0;
  }

  for (int i = 1; i <= n + 1; i++) {
    for (int j = 0; j <= n; j++) {
      down[i][j].clear();
      up[i][j] = 0;
    }
  }

  for (int i = 1; i <= n + 1; i++) {
    down[i][i - 1] = {1};
    up[i][i - 1] = 1;
  }

  for (int len = 1; len <= n; len++) {
    for (int left = 1; left + len - 1 <= n; left++) {
      const int right = left + len - 1;
      if (a[right + 1] == -1) {
        up[left][right] = 0;
        for (int mid = left; mid <= right; mid++) {
          const int64_t add = 1LL * binom[right - left][mid - left] * ways(left, mid) % kMod;
          up[left][right] = (up[left][right] + add * up[mid + 1][right]) % kMod;
        }
      } else {
        down[left][right].assign(a[right + 1] + 1, 0);
        for (int height = 1; height <= a[right + 1]; height++) {
          for (int mid = left; mid <= right; mid++) {
            if (height - 1 >= static_cast<int>(down[mid + 1][right].size())) {
              continue;
            }
            const int64_t add = 1LL * binom[right - left][mid - left] * ways(left, mid) % kMod;
            down[left][right][height] =
                (down[left][right][height] + add * down[mid + 1][right][height - 1]) % kMod;
          }
        }
      }
    }
  }

  return up[1][n];
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  binom[0][0] = 1;
  for (int i = 1; i < kMaxN; i++) {
    for (int j = 0; j <= i; j++) {
      binom[i][j] = (binom[i - 1][j] + (j ? binom[i - 1][j - 1] : 0)) % kMod;
    }
  }

  int tests;
  cin >> tests;
  while (tests--) {
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }
    a[n + 1] = -1;
    cout << solve_one(n) << '\n';
  }
  return 0;
}

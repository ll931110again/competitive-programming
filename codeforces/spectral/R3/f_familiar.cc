// Codeforces Spectral::Cup 2026 Round 3 — Familiar
// https://codeforces.com/contest/2245/problem/F
//
// Sketch
// ------
// Interval DP on permutations with stack-height constraints. f[i][j][c] counts
// relative permutations of [i, j] that end with c items on the stack; g is the
// sum over c. Recurrence over the position of the minimum yields O(n^3).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int kMaxN = 505;

int T, n;
int a[kMaxN];

int mod = 998'244'353;
int binom[kMaxN][kMaxN];

vector<int> f[kMaxN][kMaxN];
int g[kMaxN][kMaxN];

int64_t solve() {
  int sa = 0;
  for (int i = 0; i < n; i++)
    if (a[i] >= 0) {
      sa += a[i];
    }
  if (sa >= n) {
    return 0;
  }

  for (int i = 1; i <= n + 1; i++) {
    for (int j = 0; j <= n; j++) {
      f[i][j].clear();
    }
  }

  for (int i = 1; i <= n + 1; i++) {
    f[i][i - 1] = {1};
    g[i][i - 1] = 1;
  }

  auto w = [&](int i, int j) { return (a[j] >= 0) ? f[i][j - 1][a[j]] : g[i][j - 1]; };

  for (int l = 1; l <= n; l++) {
    for (int i = 1; i + l - 1 <= n; i++) {
      int j = i + l - 1;
      if (a[j + 1] == -1) {
        g[i][j] = 0;
        for (int k = i; k <= j; k++) {
          int64_t value = 1LL * binom[j - i][k - i] * g[k + 1][j] % mod;
          value = value * w(i, k) % mod;
          g[i][j] = (g[i][j] + value) % mod;
        }
      } else {
        f[i][j].resize(a[j + 1] + 1);
        for (int t = 1; t <= a[j + 1]; t++) {
          f[i][j][t] = 0;
          for (int k = i; k <= j; k++) {
            if (t >= f[k + 1][j].size()) {
              continue;
            }

            int64_t value = 1LL * binom[j - i][k - i] * f[k + 1][j][t - 1] % mod;
            value = value * w(i, k) % mod;
            f[i][j][t] = (f[i][j][t] + value) % mod;
          }
        }
      }
    }
  }

  return w(1, n + 1);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  binom[0][0] = 1;
  for (int i = 1; i < kMaxN; i++) {
    for (int j = 0; j <= i; j++) {
      binom[i][j] = (binom[i - 1][j] + (j ? binom[i - 1][j - 1] : 0)) % mod;
    }
  }

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }
    a[n + 1] = -1;
    cout << solve() << endl;
  }

  return 0;
}

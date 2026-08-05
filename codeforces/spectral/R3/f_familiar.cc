/*
 * Solutions for F - Familiar.
 * 
 * Consider the smallest element of the permutation, say x. This divides the permutation into two halves:
   The remaining elements in the first half will be emptied by x, and the second half will always
   have x in the stack.

 * Let f[i, j, c] be the number of (relative) permutations such that:
    * All the constraints from i to j are satisfied.
    * In the end, the stack has c remaining elements.

   Initial conditions: f[i, i - 1, 0] = 1 for all 1 <= i <= n.

   Recursion:
      f[i, j, c] = sum(i <= k <= j) w[i, k] * f[k + 1, j, c - 1] * binom(j - i, k - i)
      where w(i, j) is the number of (relative) permutations such that
      all the constraints from i to j - 1 are satisfied.

      w[i, i] = 1 for all 1 <= i <= n.
      w[i, j] = f[i, j - 1, a_j] if a_j >= 0.
      w[i, j] = sum_c f[i, j - 1, c] if a_j < 0.

  The recursion here is O(N^4) which is too slow. To improve that, let's sum over the elements of f:
    g[i, j] = sum f[i, j, c]

  First, this simplifies calculation of w[i, j]:
    w[i, j] = f[i, j - 1, a_j] if a_j >= 0 else g[i, j - 1]

  sum over all c for f:
    g[i, j] = sum_c f[i, j, c] = sum_c sum(i <= k <= j) w[i, k] * f[k + 1, j, c - 1] * binom(j - i, k - i)
                     = sum(i <= k <= j) w[i, j] * binom(j - i, k - i) * sum_c f[k + 1, j, c - 1]
                     = sum(i <= k <= j) w[i, j] * binom(j - i, k - i) * g[k + 1, j]

  which gives a O(n^3) solution.
 */

#include <bits/stdc++.h>
#define maxn 505
using namespace std;

int T, n;
int a[maxn];

int mod = 998'244'353;
int binom[maxn][maxn];

vector<int> f[maxn][maxn];
int g[maxn][maxn];

long long solve() {
  int sa = 0;
  for (int i = 0; i < n; i++) if (a[i] >= 0) {
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

  auto w = [&](int i, int j) {
    return (a[j] >= 0) ? f[i][j - 1][a[j]] : g[i][j - 1];
  };

  for (int l = 1; l <= n; l++) {
    for (int i = 1; i + l - 1 <= n; i++) {
      int j = i + l - 1;
      if (a[j + 1] == -1) {
        g[i][j] = 0;
        for (int k = i; k <= j; k++) {
          long long value = 1LL * binom[j - i][k - i] * g[k + 1][j] % mod;
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

            long long value = 1LL * binom[j - i][k - i] * f[k + 1][j][t - 1] % mod;
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
  cin.tie(0);

  binom[0][0] = 1;
  for (int i = 1; i < maxn; i++) {
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

/*
 * Solution for problem E: Evacuation plan.
 *
 */

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 4005;
struct Point {
  int pos;
  int idx;
};

int m, n;
Point a[k_max_n], b[k_max_n];

i64 dp[k_max_n][k_max_n];
i64 inf = 1e18;
int trace[k_max_n][k_max_n], assignment[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("evacuation.in", "r", stdin);
  freopen("evacuation.out", "w", stdout);

  cin >> n;
  for (int i = 1; i <= n; i++) {
    cin >> a[i].pos;
    a[i].idx = i;
  }

  sort(a + 1, a + n + 1, [&](auto x, auto y) { return x.pos < y.pos; });

  cin >> m;
  for (int i = 1; i <= m; i++) {
    cin >> b[i].pos;
    b[i].idx = i;
  }

  sort(b + 1, b + m + 1, [&](auto x, auto y) { return x.pos < y.pos; });

  dp[0][0] = 0;
  for (int i = 1; i <= n; i++) {
    dp[i][0] = inf;
  }
  for (int j = 1; j <= m; j++) {
    dp[0][j] = inf;
  }

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m && j <= i; j++) {
      dp[i][j] = inf;
      if (i - 1 >= j) {
        dp[i][j] = dp[i - 1][j];
        trace[i][j] = j;
      }
      if (dp[i][j] > dp[i - 1][j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
        trace[i][j] = j - 1;
      }
      dp[i][j] += abs(a[i].pos - b[j].pos);
    }
  }

  cout << dp[n][m] << endl;

  int j = m;
  for (int i = n; i > 0; i--) {
    assignment[a[i].idx] = b[j].idx;
    j = trace[i][j];
  }

  for (int i = 1; i <= n; i++) {
    cout << assignment[i] << ' ';
  }
  cout << endl;

  return 0;
}

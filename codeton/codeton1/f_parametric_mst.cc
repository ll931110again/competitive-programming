// Codeforces 1656 (CodeTON Round 1 (Div. 1 + Div. 2, Rated, Prizes!)) — F. Parametric MST
// Submission: https://codeforces.com/contest/1656/submission/151142642

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 200005;
int T, n;
int INF = 1e6;
int a[k_max_n], b[k_max_n];
i64 ss[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }
    sort(a + 1, a + n + 1);

    auto f = [&](int t) {
      for (int i = 1; i <= n; i++) {
        b[i] = a[i] + t;
      }
      for (int i = 1; i <= n; i++) {
        ss[i] = ss[i - 1] + b[i];
      }
      i64 ans = 1e18;
      for (int i = 1; i < n; i++) {
        ans = min(ans, 1LL * b[n] * ss[i] + 1LL * b[1] * (ss[n] - ss[i]));
      }
      return ans - 1LL * b[1] * b[n] - 1LL * (n - 1) * t * t;
    };

    int low = -INF, high = INF, pos = INF;
    i64 value = -1e18;
    while (low <= high - 10) {
      int m1 = (low + low + high) / 3, m2 = (low + high + high) / 3;
      i64 v1 = f(m1), v2 = f(m2);
      if (value < v1) {
        value = v1;
        pos = m1;
      }
      if (value < v2) {
        value = v2;
        pos = m2;
      }
      if (v1 < v2) {
        low = m1;
      } else {
        high = m2;
      }
    }
    for (int i = low; i <= high; i++) {
      i64 v = f(i);
      if (value < v) {
        pos = i;
        value = v;
      }
    }

    if (abs(pos) >= INF) {
      printf("INF\n");
    } else {
      cout << value << '\n';
    }
  }
  return 0;
}

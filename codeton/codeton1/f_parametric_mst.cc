// Codeforces 1656 (CodeTON Round 1 (Div. 1 + Div. 2, Rated, Prizes!)) — F. Parametric MST
// Submission: https://codeforces.com/contest/1656/submission/151142642

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <cstdio>
#include <queue>
#include <stack>
#include <vector>
#define maxn 200005
using namespace std;

int T, n;
int INF = 1e6;
int a[maxn], b[maxn];
long long ss[maxn];

int main() {
  scanf("%d", &T);
  while (T--) {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
      scanf("%d", &a[i]);
    }
    sort(a + 1, a + n + 1);

    auto f = [&](int t) {
      for (int i = 1; i <= n; i++) {
        b[i] = a[i] + t;
      }
      for (int i = 1; i <= n; i++) {
        ss[i] = ss[i - 1] + b[i];
      }
      long long ans = 1e18;
      for (int i = 1; i < n; i++) {
        ans = min(ans, 1LL * b[n] * ss[i] + 1LL * b[1] * (ss[n] - ss[i]));
      }
      return ans - 1LL * b[1] * b[n] - 1LL * (n - 1) * t * t;
    };

    int low = -INF, high = INF, pos = INF;
    long long value = -1e18;
    while (low <= high - 10) {
      int m1 = (low + low + high) / 3, m2 = (low + high + high) / 3;
      long long v1 = f(m1), v2 = f(m2);
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
      long long v = f(i);
      if (value < v) {
        pos = i;
        value = v;
      }
    }

    if (abs(pos) >= INF) {
      printf("INF\n");
    } else {
      printf("%lld\n", value);
    }
  }
  return 0;
}

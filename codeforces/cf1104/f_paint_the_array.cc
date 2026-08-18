
#include <bits/stdc++.h>
#define maxn 500005
using namespace std;

int T, n, m;
int a[maxn];
int inf = 1e9;

int solve() {
  deque<int> dp(m, -inf);
  dp[0] = (a[0] == 0);

  int mn = -inf, mx = dp[0];
  auto get = [&](int i) { return max(dp[i], mn); };

  for (int i = 1; i < n; i++) {
    dp.push_front(mx);
    mn = get(m);
    dp.pop_back();

    dp[a[i]] = get(a[i]) + 1;
    mx = max(mx, dp[a[i]]);
  }

  return n - get(m - 1);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
      a[i]--;
    }
    cout << solve() << endl;
  }
  return 0;
}

#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#define maxn 18
using namespace std;

int T, n, k;
vector<int> a;
pair<int, long long> dp[1 << maxn];

long long solve() {
  int extra = a.back();
  a.pop_back();
  n--;

  long long low = 0, high = 1LL * extra * n, ans = low;
  while (low <= high) {
    long long mid = (low + high) / 2;
    dp[0] = {0, 0};
    for (int mask = 1; mask < (1 << n); mask++) {
      dp[mask] = {0, 0};
      for (int i = 0; i < n; i++)
        if (mask & (1 << i)) {
          auto cur = dp[mask ^ (1 << i)];
          cur.second += a[i];
          if (cur.second >= mid) {
            cur.first++;
            cur.second = 0;
          }
          dp[mask] = max(dp[mask], cur);
        }
    }

    if (dp[(1 << n) - 1].first >= k) {
      ans = mid;
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return (ans + extra);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n >> k;
    a.resize(n);
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    sort(a.begin(), a.end());
    cout << solve() << endl;
  }

  return 0;
}

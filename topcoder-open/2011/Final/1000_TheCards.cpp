#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class TheCards {
  const i64 MOD = 1234567891LL;

public:
  int find(vector<int> dist, int m) {
    int n = (int)dist.size();
    map<int, i64> dp;
    dp[0] = 1;

    for (int i = 0; i < n - 1; i++) {
      map<int, i64> ndp;
      int d = dist[i];
      for (auto& it : dp) {
        int off = it.first;
        i64 ways = it.second;
        if (d == -1) {
          for (int no = -m; no <= m; no++)
            ndp[no] = (ndp[no] + ways) % MOD;
        } else if (d == 0) {
          ndp[off] = (ndp[off] + ways) % MOD;
        } else {
          ndp[off + d] = (ndp[off + d] + ways) % MOD;
          ndp[off - d] = (ndp[off - d] + ways) % MOD;
        }
      }
      dp.swap(ndp);
    }

    auto slotsForOffset = [&](int off) -> i64 {
      if (off < -(m - 1) || off > (m - 1))
        return 0;
      if (off >= 0)
        return m - off;
      return m + off;
    };

    i64 ans = 0;
    int last = dist[n - 1];
    for (auto& it : dp) {
      int off = it.first;
      i64 ways = it.second;
      if (last == -1) {
        ans = (ans + ways * slotsForOffset(off)) % MOD;
      } else if (last == 0) {
        if (off == 0)
          ans = (ans + ways * m) % MOD;
      } else {
        if (off == last || off == -last)
          ans = (ans + ways * slotsForOffset(off)) % MOD;
      }
    }
    return (int)ans;
  }
};

#include <bits/stdc++.h>
using namespace std;

class TheCards {
  const long long MOD = 1234567891LL;

public:
  int find(vector<int> dist, int m) {
    int n = (int)dist.size();
    map<int, long long> dp;
    dp[0] = 1;

    for (int i = 0; i < n - 1; i++) {
      map<int, long long> ndp;
      int d = dist[i];
      for (auto& it : dp) {
        int off = it.first;
        long long ways = it.second;
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

    auto slotsForOffset = [&](int off) -> long long {
      if (off < -(m - 1) || off > (m - 1))
        return 0;
      if (off >= 0)
        return m - off;
      return m + off;
    };

    long long ans = 0;
    int last = dist[n - 1];
    for (auto& it : dp) {
      int off = it.first;
      long long ways = it.second;
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

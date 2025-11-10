#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 3005
using namespace std;

struct Meeting {
  int start_ts;
  int end_ts;
};

int THRESHOLD = 80'000'000;
int T, m, n, L;
Meeting a[maxn], b[maxn];

int dp[2 * maxn][maxn];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
      cin >> m >> n >> L;
      for (int i = 0; i < m; i++) {
        cin >> a[i].start_ts >> a[i].end_ts;
      }
      for (int i = 0; i < n; i++) {
        cin >> b[i].start_ts >> b[i].end_ts;
      }

      sort(a, a + m, [&](auto x, auto y) {
        return x.start_ts < y.start_ts;
      });

      sort(b, b + n, [&](auto x, auto y) {
        return x.start_ts < y.start_ts;
      });

      set<int> endpoints;
      endpoints.insert(0);
      for (int i = 0; i < m; i++) {
        endpoints.insert(a[i].end_ts);
      }
      for (int i = 0; i < n; i++) {
        endpoints.insert(b[i].end_ts);
      }

      vector<int> tss;
      unordered_map<int, int> tsid;
      int cnt = 0;
      for (auto ts : endpoints) {
        tsid[ts] = cnt++;
        tss.push_back(ts);
      }


      int reach_a[2 * maxn], reach_b[2 * maxn];

      map<int, int> available_endpoints;
      int j = 0;
      for (int x = 0; x < cnt; x++) {
        while (j < m && a[j].start_ts < tss[x] + L) {
          available_endpoints[a[j].end_ts]++;
          j++;
        }

        reach_a[x] = -1;
        if (!available_endpoints.empty()) {
          auto it = available_endpoints.end();
          it--;

          if (it->first > tss[x]) {
            reach_a[x] = tsid[it->first];
          }
        }
      }

      j = 0;
      available_endpoints.clear();

      for (int x = 0; x < cnt; x++) {
        while (j < n && b[j].start_ts < tss[x] + L) {
          available_endpoints[b[j].end_ts]++;
          j++;
        }

        reach_b[x] = -1;
        if (!available_endpoints.empty()) {
          auto it = available_endpoints.end();
          it--;

          if (it->first > tss[x]) {
            reach_b[x] = tsid[it->first];
          }
        }
      }

      memset(dp, -1, sizeof dp);
      dp[0][0] = 0;
      
      for (int i = 0; i < tss.size(); i++) {
        for (int j = 0; j <= m; j++) if (dp[i][j] >= 0) {
          if (reach_a[i] >= 0) {
            int _i = reach_a[i];
            int _j = j + 1;
            if (dp[_i][_j] < 0 || dp[_i][_j] > dp[i][j]) {
              dp[_i][_j] = dp[i][j];
            }
          }

          if (reach_b[i] >= 0) {
            int _i = reach_b[i];
            int _j = j;

            if (dp[_i][_j] < 0 || dp[_i][_j] > dp[i][j] + 1) {
              dp[_i][_j] = dp[i][j] + 1;
            }
          }
        }
      }

      int inf = 1e9;
      int ans = inf;
      for (int i = 0; i < tss.size(); i++) if (tss[i] + L > THRESHOLD) {
        for (int j = 0; j <= m; j++) if (dp[i][j] >= 0) {
          ans = min(ans, max(j, dp[i][j]));
        }
      }

      cout << "Case #" << it << ": ";
      if (ans >= inf) {
        cout << "Lunchtime" << endl;
      } else {
        cout << ans << endl;
      }
    }

    return 0;
}
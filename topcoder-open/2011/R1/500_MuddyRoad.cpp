#include <bits/stdc++.h>
using namespace std;

class MuddyRoad {
public:
  double getExpectedValue(vector<int> road) {
    int n = road.size();
    vector<double> p(n);
    for (int i = 0; i < n; i++)
      p[i] = road[i] / 100.0;

    map<pair<int, int>, double> cur, nxt;
    cur[{0, 0}] = 1.0;
    for (int i = 1; i < n; i++) {
      nxt.clear();
      for (auto& kv : cur) {
        int c1 = kv.first.first, c2 = kv.first.second;
        double pr = kv.second;
        for (int mud = 0; mud <= 1; mud++) {
          double pm = mud ? p[i] : (1 - p[i]);
          int nc = mud + min(c1, c2);
          pair<int, int> nk = {nc, c1};
          nxt[nk] += pr * pm;
        }
      }
      cur.swap(nxt);
    }

    double ans = 0;
    for (auto& kv : cur)
      ans += kv.first.first * kv.second;
    return ans;
  }
};

#include <bits/stdc++.h>
using namespace std;

class FoxListeningToMusic {
public:
  vector<double> getProbabilities(vector<int> length, int T) {
    int n = length.size();
    vector<double> ans(n);
    map<int, vector<double>> start;
    start[0] = vector<double>(n, 1.0 / n);
    double target = T + 0.5;

    while (!start.empty()) {
      auto it = start.begin();
      int t = it->first;
      vector<double> p = it->second;
      start.erase(it);

      for (int i = 0; i < n; i++) {
        if (p[i] == 0)
          continue;
        if (t <= T && t + length[i] > target)
          ans[i] += p[i];
        int end = t + length[i];
        if (end <= T) {
          vector<double> nxt(n);
          for (int j = 0; j < n; j++)
            nxt[j] = p[i] / n;
          auto& slot = start[end];
          if (slot.empty())
            slot = vector<double>(n);
          for (int j = 0; j < n; j++)
            slot[j] += nxt[j];
        }
      }
    }
    return ans;
  }
};

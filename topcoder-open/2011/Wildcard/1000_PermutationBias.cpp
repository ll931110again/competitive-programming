#include <bits/stdc++.h>
using namespace std;

class PermutationBias {
  int n;
  vector<int> targetType;

  vector<int> getCycleType(const vector<int>& perm) const {
    vector<int> vis(n);
    vector<int> cyc;
    for (int i = 0; i < n; i++)
      if (!vis[i]) {
        int j = i, len = 0;
        while (!vis[j]) {
          vis[j] = 1;
          j = perm[j];
          len++;
        }
        cyc.push_back(len);
      }
    sort(cyc.begin(), cyc.end());
    return cyc;
  }

  vector<int> permFromType(vector<int> cyc) const {
    sort(cyc.begin(), cyc.end(), greater<int>());
    vector<int> perm(n);
    int pos = 0;
    for (int len : cyc) {
      if (len == 1) {
        perm[pos] = pos;
        pos++;
        continue;
      }
      for (int k = 0; k < len; k++)
        perm[pos + k] = pos + (k + 1) % len;
      pos += len;
    }
    return perm;
  }

  vector<int> applySwapType(const vector<int>& cyc, int x, int y) const {
    vector<int> perm = permFromType(cyc);
    swap(perm[x], perm[y]);
    return getCycleType(perm);
  }

  double typeWeight(const vector<int>& cyc) const {
    double w = 1;
    for (int len : cyc)
      w *= len;
    for (int i = 0; i < (int)cyc.size();) {
      int j = i;
      while (j < (int)cyc.size() && cyc[j] == cyc[i])
        j++;
      int cnt = j - i;
      for (int t = 2; t <= cnt; t++)
        w *= t;
      i = j;
    }
    return w;
  }

public:
  double relativeProbability(vector<int> permutation) {
    n = (int)permutation.size();
    vector<int> perm(n);
    for (int i = 0; i < n; i++)
      perm[i] = permutation[i] - 1;
    targetType = getCycleType(perm);

    vector<int> start(n, 1);
    map<vector<int>, double> dist;
    dist[start] = 1.0;

    for (int step = 0; step < n; step++) {
      map<vector<int>, double> nd;
      for (const auto& kv : dist) {
        map<vector<int>, double> trans;
        for (int x = 0; x < n; x++)
          for (int y = 0; y < n; y++) {
            vector<int> nxt = applySwapType(kv.first, x, y);
            trans[nxt] += 1.0 / (n * n);
          }
        for (const auto& t : trans)
          nd[t.first] += kv.second * t.second;
      }
      dist.swap(nd);
    }

    double pType = dist.count(targetType) ? dist[targetType] : 0.0;
    return pType * typeWeight(targetType);
  }
};

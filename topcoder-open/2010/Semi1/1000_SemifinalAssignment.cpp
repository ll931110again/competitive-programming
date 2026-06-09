#include <bits/stdc++.h>
using namespace std;

struct SemifinalAssignment {
  int N, K;
  vector<int> worst, best;
  vector<double> ans;

  using State = pair<int, vector<int>>;

  static bool sf1(int rank) {
    int r = rank % 4;
    return r == 0 || r == 3;
  }

  void addOutcome(const vector<int>& beats, double prob) {
    if (prob == 0.0)
      return;
    vector<int> adv(N, 0);
    for (int i = 0; i < N; i++)
      if (beats[i] < K)
        adv[i] = 1;
    for (int i = 0; i < N; i++) {
      if (!adv[i])
        continue;
      int rank = 0;
      for (int j = 0; j < i; j++)
        if (adv[j])
          rank++;
      if (sf1(rank))
        ans[2 * i] += prob;
      else
        ans[2 * i + 1] += prob;
    }
  }

  vector<double> getProbability(vector<int> worst_, vector<int> best_, int k) {
    worst = worst_;
    best = best_;
    K = k;
    N = worst.size();
    ans.assign(2 * N, 0.0);

    map<State, double> cur, nxt;
    cur[{0, vector<int>(N, 0)}] = 1.0;

    int lo = *min_element(worst.begin(), worst.end());
    int hi = *max_element(best.begin(), best.end());

    for (int v = hi; v >= lo; v--) {
      nxt.clear();
      for (const auto& it : cur) {
        int mask = it.first.first;
        const vector<int>& beats = it.first.second;
        double prob = it.second;

        vector<int> elig;
        for (int i = 0; i < N; i++) {
          if (mask & (1 << i))
            continue;
          if (worst[i] <= v && v <= best[i])
            elig.push_back(i);
        }
        if (elig.empty()) {
          nxt[{mask, beats}] += prob;
          continue;
        }

        int m = elig.size();
        int subsets = 1 << m;
        for (int s = 0; s < subsets; s++) {
          double p = prob;
          int nmask = mask;
          vector<int> nbeats = beats;
          for (int j = 0; j < m; j++) {
            int i = elig[j];
            double land = 1.0 / (v - worst[i] + 1);
            if (s & (1 << j)) {
              p *= land;
              int rank = 0;
              for (int t = 0; t < j; t++)
                if (s & (1 << t))
                  rank++;
              nbeats[i] = __builtin_popcount(mask) + rank;
              nmask |= 1 << i;
            } else {
              p *= 1.0 - land;
            }
          }
          if (p != 0.0)
            nxt[{nmask, nbeats}] += p;
        }
      }
      cur.swap(nxt);
    }

    for (const auto& it : cur)
      addOutcome(it.first.second, it.second);
    return ans;
  }
};

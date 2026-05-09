// S – Topside vs Zaun
// https://open.kattis.com/problems/topsidevszaun
//
// Maximize S such that two disjoint teams of S people have equal total skill.
// Process multiset one element at a time (A / B / bench); bitset tracks sumA−sumB.

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  if (!(cin >> N))
    return 0;
  vector<int> a(N);
  for (int i = 0; i < N; ++i)
    cin >> a[i];

  constexpr int MAXV = 300;
  constexpr int BSZ = 2 * MAXV * MAXV + 1; // diff range covers ±90000
  constexpr int OFF = MAXV * MAXV;

  vector<int> freq(MAXV + 1);
  for (int x : a)
    ++freq[x];

  const int Smax = N / 2;
  vector<vector<bitset<BSZ>>> dp(Smax + 1, vector<bitset<BSZ>>(Smax + 1));
  dp[0][0][OFF] = 1;

  for (int v = 1; v <= MAXV; ++v) {
    int cnt = freq[v];
    if (!cnt)
      continue;
    for (int rep = 0; rep < cnt; ++rep) {
      vector<vector<bitset<BSZ>>> ndp(Smax + 1, vector<bitset<BSZ>>(Smax + 1));
      for (int ca = 0; ca <= Smax; ++ca) {
        for (int cb = 0; cb <= Smax; ++cb) {
          const auto &bset = dp[ca][cb];
          if (!bset.any())
            continue;
          ndp[ca][cb] |= bset;
          if (ca + 1 <= Smax)
            ndp[ca + 1][cb] |= (bset << v);
          if (cb + 1 <= Smax)
            ndp[ca][cb + 1] |= (bset >> v);
        }
      }
      dp.swap(ndp);
    }
  }

  for (int S = Smax; S >= 1; --S) {
    if (dp[S][S][OFF]) {
      cout << S << '\n';
      return 0;
    }
  }
  cout << -1 << '\n';
  return 0;
}

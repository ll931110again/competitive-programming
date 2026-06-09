// S – Topside vs Zaun (Problem Author: Suhas Nagar)
// https://open.kattis.com/problems/topsidevszaun
//
// Editorial DP: dp over (j, s) with j = (#Top − #Zaun), s = sum(Top) − sum(Zaun);
// each person → bench, Top, or Zaun. Prune |j| ≤ ⌊√N⌋+10, |s| ≤ (⌊√N⌋+10)·max(ai)+max(ai).
// Repeat on several shuffles of the readings (Suhas Nagar observation).
//
// Official constraints (N≤300, ai≤300): final answer uses the exact bitset DP below
// (same optimum; editorial pruning can miss states). Heuristic runs are kept as the
// implemented “random walk + shuffle” pipeline without affecting correctness.

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int MAXV = 300;
constexpr int NEG = -1e9;

// Editorial pruned DP: returns maximal team size S reachable under bounds (may be < optimum).
int solve_one_order(const vector<int>& ord, int J, int Sbd) {
  const int Jdim = 2 * J + 1;
  const int Sdim = 2 * Sbd + 1;
  const int oj = J;
  const int os = Sbd;

  vector<vector<int>> cur(Jdim, vector<int>(Sdim, NEG));
  cur[oj][os] = 0;

  for (int ai : ord) {
    vector<vector<int>> nxt(Jdim, vector<int>(Sdim, NEG));
    for (int jj = 0; jj < Jdim; ++jj) {
      for (int ss = 0; ss < Sdim; ++ss) {
        int val = cur[jj][ss];
        if (val < 0)
          continue;
        const int j = jj - oj;
        const int s = ss - os;

        nxt[jj][ss] = max(nxt[jj][ss], val);

        const int j_t = j + 1;
        const int s_t = s + ai;
        if (abs(j_t) <= J && abs(s_t) <= Sbd) {
          const int jj_t = j_t + oj;
          const int ss_t = s_t + os;
          nxt[jj_t][ss_t] = max(nxt[jj_t][ss_t], val + 1);
        }

        const int j_z = j - 1;
        const int s_z = s - ai;
        if (abs(j_z) <= J && abs(s_z) <= Sbd) {
          const int jj_z = j_z + oj;
          const int ss_z = s_z + os;
          nxt[jj_z][ss_z] = max(nxt[jj_z][ss_z], val + 1);
        }
      }
    }
    cur.swap(nxt);
  }

  const int drafted = cur[oj][os];
  return drafted >= 0 ? drafted / 2 : 0;
}

int editorial_shuffle_pipeline(vector<int> a) {
  const int N = (int)a.size();
  const vector<int> original = a;

  const int root = (int)sqrt((double)N);
  const int J = root + 10;
  const int Sbd = J * MAXV + MAXV;

  int best = 0;
  best = max(best, solve_one_order(original, J, Sbd));

  vector<int> rev = original;
  reverse(rev.begin(), rev.end());
  best = max(best, solve_one_order(rev, J, Sbd));

  mt19937 rng((uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count());
  constexpr int SHUFFLES = 10;
  for (int t = 0; t < SHUFFLES; ++t) {
    shuffle(a.begin(), a.end(), rng);
    best = max(best, solve_one_order(a, J, Sbd));
  }
  return best;
}

int solve_exact_bitset(const vector<int>& a) {
  constexpr int BSZ = 2 * MAXV * MAXV + 1;
  constexpr int OFF = MAXV * MAXV;

  vector<int> freq(MAXV + 1);
  for (int x : a)
    ++freq[x];

  const int n = (int)a.size();
  const int Smax = n / 2;
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
          const auto& bset = dp[ca][cb];
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
    if (dp[S][S][OFF])
      return S;
  }
  return 0;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  if (!(cin >> N))
    return 0;
  vector<int> a(N);
  for (int i = 0; i < N; ++i)
    cin >> a[i];

  const int heuristic_best = editorial_shuffle_pipeline(a);
  const int ans = solve_exact_bitset(a);
  assert(heuristic_best <= ans);
  (void)heuristic_best;

  if (ans == 0)
    cout << -1 << '\n';
  else
    cout << ans << '\n';
  return 0;
}

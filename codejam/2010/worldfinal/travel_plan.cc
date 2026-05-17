#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

static void dfs_left(const vector<long long>& d, int pivot, int i, int prev_t, long long sum,
                     map<int, vector<long long>>& out) {
  if (i > pivot) {
    out[prev_t].push_back(sum);
    return;
  }
  static const int delta[3] = {-2, 0, 2};
  for (int dd : delta) {
    int t = prev_t + dd;
    if (t <= 0)
      continue;
    dfs_left(d, pivot, i + 1, t, sum + (long long)t * d[i], out);
  }
}

static void dfs_right(const vector<long long>& d, int pivot, int i, int cur_t, long long sum,
                      map<int, vector<long long>>& out) {
  // sum already includes contributions for intervals i..m-1, where cur_t = t_i
  static const int delta[3] = {-2, 0, 2};
  if (i == pivot + 1) {
    // choose t_pivot consistent with t_{pivot+1}=cur_t
    for (int dd : delta) {
      int tp = cur_t + dd;
      if (tp <= 0)
        continue;
      out[tp].push_back(sum);
    }
    return;
  }
  for (int dd : delta) {
    int tprev = cur_t + dd;
    if (tprev <= 0)
      continue;
    dfs_right(d, pivot, i - 1, tprev, sum + (long long)tprev * d[i - 1], out);
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int N;
    cin >> N;
    vector<long long> X(N);
    for (int i = 0; i < N; i++)
      cin >> X[i];
    long long F;
    cin >> F;

    sort(X.begin(), X.end());

    const int m = N - 1; // number of intervals
    vector<long long> d(m);
    for (int i = 0; i < m; i++)
      d[i] = X[i + 1] - X[i];

    // Special case: N==2 (one interval, must have t=2).
    if (m == 1) {
      long long need = 2LL * d[0];
      if (need <= F) {
        cout << "Case #" << tc << ": " << need << "\n";
      } else {
        cout << "Case #" << tc << ": NO SOLUTION\n";
      }
      continue;
    }

    const int pivot = m / 2; // 0-based interval index included in the left half

    map<int, vector<long long>> left, right;

    // Left enumeration: t0 is forced to 2 (this is t1 in the analysis).
    // We start from i=1 since we already include interval 0.
    dfs_left(d, pivot, 1, 2, 2LL * d[0], left);

    // Right enumeration: tn-2 (last interval) forced to 2 (this is t_{N-1} in analysis).
    if (pivot == m - 1) {
      // No right half (shouldn't happen for m>1 with pivot=m/2), but keep safe.
      right[2].push_back(0);
    } else if (pivot == m - 2) {
      // Right half is just the last interval (index m-1 is fixed to 2), so no t_pivot constraint
      // beyond diff. Here pivot+1 == m-1, so we can directly set based on t_{m-1}=2.
      static const int delta[3] = {-2, 0, 2};
      long long sum = 2LL * d[m - 1];
      for (int dd : delta) {
        int tp = 2 + dd;
        if (tp <= 0)
          continue;
        right[tp].push_back(sum);
      }
    } else {
      // General: start at last interval (m-1) with t=2, walk left down to pivot+1.
      dfs_right(d, pivot, m - 1, 2, 2LL * d[m - 1], right);
    }

    long long best = -1;
    for (auto& [tp, A] : left) {
      auto it = right.find(tp);
      if (it == right.end())
        continue;
      auto& B = it->second;

      sort(B.begin(), B.end());
      for (long long a : A) {
        if (a > F)
          continue;
        long long rem = F - a;
        auto ub = upper_bound(B.begin(), B.end(), rem);
        if (ub == B.begin())
          continue;
        --ub;
        best = max(best, a + *ub);
      }
    }

    if (best < 0) {
      cout << "Case #" << tc << ": NO SOLUTION\n";
    } else {
      cout << "Case #" << tc << ": " << best << "\n";
    }
  }
  return 0;
}

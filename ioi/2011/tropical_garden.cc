// IOI 2011 Day 1 – Tropical Garden (garden)
// Functional graph on 2N states (fountain, mode). Reverse BFS from P, then
// precompute route counts for all K up to 2(N+M) and extrapolate periodically.
// Time: O(M + N + Q), build with grader or -DTROPICAL_GARDEN_LOCAL_MAIN.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
vector<i64> garden_local_answers;
inline void answer(i64 X) {
  garden_local_answers.push_back(X);
}
#else
#include "garden.h"
#endif

namespace {

constexpr int k_max_n = 150005;
constexpr int k_inf = 1e9;

int nxt[2 * k_max_n];
vector<int> rev[2 * k_max_n];
int dist_to_p[2 * k_max_n];
int next_p_gap[2 * k_max_n];

int compute_next_p_gap(int state, int P, int N) {
  int cur = nxt[state];
  int steps = 1;
  vector<char> seen(2 * N, 0);
  seen[state] = 1;
  while (cur / 2 != P) {
    if (seen[cur] || steps > 4 * N + 5) {
      return k_inf;
    }
    seen[cur] = 1;
    cur = nxt[cur];
    ++steps;
  }
  return steps;
}

} // namespace

void count_routes(int N, int M, int P, int R[][2], int Q, int G[]) {
  vector<int> out0(N, -1), out1(N, -1);
  for (int i = M - 1; i >= 0; --i) {
    const int a = R[i][0], b = R[i][1];
    out1[a] = out0[a];
    out0[a] = b;
    out1[b] = out0[b];
    out0[b] = a;
  }

  const int states = 2 * N;
  for (int i = 0; i < states; ++i) {
    rev[i].clear();
    dist_to_p[i] = k_inf;
    next_p_gap[i] = k_inf;
  }

  for (int v = 0; v < N; ++v) {
    for (int m = 0; m < 2; ++m) {
      const int w = (m == 0) ? out0[v] : (out1[v] != -1 ? out1[v] : out0[v]);
      const int nm = (v == out0[w]) ? 1 : 0;
      nxt[2 * v + m] = 2 * w + nm;
    }
  }

  for (int u = 0; u < states; ++u) {
    rev[nxt[u]].push_back(u);
  }

  vector<int> queue;
  queue.reserve(states);
  for (int m = 0; m < 2; ++m) {
    const int target = 2 * P + m;
    dist_to_p[target] = 0;
    queue.push_back(target);
  }

  for (size_t qi = 0; qi < queue.size(); ++qi) {
    const int u = queue[qi];
    for (int pred : rev[u]) {
      if (dist_to_p[pred] == k_inf) {
        dist_to_p[pred] = dist_to_p[u] + 1;
        queue.push_back(pred);
      }
    }
  }

  struct FountainInfo {
    int first = k_inf;
    int period = k_inf;
  };

  vector<FountainInfo> info(N);
  for (int s = 0; s < N; ++s) {
    const int start = 2 * s;
    if (dist_to_p[start] == k_inf) {
      continue;
    }
    info[s].first = dist_to_p[start];
    int cur = start;
    for (int step = 0; step < info[s].first; ++step) {
      cur = nxt[cur];
    }
    if (next_p_gap[cur] == k_inf) {
      next_p_gap[cur] = compute_next_p_gap(cur, P, N);
    }
    info[s].period = next_p_gap[cur];
  }

  const int limit = 2 * (N + M) + 5;
  const int tail_start = 2 * M;
  vector<i64> ways(limit + 1, 0);

  vector<int> unit_period;
  for (int s = 0; s < N; ++s) {
    const int d = info[s].first;
    if (d == k_inf) {
      continue;
    }
    if (info[s].period == k_inf) {
      if (d <= limit) {
        ++ways[d];
      }
      continue;
    }
    if (info[s].period == 1) {
      unit_period.push_back(d);
      continue;
    }
    for (int k = d; k <= limit; k += info[s].period) {
      ++ways[k];
    }
  }

  sort(unit_period.begin(), unit_period.end());
  for (int k = 0; k <= limit; ++k) {
    ways[k] += static_cast<i64>(unit_period.end() -
                                lower_bound(unit_period.begin(), unit_period.end(), k));
  }

  int seq_period = 0;
  for (int p = 1; p <= limit - tail_start && !seq_period; ++p) {
    bool ok = true;
    for (int k = tail_start; k + p <= limit; ++k) {
      if (ways[k] != ways[k + p]) {
        ok = false;
        break;
      }
    }
    if (ok) {
      seq_period = p;
    }
  }

  auto count_for_k = [&](i64 K) -> i64 {
    if (K <= 0) {
      return 0;
    }
    if (K <= limit) {
      return ways[static_cast<int>(K)];
    }
    if (seq_period == 0) {
      return ways[limit];
    }
    const i64 idx = tail_start + (K - tail_start) % seq_period;
    return ways[static_cast<int>(idx)];
  };

  for (int qi = 0; qi < Q; ++qi) {
    answer(count_for_k(G[qi]));
  }
}

#ifdef TROPICAL_GARDEN_LOCAL_MAIN
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M, P;
  if (!(cin >> N >> M >> P)) {
    return 0;
  }
  int R[k_max_n][2];
  for (int i = 0; i < M; ++i) {
    cin >> R[i][0] >> R[i][1];
  }
  int Q;
  cin >> Q;
  int G[k_max_n];
  for (int i = 0; i < Q; ++i) {
    cin >> G[i];
  }
  garden_local_answers.clear();
  count_routes(N, M, P, R, Q, G);
  for (int i = 0; i < Q; ++i) {
    if (i) {
      cout << ' ';
    }
    cout << garden_local_answers[i];
  }
  cout << '\n';
  return 0;
}
#endif

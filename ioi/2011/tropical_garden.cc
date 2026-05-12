// IOI 2011 Day 1 – Tropical Garden (garden)
// Statement: fountains 0..N-1, trails in decreasing beauty; from a fountain take the
// most beautiful outgoing trail unless it equals the last trail taken and another
// trail exists (then second-most, else re-use the same trail).
//
// Model: 2N states (fountain v, mode m). Functional graph + binary lifting.
// Build O(N log U), queries O(Q · N · log U), U = max G[i] (G[i] ≤ 1e9 → 35 bits).
//
// Build with grader only (default): implement count_routes; grader supplies answer.
// Local samples: g++ ... -DTROPICAL_GARDEN_LOCAL_MAIN -I. tropical_garden.cc

#include <bits/stdc++.h>

#ifdef TROPICAL_GARDEN_LOCAL_MAIN
static std::vector<long long> garden_local_answers;
static inline void answer(long long X) { garden_local_answers.push_back(X); }
#endif

#include "garden.h"

#define maxn 150005
#define LOGK 35

using namespace std;

static int up[LOGK][2 * maxn];

void count_routes(int N, int M, int P, int R[][2], int Q, int G[]) {
  vector<int> out0(N, -1), out1(N, -1);
  for (int i = M - 1; i >= 0; --i) {
    int a = R[i][0], b = R[i][1];
    out1[a] = out0[a];
    out0[a] = b;
    out1[b] = out0[b];
    out0[b] = a;
  }

  auto nxf = [&](int v, int m) -> int {
    return m == 0 ? out0[v] : (out1[v] != -1 ? out1[v] : out0[v]);
  };
  auto nxm = [&](int v, int m) -> int {
    int w = nxf(v, m);
    return (v == out0[w]) ? 1 : 0;
  };

  const int SZ = 2 * N;
  static int nxt[2 * maxn];
  for (int v = 0; v < N; ++v) {
    for (int m = 0; m < 2; ++m) {
      int w = nxf(v, m);
      int nm = nxm(v, m);
      nxt[2 * v + m] = 2 * w + nm;
    }
  }

  for (int i = 0; i < SZ; ++i)
    up[0][i] = nxt[i];
  for (int k = 1; k < LOGK; ++k)
    for (int i = 0; i < SZ; ++i)
      up[k][i] = up[k - 1][up[k - 1][i]];

  for (int qi = 0; qi < Q; ++qi) {
    long long K = G[qi];
    long long cnt = 0;
    for (int s = 0; s < N; ++s) {
      int cur = 2 * s;
      for (int b = 0; b < LOGK; ++b) {
        if ((K >> b) & 1LL)
          cur = up[b][cur];
      }
      if (cur / 2 == P)
        ++cnt;
    }
    answer(cnt);
  }
}

#ifdef TROPICAL_GARDEN_LOCAL_MAIN
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M, P;
  if (!(cin >> N >> M >> P))
    return 0;
  static int R[maxn][2];
  for (int i = 0; i < M; ++i)
    cin >> R[i][0] >> R[i][1];
  int Q;
  cin >> Q;
  static int G[maxn];
  for (int i = 0; i < Q; ++i)
    cin >> G[i];
  garden_local_answers.clear();
  count_routes(N, M, P, R, Q, G);
  for (int i = 0; i < Q; ++i) {
    if (i)
      cout << ' ';
    cout << garden_local_answers[i];
  }
  cout << '\n';
  return 0;
}
#endif

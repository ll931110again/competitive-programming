#include <bits/stdc++.h>
using namespace std;

namespace {

int M, N;
int TOT;
int end_pos;
int cp_pos[3];
int cp_time[3];

vector<int> nbr[64];
uint64_t visited;
long long ways;

inline int id(int r, int c) {
  return r * N + c;
}

bool connectivity_ok(int cur) {
  // All unvisited squares must be reachable from cur via unvisited squares.
  uint64_t unvis = ((TOT == 64) ? ~visited : ((1ULL << TOT) - 1) ^ visited);
  // include cur as start even though visited.
  uint64_t seen = 0;
  int q[64];
  int qs = 0, qe = 0;
  q[qe++] = cur;
  seen |= (1ULL << cur);
  int reach_unvis = 0;
  while (qs < qe) {
    int u = q[qs++];
    for (int v : nbr[u]) {
      if (seen & (1ULL << v))
        continue;
      if (v != cur && (visited & (1ULL << v)))
        continue;
      seen |= (1ULL << v);
      q[qe++] = v;
      if (!(visited & (1ULL << v)))
        reach_unvis++;
    }
  }
  int remaining = TOT - __builtin_popcountll(visited);
  return reach_unvis == remaining;
}

void dfs(int cur, int step) {
  // step = number of visited squares so far (start is step=1)
  if (step == TOT) {
    if (cur == end_pos)
      ways++;
    return;
  }

  // checkpoint constraints at this step
  for (int i = 0; i < 3; i++) {
    if (step == cp_time[i]) {
      if (cur != cp_pos[i])
        return;
    } else {
      if (cur == cp_pos[i])
        return; // visited checkpoint at wrong time
    }
  }

  if (cur == end_pos)
    return; // end must be last

  if (!connectivity_ok(cur))
    return;

  for (int v : nbr[cur]) {
    if (visited & (1ULL << v))
      continue;
    visited |= (1ULL << v);
    dfs(v, step + 1);
    visited ^= (1ULL << v);
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int m, n;
  int case_num = 1;
  while (cin >> m >> n) {
    if (m == 0 && n == 0)
      break;
    M = m;
    N = n;
    TOT = M * N;

    int r1, c1, r2, c2, r3, c3;
    cin >> r1 >> c1 >> r2 >> c2 >> r3 >> c3;
    cp_pos[0] = id(r1, c1);
    cp_pos[1] = id(r2, c2);
    cp_pos[2] = id(r3, c3);

    cp_time[0] = (1 * TOT) / 4;
    cp_time[1] = (2 * TOT) / 4;
    cp_time[2] = (3 * TOT) / 4;

    for (int i = 0; i < 64; i++)
      nbr[i].clear();
    for (int r = 0; r < M; r++) {
      for (int c = 0; c < N; c++) {
        int u = id(r, c);
        if (r > 0)
          nbr[u].push_back(id(r - 1, c));
        if (r + 1 < M)
          nbr[u].push_back(id(r + 1, c));
        if (c > 0)
          nbr[u].push_back(id(r, c - 1));
        if (c + 1 < N)
          nbr[u].push_back(id(r, c + 1));
      }
    }

    int start = id(0, 0);
    end_pos = id(0, 1);

    visited = 0;
    visited |= (1ULL << start);
    ways = 0;
    dfs(start, 1);

    cout << "Case " << case_num++ << ": " << ways << "\n";
  }
  return 0;
}

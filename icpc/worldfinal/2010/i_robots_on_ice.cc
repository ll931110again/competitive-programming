#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

static int M, N;
static int TOT;
static int endPos;
static int cpPos[3];
static int cpTime[3];

static vector<int> nbr[64];
static uint64_t visited;
static long long ways;

static inline int id(int r, int c) { return r * N + c; }

static bool connectivityOk(int cur) {
  // All unvisited squares must be reachable from cur via unvisited squares.
  uint64_t unvis = ((TOT == 64) ? ~visited : ((1ULL << TOT) - 1) ^ visited);
  // include cur as start even though visited.
  uint64_t seen = 0;
  int q[64];
  int qs = 0, qe = 0;
  q[qe++] = cur;
  seen |= (1ULL << cur);
  int reachUnvis = 0;
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
        reachUnvis++;
    }
  }
  int remaining = TOT - __builtin_popcountll(visited);
  return reachUnvis == remaining;
}

static void dfs(int cur, int step) {
  // step = number of visited squares so far (start is step=1)
  if (step == TOT) {
    if (cur == endPos)
      ways++;
    return;
  }

  // checkpoint constraints at this step
  for (int i = 0; i < 3; i++) {
    if (step == cpTime[i]) {
      if (cur != cpPos[i])
        return;
    } else {
      if (cur == cpPos[i])
        return; // visited checkpoint at wrong time
    }
  }

  if (cur == endPos)
    return; // end must be last

  if (!connectivityOk(cur))
    return;

  for (int v : nbr[cur]) {
    if (visited & (1ULL << v))
      continue;
    visited |= (1ULL << v);
    dfs(v, step + 1);
    visited ^= (1ULL << v);
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int m, n;
  int caseNum = 1;
  while (cin >> m >> n) {
    if (m == 0 && n == 0)
      break;
    M = m;
    N = n;
    TOT = M * N;

    int r1, c1, r2, c2, r3, c3;
    cin >> r1 >> c1 >> r2 >> c2 >> r3 >> c3;
    cpPos[0] = id(r1, c1);
    cpPos[1] = id(r2, c2);
    cpPos[2] = id(r3, c3);

    cpTime[0] = (1 * TOT) / 4;
    cpTime[1] = (2 * TOT) / 4;
    cpTime[2] = (3 * TOT) / 4;

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
    endPos = id(0, 1);

    visited = 0;
    visited |= (1ULL << start);
    ways = 0;
    dfs(start, 1);

    cout << "Case " << caseNum++ << ": " << ways << "\n";
  }
  return 0;
}

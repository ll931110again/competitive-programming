// Local verifier for AHC005 A (Patrolling).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int DR[4] = {-1, 1, 0, 0};
constexpr int DC[4] = {0, 0, -1, 1};

bool road(const vector<string>& g, int r, int c) {
  return g[r][c] != '#';
}

void mark_visible(const vector<string>& g, int r, int c, vector<vector<char>>& vis) {
  const int N = (int)g.size();
  vis[r][c] = 1;
  for (int d = 0; d < 4; ++d) {
    int nr = r + DR[d], nc = c + DC[d];
    while (nr >= 0 && nr < N && nc >= 0 && nc < N && road(g, nr, nc)) {
      vis[nr][nc] = 1;
      nr += DR[d];
      nc += DC[d];
    }
  }
}

}  // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, si, sj;
  cin >> N >> si >> sj;
  vector<string> g(N);
  for (int i = 0; i < N; ++i)
    cin >> g[i];

  string moves;
  cin >> moves;

  int r = si, c = sj;
  long long t = 0;
  vector<vector<char>> seen(N, vector<char>(N, 0));
  mark_visible(g, r, c, seen);

  auto fail = [&](const string& msg) {
    cout << msg << " score=0\n";
    return 1;
  };

  for (char mv : moves) {
    int dr = 0, dc = 0;
    if (mv == 'U')
      dr = -1;
    else if (mv == 'D')
      dr = 1;
    else if (mv == 'L')
      dc = -1;
    else if (mv == 'R')
      dc = 1;
    else
      return fail("invalid move");

    const int nr = r + dr, nc = c + dc;
    if (nr < 0 || nr >= N || nc < 0 || nc >= N || !road(g, nr, nc))
      return fail("illegal move");

    t += g[nr][nc] - '0';
    r = nr;
    c = nc;
    mark_visible(g, r, c, seen);
  }

  if (r != si || c != sj)
    return fail("must return to start");

  int roads = 0, covered = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (!road(g, i, j))
        continue;
      ++roads;
      if (seen[i][j])
        ++covered;
    }
  }

  long double score = 1e4 * (long double)covered / roads;
  if (covered == roads)
    score += 1e7 * N / max<long long>(1, t);

  cout << "v=" << covered << " r=" << roads << " t=" << t << " score=" << llround(score) << '\n';
  return 0;
}

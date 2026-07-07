// Local verifier for AHC002 A (Walking on Tiles).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int N = 50;
constexpr int DR[4] = {-1, 1, 0, 0};
constexpr int DC[4] = {0, 0, -1, 1};

int move_idx(char c) {
  if (c == 'U')
    return 0;
  if (c == 'D')
    return 1;
  if (c == 'L')
    return 2;
  if (c == 'R')
    return 3;
  return -1;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int si, sj;
  cin >> si >> sj;
  vector<vector<int>> tile(N, vector<int>(N));
  vector<vector<int>> pts(N, vector<int>(N));
  int M = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      cin >> tile[i][j];
      M = max(M, tile[i][j] + 1);
    }
  }
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      cin >> pts[i][j];

  string path;
  if (!(cin >> path)) {
    cout << "WA: empty output\n";
    return 1;
  }

  vector<uint8_t> used(M, 0);
  int r = si, c = sj;
  if (used[tile[r][c]]) {
    cout << "WA: start tile reused\n";
    return 1;
  }
  used[tile[r][c]] = 1;
  long long score = pts[r][c];

  for (char ch : path) {
    const int d = move_idx(ch);
    if (d < 0) {
      cout << "WA: invalid move '" << ch << "'\n";
      return 1;
    }
    const int nr = r + DR[d], nc = c + DC[d];
    if (nr < 0 || nr >= N || nc < 0 || nc >= N) {
      cout << "WA: out of bounds\n";
      return 1;
    }
    const int tid = tile[nr][nc];
    if (used[tid]) {
      cout << "WA: tile revisited\n";
      return 1;
    }
    used[tid] = 1;
    r = nr;
    c = nc;
    score += pts[r][c];
  }

  cout << "OK steps=" << path.size() << " score=" << score << '\n';
  return 0;
}

// O – Plotting the Pilt
// https://open.kattis.com/problems/plottingthepilt

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int R, C, d;
  cin >> R >> C >> d;
  vector<vector<int>> A(R, vector<int>(C));
  for (int i = 0; i < R; ++i)
    for (int j = 0; j < C; ++j)
      cin >> A[i][j];

  auto neigh_min_k = [&](int i, int j) -> int {
    int m = INT_MAX;
    const int di[4] = {-1, 1, 0, 0};
    const int dj[4] = {0, 0, -1, 1};
    for (int t = 0; t < 4; ++t) {
      int ni = i + di[t], nj = j + dj[t];
      if (ni < 0 || ni >= R || nj < 0 || nj >= C)
        continue;
      int kk = A[ni][nj] / d;
      m = min(m, kk);
    }
    return m;
  };

  for (int i = 0; i < R; ++i) {
    for (int j = 0; j < C; ++j) {
      int kij = A[i][j] / d;
      int mij = neigh_min_k(i, j);
      char ch;
      if (kij <= mij)
        ch = ' ';
      else if (kij == mij + 1)
        ch = '+';
      else if (kij == mij + 2)
        ch = 'X';
      else
        ch = '#';
      cout << ch;
    }
    cout << '\n';
  }
  return 0;
}

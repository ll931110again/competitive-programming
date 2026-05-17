// Local verifier for AHC065 A (matches problem rules).

#include <bits/stdc++.h>
using namespace std;

struct Belt {
  vector<pair<int, int>> cells;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  const int EXIT_C = N / 2;
  const int NN = N * N;
  vector<vector<int>> init(N, vector<int>(N));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) cin >> init[i][j];
  }

  vector<vector<int>> grid = init;
  int M;
  cin >> M;
  vector<Belt> belts(M);
  vector<vector<int>> cover(N, vector<int>(N, 0));
  for (int m = 0; m < M; m++) {
    int L;
    cin >> L;
    if (L < 2) {
      cout << "invalid belt length\n";
      return 1;
    }
    belts[m].cells.resize(L);
    for (int x = 0; x < L; x++) {
      int i, j;
      cin >> i >> j;
      belts[m].cells[x] = {i, j};
      if (i < 0 || i >= N || j < 0 || j >= N) {
        cout << "invalid coord\n";
        return 1;
      }
      cover[i][j]++;
    }
    for (int x = 0; x < L; x++) {
      const auto [i0, j0] = belts[m].cells[x];
      const auto [i1, j1] = belts[m].cells[(x + 1) % L];
      if (abs(i0 - i1) + abs(j0 - j1) != 1) {
        cout << "non-adjacent belt\n";
        return 1;
      }
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (cover[i][j] > 2) {
        cout << "cell cover > 2\n";
        return 1;
      }
    }
  }

  auto rotate = [&](int m, int d) {
    const auto &cells = belts[m].cells;
    const int L = (int)cells.size();
    vector<int> vals(L);
    for (int i = 0; i < L; i++) {
      const auto [r, c] = cells[i];
      vals[i] = grid[r][c];
    }
    for (int i = 0; i < L; i++) {
      const int ni = (i + d + L) % L;
      const auto [r, c] = cells[ni];
      grid[r][c] = vals[i];
    }
  };

  int cur = 0;
  int B = 0;
  if (grid[0][EXIT_C] == 0) {
    grid[0][EXIT_C] = -1;
    cur = 1;
    B = 1;
  }

  int T;
  cin >> T;
  if (T < 0 || T > 100000) {
    cout << "invalid T\n";
    return 1;
  }
  for (int t = 0; t < T; t++) {
    int m, d;
    cin >> m >> d;
    if (m < 0 || m >= M || (d != -1 && d != 1)) {
      cout << "invalid op\n";
      return 1;
    }
    rotate(m, d);
    const int v = grid[0][EXIT_C];
    if (v >= 0 && v == cur) {
      grid[0][EXIT_C] = -1;
      cur++;
      B++;
    }
  }

  const long double score =
      (B == NN) ? 1e6 + llround(1e6 * log2(1e5 / max(1, T))) : llround(1e6 * (long double)B / NN);
  cout << "B=" << B << " T=" << T << " score=" << score << '\n';
  return 0;
}

// AHC050 A - Gamble on Ice: https://atcoder.jp/contests/ahc050/tasks/ahc050_a
//
// Greedy rock placement at the current highest robot probability cell.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

struct Solver {
  int n = 0, m = 0;
  vector<string> grid;
  vector<vector<double>> prob;

  void solve() {
    const int total = n * n - m;
    int empty = 0;
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (grid[i][j] == '.')
          ++empty;
      }
    }
    const double p0 = 1.0 / empty;
    prob.assign(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (grid[i][j] == '.')
          prob[i][j] = p0;
      }
    }

    constexpr int dr[4] = {-1, 1, 0, 0};
    constexpr int dc[4] = {0, 0, -1, 1};

    for (int t = 0; t < total; ++t) {
      int bi = -1, bj = -1;
      double best = -1.0;
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
          if (grid[i][j] == '.' && prob[i][j] > best) {
            best = prob[i][j];
            bi = i;
            bj = j;
          }
        }
      }
      cout << bi << ' ' << bj << '\n';
      grid[bi][bj] = '#';
      prob[bi][bj] = 0.0;

      vector<vector<double>> nxt(n, vector<double>(n, 0.0));
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
          if (prob[i][j] == 0.0)
            continue;
          for (int k = 0; k < 4; ++k) {
            int i2 = i, j2 = j;
            while (true) {
              const int ni = i2 + dr[k], nj = j2 + dc[k];
              if (ni < 0 || nj < 0 || ni >= n || nj >= n || grid[ni][nj] == '#')
                break;
              i2 = ni;
              j2 = nj;
            }
            nxt[i2][j2] += prob[i][j] * 0.25;
          }
        }
      }
      prob = std::move(nxt);
    }
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver solver;
  cin >> solver.n >> solver.m;
  solver.grid.resize(solver.n);
  for (int i = 0; i < solver.n; ++i)
    cin >> solver.grid[i];

  solver.solve();
  return 0;
}

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class RGBGame {
  int n;
  vector<string> board;
  vector<pair<int, int>> unknowns;
  double pR[55][55], pB[55][55];

  pair<double, double> playKnown() {
    map<pair<int, int>, pair<double, double>> memo;
    function<pair<double, double>(int, int, int)> dfs = [&](int rows, int cols,
                                                            int turn) -> pair<double, double> {
      if (__builtin_popcount(rows) == 1 && __builtin_popcount(cols) == 1) {
        int r = __builtin_ctz(rows);
        int c = __builtin_ctz(cols);
        return {pR[r][c], pB[r][c]};
      }
      pair<int, int> key = {rows * 128 + cols, turn};
      if (memo.count(key))
        return memo[key];
      if (turn == 0) {
        double aw = 0, bw = 1;
        for (int r = 0; r < n; r++)
          if (rows & (1 << r)) {
            auto nxt = dfs(rows ^ (1 << r), cols, 1);
            if (nxt.first > aw + 1e-15 || (fabs(nxt.first - aw) < 1e-15 && nxt.second < bw)) {
              aw = nxt.first;
              bw = nxt.second;
            }
          }
        return memo[key] = {aw, bw};
      }
      double aw = 1, bw = 0;
      for (int c = 0; c < n; c++)
        if (cols & (1 << c)) {
          auto nxt = dfs(rows, cols ^ (1 << c), 0);
          if (nxt.second > bw + 1e-15 || (fabs(nxt.second - bw) < 1e-15 && nxt.first < aw)) {
            aw = nxt.first;
            bw = nxt.second;
          }
        }
      return memo[key] = {aw, bw};
    };
    return dfs((1 << n) - 1, (1 << n) - 1, 0);
  }

  void setCell(int i, int j, char c) {
    if (c == 'R')
      pR[i][j] = 1, pB[i][j] = 0;
    else if (c == 'B')
      pR[i][j] = 0, pB[i][j] = 1;
    else
      pR[i][j] = 0, pB[i][j] = 0;
  }

public:
  vector<double> probabilities(vector<string> board_) {
    board = board_;
    n = (int)board.size();
    unknowns.clear();
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        if (board[i][j] == '?')
          unknowns.push_back({i, j});
        else
          setCell(i, j, board[i][j]);

    int q = (int)unknowns.size();
    i64 ways = 1;
    for (int i = 0; i < q; i++) {
      ways *= 3;
      if (ways > 3000000)
        break;
    }

    if (ways <= 3000000) {
      double aw = 0, bw = 0;
      vector<int> pick(q);
      function<void(int)> dfs = [&](int pos) {
        if (pos == q) {
          for (int t = 0; t < q; t++)
            setCell(unknowns[t].first, unknowns[t].second, "RGB"[pick[t]]);
          auto res = playKnown();
          aw += res.first;
          bw += res.second;
          return;
        }
        for (int c = 0; c < 3; c++) {
          pick[pos] = c;
          dfs(pos + 1);
        }
      };
      dfs(0);
      double inv = 1.0 / pow(3.0, q);
      return {aw * inv, bw * inv};
    }

    int r = 0, g = 0, b = 0;
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++) {
        char c = board[i][j];
        if (c == 'R')
          r++;
        else if (c == 'G')
          g++;
        else if (c == 'B')
          b++;
      }
    double tot = (double)(r + g + b + q);
    for (auto [i, j] : unknowns) {
      pR[i][j] = (r + q / 3.0) / tot;
      pB[i][j] = (b + q / 3.0) / tot;
    }
    return vector<double>{playKnown().first, playKnown().second};
  }
};

#include <bits/stdc++.h>
using namespace std;

class PerfectlyFairGame {
  vector<string> board;
  int H, W, darts;
  map<pair<int, int>, double> memo;

  double solve(int turn, int diff) {
    if (turn == 2 * darts) {
      if (diff > 0)
        return 1.0;
      if (diff < 0)
        return 0.0;
      return 0.5;
    }
    auto key = make_pair(turn, diff);
    if (memo.count(key))
      return memo[key];

    bool janeTurn = (turn % 2 == 1);
    double res = janeTurn ? 0.0 : 1.0;

    for (int r = 1; r + 1 < H; r++) {
      for (int c = 1; c + 1 < W; c++) {
        map<int, double> dist;
        for (int dr = -1; dr <= 1; dr++) {
          for (int dc = -1; dc <= 1; dc++) {
            int v = board[r + dr][c + dc] - '0';
            dist[v] += 1.0 / 9.0;
          }
        }
        double val = 0.0;
        for (auto& kv : dist) {
          int ndiff = diff + (janeTurn ? kv.first : -kv.first);
          val += kv.second * solve(turn + 1, ndiff);
        }
        if (janeTurn)
          res = max(res, val);
        else
          res = min(res, val);
      }
    }

    memo[key] = res;
    return res;
  }

public:
  double winChance(vector<string> board_, int darts_) {
    board = board_;
    darts = darts_;
    H = (int)board.size();
    W = (int)board[0].size();
    memo.clear();
    return solve(0, 0);
  }
};

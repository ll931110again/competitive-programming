#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class RemoveGame {
  string f;
  int R;
  int n;
  // dp[l][r][t]: 0 unknown, 1 Ciel win, 2 draw, 3 Toast win; second: o left
  vector<vector<vector<int>>> outcome;
  vector<vector<vector<int>>> oLeft;

  struct Res {
    int kind;
    int o;
  };

  Res solve(int l, int r, int turn) {
    if (l > r)
      return {2, 0};
    int& oc = outcome[l][r][turn];
    if (oc != 0)
      return {oc, oLeft[l][r][turn]};

    vector<pair<int, int>> moves;
    for (int i = l; i <= r; i++) {
      if (f[i] == (turn == 0 ? 'x' : 'o')) {
        bool hasL = false, hasR = false;
        for (int j = l; j < i; j++)
          if (f[j] == (turn == 0 ? 'o' : 'x'))
            hasL = true;
        for (int j = i + 1; j <= r; j++)
          if (f[j] == (turn == 0 ? 'o' : 'x'))
            hasR = true;
        if (hasL && hasR)
          moves.push_back({i, turn});
      }
    }

    if (moves.empty()) {
      int o = 0;
      for (int i = l; i <= r; i++)
        if (f[i] == 'o')
          o++;
      if (o > (r - l + 1) - o)
        oc = 1;
      else if (o == (r - l + 1) - o)
        oc = 2;
      else
        oc = 3;
      oLeft[l][r][turn] = o;
      return {oc, o};
    }

    if (turn == 0) {
      Res best = {3, 0};
      for (auto& mv : moves) {
        Res left = solve(l, mv.first - 1, 1);
        Res right = solve(mv.first + 1, r, 1);
        int o = left.o + right.o;
        int kind = max(left.kind, right.kind);
        if (kind == 1) {
          if (o > best.o || best.kind != 1)
            best = {1, o};
        } else if (kind == 2) {
          if (best.kind == 3 || (best.kind == 2 && o > best.o))
            best = {2, o};
        } else {
          if (best.kind == 3 && o < best.o)
            best = {3, o};
        }
        if (best.kind == 1 && o == (r - l + 1))
          break;
      }
      if (best.kind != 1) {
        for (auto& mv : moves) {
          Res left = solve(l, mv.first - 1, 1);
          Res right = solve(mv.first + 1, r, 1);
          int o = left.o + right.o;
          int kind = max(left.kind, right.kind);
          if (kind == 1 && (best.kind != 1 || o > best.o))
            best = {1, o};
          else if (kind == 2 && best.kind == 3)
            best = {2, o};
          else if (kind == 3 && best.kind == 3 && o < best.o)
            best = {3, o};
        }
      }
      oc = best.kind;
      oLeft[l][r][turn] = best.o;
      return best;
    }

    Res best = {1, 0};
    for (auto& mv : moves) {
      Res left = solve(l, mv.first - 1, 0);
      Res right = solve(mv.first + 1, r, 0);
      int o = left.o + right.o;
      int kind = max(left.kind, right.kind);
      if (kind == 3) {
        if (o < best.o || best.kind != 3)
          best = {3, o};
      } else if (kind == 2) {
        if (best.kind == 1 || (best.kind == 2 && o < best.o))
          best = {2, o};
      } else {
        if (best.kind == 1 && o > best.o)
          best = {1, o};
      }
    }
    oc = best.kind;
    oLeft[l][r][turn] = best.o;
    return best;
  }

  void assignChar(int pos, char c) {
    f[pos] = c;
    n = (int)f.size();
    outcome.assign(n, vector<vector<int>>(n, vector<int>(2, 0)));
    oLeft.assign(n, vector<vector<int>>(n, vector<int>(2, 0)));
  }

public:
  i64 countWinning(string field, int R) {
    this->R = R;
    f = field;
    n = (int)f.size();
    i64 ans = 0;
    int q = 0;
    for (char c : f)
      if (c == '?')
        q++;

    function<void(int)> dfs = [&](int pos) {
      if (pos == n) {
        outcome.assign(n, vector<vector<int>>(n, vector<int>(2, 0)));
        oLeft.assign(n, vector<vector<int>>(n, vector<int>(2, 0)));
        Res res = solve(0, n - 1, 0);
        if (res.kind == 1 && res.o >= R)
          ans++;
        return;
      }
      if (f[pos] != '?') {
        dfs(pos + 1);
        return;
      }
      f[pos] = 'o';
      dfs(pos + 1);
      f[pos] = 'x';
      dfs(pos + 1);
      f[pos] = '?';
    };

    dfs(0);
    return ans;
  }
};

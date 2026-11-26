#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {
int T, n;
} // namespace

struct Board {
  vector<string> a;

  Board() {}

  Board(vector<string> _a) : a(_a) {};

  Board transpose() {
    auto at = a;
    for (size_t i = 0; i < a.size(); i++) {
      for (size_t j = 0; j < i; j++) {
        char t = at[i][j];
        at[i][j] = at[j][i];
        at[j][i] = t;
      }
    }
    return Board(at);
  }
};

int solve_row(Board board) {
  string row_set[2];
  row_set[0] = row_set[1] = "";

  int row_cnt[2];
  row_cnt[0] = row_cnt[1] = 0;

  // all the rows starting with 0 or starting with 1 must be the same
  for (auto row : board.a) {
    int key = row[0] - '0';
    if (row_set[key] == "" || row_set[key] == row) {
      row_set[key] = row;
      row_cnt[key]++;
    } else {
      return -1;
    }

    // each row must have the same number of 0 and 1
    int cnt[2];
    cnt[0] = cnt[1] = 0;
    for (auto entry : row) {
      cnt[entry - '0']++;
    }

    if (cnt[0] != n || cnt[1] != n) {
      return -1;
    }
  }

  // there must be exactly n rows starting with 0 and n rows starting with 1
  if (row_cnt[0] != n || row_cnt[1] != n) {
    return -1;
  }

  // the 0-row and 1-row must mirror each other
  for (int i = 0; i < n; i++) {
    if (row_set[0][i] == row_set[1][i]) {
      return -1;
    }
  }

  int ans = 2 * n;
  for (int choice = 0; choice < 2; choice++) {
    int tmp = 0;
    for (int i = 0; i < 2 * n; i++) {
      int key = (choice + i) % 2 + '0';
      if (board.a[i][0] != key) {
        tmp++;
      }
    }

    if (ans > tmp) {
      ans = tmp;
    }
  }

  return ans / 2;
}

int solve(Board board) {
  int row = solve_row(board);
  if (row < 0) {
    return -1;
  }

  board = board.transpose();
  int col = solve_row(board);
  if (col < 0) {
    return -1;
  }

  return row + col;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    cin >> n;
    Board board;
    board.a.resize(2 * n);
    for (int i = 0; i < 2 * n; i++) {
      cin >> board.a[i];
    }

    int ans = solve(board);
    cout << "Case #" << tc << ": ";
    if (ans >= 0) {
      cout << ans << endl;
    } else {
      cout << "IMPOSSIBLE" << endl;
    }
  }
  return 0;
}

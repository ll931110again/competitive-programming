/*
 * Solution for CEOI 2010 Day 1: The Alliances.
 *
 * Row-by-row DP with a bitmask of vertical alliances. Transpose the grid when
 * the number of rows is small instead of the number of columns.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max = 70;

int r, c;
int grid[k_max][k_max];
int h_alliance[k_max][k_max][2];
int v_alliance[k_max][k_max][2];

map<pair<int, int>, bool> memo;

bool cell_ok(int type, int up, int left, int down, int right) {
  if (type == 0) {
    return true;
  }
  if (type == 2) {
    int cnt = up + left + down + right;
    if (cnt != 2) {
      return false;
    }
    return !((up && down) || (left && right));
  }
  return up + left + down + right == type;
}

bool dfs_row(int row, int col, int up_mask, int left, int built_mask);

bool solve_row(int row, int up_mask) {
  if (row == r) {
    return up_mask == 0;
  }
  auto key = make_pair(row, up_mask);
  auto it = memo.find(key);
  if (it != memo.end()) {
    return it->second;
  }
  bool ok = dfs_row(row, 0, up_mask, 0, 0);
  memo[key] = ok;
  return ok;
}

bool dfs_row(int row, int col, int up_mask, int left, int built_mask) {
  if (col == c) {
    return solve_row(row + 1, built_mask);
  }

  int up = (up_mask >> col) & 1;
  int type = grid[row][col];
  int max_down = (row + 1 < r) ? 1 : 0;
  int max_east = (col + 1 < c) ? 1 : 0;

  for (int down = 0; down <= max_down; down++) {
    for (int east = 0; east <= max_east; east++) {
      if (!cell_ok(type, up, left, down, east)) {
        continue;
      }
      v_alliance[row][col][0] = up;
      v_alliance[row][col][1] = down;
      h_alliance[row][col][0] = left;
      h_alliance[row][col][1] = east;
      if (dfs_row(row, col + 1, up_mask, east, built_mask | (down << col))) {
        return true;
      }
    }
  }
  return false;
}

bool solve_dp() {
  if (c > 10) {
    return false;
  }
  memo.clear();
  return solve_row(0, 0);
}

void transpose_grid() {
  int grid_t[k_max][k_max];
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      grid_t[j][i] = grid[i][j];
    }
  }
  swap(r, c);
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      grid[i][j] = grid_t[i][j];
    }
  }
}

void print_output() {
  vector<string> out(3 * r, string(3 * c, '.'));
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      if (grid[i][j] == 0) {
        continue;
      }
      out[3 * i + 1][3 * j + 1] = 'O';
      if (i > 0 && v_alliance[i - 1][j][1]) {
        out[3 * i][3 * j + 1] = 'X';
      }
      if (i + 1 < r && v_alliance[i][j][1]) {
        out[3 * i + 2][3 * j + 1] = 'X';
      }
      if (j > 0 && h_alliance[i][j - 1][1]) {
        out[3 * i + 1][3 * j] = 'X';
      }
      if (j + 1 < c && h_alliance[i][j][1]) {
        out[3 * i + 1][3 * j + 2] = 'X';
      }
    }
  }
  for (const auto& line : out) {
    cout << line << '\n';
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> r >> c;
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      cin >> grid[i][j];
    }
  }

  if (solve_dp()) {
    print_output();
    return 0;
  }

  if (r <= 10) {
    transpose_grid();
    if (solve_dp()) {
      print_output();
      return 0;
    }
  }

  cout << "Impossible!\n";
  return 0;
}

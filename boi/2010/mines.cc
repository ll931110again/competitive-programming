/*
 * Solution for BOI 2010 Day 2: Mines.
 *
 * Fill alternating rows with local constraints, rotating the grid when needed so
 * the number of rows is not 2 mod 3.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max = 600;

int given[k_max + 2][k_max + 2];
int mines[k_max + 2][k_max + 2];
int row_count;
int col_count;
int row_line[k_max + 1];

void rotate_grid() {
  for (int i = 0; i <= min(row_count, col_count) + 1; i++) {
    for (int j = i + 1; j <= max(row_count, col_count) + 1; j++) {
      swap(given[i][j], given[j][i]);
      swap(mines[i][j], mines[j][i]);
    }
  }
  swap(row_count, col_count);
}

void complete_row(int row, int start) {
  for (int col = start + 1; col < col_count; col++) {
    if (mines[row][col - 1] == -1) {
      mines[row][col - 1] = row_line[col] - mines[row][col] - mines[row][col + 1];
    } else if (mines[row][col] == -1) {
      mines[row][col] = row_line[col] - mines[row][col - 1] - mines[row][col + 1];
    } else {
      mines[row][col + 1] = row_line[col] - mines[row][col - 1] - mines[row][col];
    }
  }
  for (int col = start; col >= 1; col--) {
    if (mines[row][col - 1] == -1) {
      mines[row][col - 1] = row_line[col] - mines[row][col] - mines[row][col + 1];
    } else if (mines[row][col] == -1) {
      mines[row][col] = row_line[col] - mines[row][col - 1] - mines[row][col + 1];
    } else {
      mines[row][col + 1] = row_line[col] - mines[row][col - 1] - mines[row][col];
    }
  }
}

void fill_row(int row0, int row1, int row2, int target_row) {
  if (row0 == -1) {
    for (int col = 1; col <= col_count; col++) {
      row_line[col] = given[1][col] - mines[3 - target_row][col - 1] - mines[3 - target_row][col] -
                      mines[3 - target_row][col + 1];
    }
  } else {
    for (int col = 1; col <= col_count; col++) {
      row_line[col] = given[row2][col] + mines[row0][col - 1] + mines[row0][col] +
                      mines[row0][col + 1] - given[row1][col];
    }
  }

  for (int col = 3; col <= col_count; col += 3) {
    mines[target_row][col] = row_line[col - 1] + mines[target_row][col - 3] - row_line[col - 2];
  }

  if (col_count % 3 != 2) {
    for (int col = col_count - 2; col >= 1; col -= 3) {
      mines[target_row][col] = row_line[col + 1] + mines[target_row][col + 3] - row_line[col + 2];
    }
    complete_row(target_row, 1);
  } else {
    int col = 2;
    bool found = false;
    while (col <= col_count && !found) {
      int known = (col % 3 == 0 ? col : (col % 3 == 1 ? col - 1 : col + 1));
      int vertical_sum = row_line[col] - mines[target_row][known];
      if (vertical_sum != 1) {
        for (int neighbor = col - 1; neighbor <= col + 1; neighbor++) {
          if (neighbor != known) {
            mines[target_row][neighbor] = vertical_sum / 2;
          }
        }
        found = true;
        complete_row(target_row, col - 1);
      } else {
        col++;
      }
    }
    if (!found) {
      mines[target_row][1] = 0;
      complete_row(target_row, 1);
    }
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> row_count >> col_count;
  for (int row = 1; row <= row_count; row++) {
    string line;
    cin >> line;
    for (int col = 1; col <= col_count; col++) {
      given[row][col] = line[col - 1] - '0';
      mines[row][col] = -1;
    }
  }

  bool rotated = false;
  if (row_count % 3 == 2) {
    rotate_grid();
    rotated = true;
  }

  for (int row = 3; row <= row_count; row += 3) {
    fill_row(row - 3, row - 2, row - 1, row);
  }
  for (int row = row_count - 2; row >= 1; row -= 3) {
    fill_row(row + 3, row + 2, row + 1, row);
  }

  int special_row = (row_count % 3 == 0 ? 2 : 1);
  fill_row(-1, -1, -1, special_row);
  for (int row = special_row + 3; row <= row_count; row += 3) {
    fill_row(row - 3, row - 2, row - 1, row);
  }

  if (rotated) {
    rotate_grid();
  }

  for (int row = 1; row <= row_count; row++) {
    for (int col = 1; col <= col_count; col++) {
      cout << (mines[row][col] == 0 ? '.' : 'X');
    }
    cout << '\n';
  }
  return 0;
}

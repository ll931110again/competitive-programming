/*
 * Solution for BOI 2011 Day 1: Treasures and Vikings.
 *
 * BFS from the Viking ship marks every cell visible along its row and column.
 * A second BFS from the player must reach the treasure before being seen.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_dim = 505;
constexpr int k_inf = 1e9;

int row_count;
int col_count;
char grid[k_max_dim][k_max_dim];
int viking_rounds[k_max_dim][k_max_dim];
int player_rounds[k_max_dim][k_max_dim];
int start_row;
int start_col;
int treasure_row;
int treasure_col;

bool water(int row, int col) {
  return row >= 1 && row <= row_count && col >= 1 && col <= col_count && grid[row][col] != 'I';
}

void mark_visible_from(int row, int col, int round) {
  for (int r = row; r >= 1 && water(r, col); r--) {
    viking_rounds[r][col] = min(viking_rounds[r][col], round);
  }
  for (int r = row; r <= row_count && water(r, col); r++) {
    viking_rounds[r][col] = min(viking_rounds[r][col], round);
  }
  for (int c = col; c >= 1 && water(row, c); c--) {
    viking_rounds[row][c] = min(viking_rounds[row][c], round);
  }
  for (int c = col; c <= col_count && water(row, c); c++) {
    viking_rounds[row][c] = min(viking_rounds[row][c], round);
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
      grid[row][col] = line[col - 1];
      if (grid[row][col] == 'Y') {
        start_row = row;
        start_col = col;
      } else if (grid[row][col] == 'T') {
        treasure_row = row;
        treasure_col = col;
      }
    }
  }

  for (int row = 1; row <= row_count; row++) {
    for (int col = 1; col <= col_count; col++) {
      viking_rounds[row][col] = k_inf;
      player_rounds[row][col] = k_inf;
    }
  }

  queue<pair<int, int>> viking_queue;
  for (int row = 1; row <= row_count; row++) {
    for (int col = 1; col <= col_count; col++) {
      if (grid[row][col] == 'V') {
        viking_queue.push({row, col});
        mark_visible_from(row, col, 0);
      }
    }
  }

  int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  while (!viking_queue.empty()) {
    auto [row, col] = viking_queue.front();
    viking_queue.pop();
    int round = viking_rounds[row][col];
    for (auto [dr, dc] : directions) {
      int next_row = row + dr;
      int next_col = col + dc;
      if (!water(next_row, next_col)) {
        continue;
      }
      if (viking_rounds[next_row][next_col] > round + 1) {
        viking_rounds[next_row][next_col] = round + 1;
        mark_visible_from(next_row, next_col, round + 1);
        viking_queue.push({next_row, next_col});
      }
    }
  }

  queue<pair<int, int>> player_queue;
  player_rounds[start_row][start_col] = 0;
  player_queue.push({start_row, start_col});
  while (!player_queue.empty()) {
    auto [row, col] = player_queue.front();
    player_queue.pop();
    int round = player_rounds[row][col];
    if (round >= viking_rounds[row][col]) {
      continue;
    }
    if (row == treasure_row && col == treasure_col) {
      cout << "YES\n";
      return 0;
    }
    for (auto [dr, dc] : directions) {
      int next_row = row + dr;
      int next_col = col + dc;
      if (!water(next_row, next_col)) {
        continue;
      }
      if (player_rounds[next_row][next_col] > round + 1) {
        player_rounds[next_row][next_col] = round + 1;
        player_queue.push({next_row, next_col});
      }
    }
  }

  cout << "NO\n";
  return 0;
}

/*
 * Solution for BOI 2010 Day 1: Lego.
 *
 * Brute-force 2x2 placements per layer, then DP across layers with support bitmasks.
 */

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_side = 6;
constexpr int k_max_layers = 6;
constexpr int k_max_configs = 10000;

char side_views[k_max_layers][2][k_max_side + 1];
int support_masks[k_max_layers + 1][k_max_configs];
int config_count[k_max_layers + 1];
i64 ways[k_max_layers + 1][k_max_configs];
int layer_count;
int piece_positions[k_max_side * k_max_side];
int board[k_max_side][k_max_side];

void enumerate_layer(int layer, int piece_id, int last_x, int last_y) {
  for (int x = last_x; x < k_max_side - 1; x++) {
    for (int y = (x == last_x ? last_y + 1 : 0); y < k_max_side - 1; y++) {
      if (board[x][y] == -1 && board[x][y + 1] == -1) {
        piece_positions[piece_id] = x * (k_max_side - 1) + y;
        board[x][y] = board[x + 1][y] = board[x][y + 1] = board[x + 1][y + 1] = piece_id;
        enumerate_layer(layer, piece_id + 1, x, y);
        board[x][y] = board[x + 1][y] = board[x][y + 1] = board[x + 1][y + 1] = -1;
      }
    }
  }

  char piece_colors[k_max_side * k_max_side];
  fill(piece_colors, piece_colors + piece_id, '-');
  for (int view = 0; view < 2; view++) {
    for (int line = 0; line < k_max_side; line++) {
      int seen_piece = -1;
      for (int cell = 0; cell < k_max_side; cell++) {
        int px = 0;
        int py = 0;
        if (view == 0) {
          px = line;
          py = cell;
        } else {
          py = line;
          px = k_max_side - 1 - cell;
        }
        if (board[px][py] != -1) {
          seen_piece = board[px][py];
          break;
        }
      }
      if (seen_piece == -1 && side_views[layer][view][line] != '.') {
        return;
      }
      if (seen_piece != -1) {
        if (side_views[layer][view][line] == '.') {
          return;
        }
        if (piece_colors[seen_piece] == '-') {
          piece_colors[seen_piece] = side_views[layer][view][line];
        }
        if (piece_colors[seen_piece] != side_views[layer][view][line]) {
          return;
        }
      }
    }
  }

  int hidden_multiplier = 1;
  for (int piece = 0; piece < piece_id; piece++) {
    if (piece_colors[piece] == '-') {
      hidden_multiplier *= 3;
    }
  }

  int next_index = config_count[layer + 1];
  ways[layer + 1][next_index] = 0;
  for (int prev = 0; prev < config_count[layer]; prev++) {
    bool fits = true;
    for (int piece = 0; piece < piece_id; piece++) {
      if (((support_masks[layer][prev] >> piece_positions[piece]) & 1) == 0) {
        fits = false;
        break;
      }
    }
    if (fits) {
      ways[layer + 1][next_index] += ways[layer][prev] * hidden_multiplier;
    }
  }
  if (ways[layer + 1][next_index] > 0) {
    support_masks[layer + 1][next_index] = 0;
    for (int x = 0; x < k_max_side - 1; x++) {
      for (int y = 0; y < k_max_side - 1; y++) {
        int bit = x * (k_max_side - 1) + y;
        if (board[x][y] != -1 || board[x][y + 1] != -1 || board[x + 1][y] != -1 ||
            board[x + 1][y + 1] != -1) {
          support_masks[layer + 1][next_index] |= 1 << bit;
        }
      }
    }
    config_count[layer + 1]++;
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> layer_count;
  for (int view = 0; view < 2; view++) {
    for (int layer = layer_count - 1; layer >= 0; layer--) {
      string row;
      cin >> row;
      copy(row.begin(), row.end(), side_views[layer][view]);
    }
  }

  config_count[0] = 1;
  ways[0][0] = 1;
  support_masks[0][0] = ~0;

  i64 answer = 0;
  for (int layer = 0; layer < layer_count; layer++) {
    for (int x = 0; x < k_max_side; x++) {
      for (int y = 0; y < k_max_side; y++) {
        board[x][y] = -1;
      }
    }
    config_count[layer + 1] = 0;
    enumerate_layer(layer, 0, -1, -1);
    answer = 0;
    for (int config = 0; config < config_count[layer + 1]; config++) {
      answer += ways[layer + 1][config];
    }
  }

  cout << answer << '\n';
  return 0;
}

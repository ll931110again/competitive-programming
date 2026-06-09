/*
 * ICPC World Finals 2011 — Problem C: Ancient Messages
 * (UVa 1103; same problem set as icpc2011.pdf in this folder — download from
 * https://icpc.global/worldfinals/past-problems if missing locally.)
 *
 * Each hex digit expands to 4 bits. Flood-fill background, then each black
 * component; holes in a symbol are white components touching it from the left.
 * Hole count maps to hieroglyph (W,A,K,J,S,D).
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

int H, W;
int grid[410][410];

const int moves[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

void flood_fill(int y, int x, int new_value, int value) {
  if (x < 0 || x > W || y < 0 || y > H)
    return;
  if (grid[y][x] != value)
    return;
  grid[y][x] = new_value;
  for (int i = 0; i < 4; ++i)
    flood_fill(y + moves[i][0], x + moves[i][1], new_value, value);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int tc = 0;
  int holes[210];
  char symbols[2000];
  char matrix[401][51];

  while (cin >> H >> W && (H || W)) {
    memset(grid, 0, sizeof(grid));
    memset(holes, 0, sizeof(holes));
    memset(symbols, 0, sizeof(symbols));

    for (int i = 0; i < H; ++i)
      cin >> matrix[i];

    for (int i = 0; i < H; ++i) {
      for (int j = 0; j < W; ++j) {
        int position = 3;
        int digit = (matrix[i][j] >= '0' && matrix[i][j] <= '9') ? matrix[i][j] - '0'
                                                                 : matrix[i][j] - 'a' + 10;
        while (digit) {
          grid[i][(j << 2) + position] = (digit & 1);
          digit >>= 1;
          --position;
        }
      }
    }

    W <<= 2;
    for (int i = 0; i < H; ++i) {
      for (int j = 0; j < W; ++j) {
        if (i == 0 || j == 0 || i == H - 1 || j == W - 1)
          flood_fill(i, j, -1, 0);
      }
    }

    int id = 2;
    for (int i = 0; i < H; ++i) {
      for (int j = 0; j < W; ++j) {
        if (grid[i][j] == 0) {
          ++holes[grid[i][j - 1]];
          flood_fill(i, j, grid[i][j - 1], 0);
        } else if (grid[i][j] == 1) {
          flood_fill(i, j, id++, 1);
        }
      }
    }

    int size = 0;
    for (int i = 2; i < id; ++i) {
      if (holes[i] == 0)
        symbols[size++] = 'W';
      else if (holes[i] == 1)
        symbols[size++] = 'A';
      else if (holes[i] == 2)
        symbols[size++] = 'K';
      else if (holes[i] == 3)
        symbols[size++] = 'J';
      else if (holes[i] == 4)
        symbols[size++] = 'S';
      else if (holes[i] == 5)
        symbols[size++] = 'D';
    }
    sort(symbols, symbols + size);
    cout << "Case " << ++tc << ": " << symbols << "\n";
  }
  return 0;
}

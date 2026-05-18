#include <bits/stdc++.h>
using namespace std;

using namespace std;
int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

class FuzzyLife {
  static int countSurvivors(const vector<string>& grid) {
    int res = 0;
    int R = grid.size(), C = grid[0].size();
    for (int i = 0; i < R; i++)
      for (int j = 0; j < C; j++) {
        int alive = 0;
        for (int d = 0; d < 8; d++) {
          int ni = i + dr[d], nj = j + dc[d];
          if (ni < 0 || nj < 0 || ni >= R || nj >= C)
            continue;
          if (grid[ni][nj] == '1')
            alive++;
        }
        if (grid[i][j] == '1' && (alive == 2 || alive == 3))
          res++;
        else if (grid[i][j] == '0' && alive == 3)
          res++;
      }
    return res;
  }

  static int localScore(vector<string> grid, int i, int j, char val) {
    grid[i][j] = val;
    int res = 0;
    int R = grid.size(), C = grid[0].size();
    for (int di = -1; di <= 1; di++)
      for (int dj = -1; dj <= 1; dj++) {
        int ni = i + di, nj = j + dj;
        if (ni < 0 || nj < 0 || ni >= R || nj >= C)
          continue;
        int alive = 0;
        for (int d = 0; d < 8; d++) {
          int xi = ni + dr[d], xj = nj + dc[d];
          if (xi < 0 || xj < 0 || xi >= R || xj >= C)
            continue;
          if (grid[xi][xj] == '1')
            alive++;
        }
        if (grid[ni][nj] == '1' && (alive == 2 || alive == 3))
          res++;
        else if (grid[ni][nj] == '0' && alive == 3)
          res++;
      }
    return res;
  }

public:
  int survivingCells(vector<string> grid) {
    string border(grid[0].size() + 2, '0');
    for (int i = 0; i < (int)grid.size(); i++)
      grid[i] = '0' + grid[i] + '0';
    grid.insert(grid.begin(), border);
    grid.push_back(border);

    int R = grid.size(), C = grid[0].size();
    for (int i = 0; i < R; i++)
      for (int j = 0; j < C; j++) {
        if (grid[i][j] != '?')
          continue;
        int live = localScore(grid, i, j, '1');
        int dead = localScore(grid, i, j, '0');
        grid[i][j] = (live >= dead) ? '1' : '0';
      }
    return countSurvivors(grid);
  }
};

#include <bits/stdc++.h>
using namespace std;

// TCO 2010 Qual 3 Div I 250 - SumRectangle
// https://archive.topcoder.com/ProblemDetail/rd/14278/pm/10948
//
// Each cell equals the sum of the three cells below, right, and diagonally
// down-right. Rearranged: a[i][j] = a[i-1][j-1] - a[i][j-1] - a[i-1][j].
// Fill the grid from the given top row and left column; the bottom-right
// cell is uniquely determined when h > 1 and w > 1.

class SumRectangle {
public:
  int getCorner(vector<int> leftColumn, vector<int> topRow) {
    int h = leftColumn.size(), w = topRow.size();
    if (h == 1)
      return topRow[w - 1];
    if (w == 1)
      return leftColumn[h - 1];

    vector<vector<int>> a(h, vector<int>(w));
    for (int j = 0; j < w; j++)
      a[0][j] = topRow[j];
    for (int i = 0; i < h; i++)
      a[i][0] = leftColumn[i];
    for (int i = 1; i < h; i++)
      for (int j = 1; j < w; j++)
        a[i][j] = a[i - 1][j - 1] - a[i][j - 1] - a[i - 1][j];
    return a[h - 1][w - 1];
  }
};

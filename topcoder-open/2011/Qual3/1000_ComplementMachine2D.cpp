#include <bits/stdc++.h>
using namespace std;

class ComplementMachine2D {
public:
  int largestSubmatrix(vector<string> matrix) {
    int x = matrix.size();
    int y = matrix[0].size();
    vector<vector<int>> in(x, vector<int>(y));
    for (int i = 0; i < x; i++)
      for (int j = 0; j < y; j++)
        in[i][j] = matrix[i][j] - '0';

    int res = 0;
    int a[60], b[60];
    for (int x1 = 0; x1 < x; x1++) {
      for (int x2 = x1; x2 < x; x2++) {
        for (int y1 = 0; y1 < y; y1++) {
          for (int y2 = y1; y2 < y; y2++) {
            int area = (x2 - x1 + 1) * (y2 - y1 + 1);
            if (area <= res)
              continue;

            int as = 1, bs = 1;
            a[0] = x1;
            b[0] = y1;
            for (int i = x1 + 1; i <= x2; i++)
              if (in[i][y1] != in[i - 1][y1])
                a[as++] = i;
            for (int i = y1 + 1; i <= y2; i++)
              if (in[x1][i] != in[x1][i - 1])
                b[bs++] = i;
            a[as] = x2 + 1;
            b[bs] = y2 + 1;

            bool dame = false;
            for (int i = 0; i < as && !dame; i++) {
              for (int j = 0; j < bs && !dame; j++) {
                int m = (i + j) % 2;
                m ^= in[x1][y1];
                for (int k = a[i]; k < a[i + 1] && !dame; k++)
                  for (int l = b[j]; l < b[j + 1] && !dame; l++)
                    if (in[k][l] != m)
                      dame = true;
              }
            }
            if (!dame)
              res = area;
          }
        }
      }
    }
    return res;
  }
};

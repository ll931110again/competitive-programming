#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class NumberGrid {
  int val(char c) {
    return c - '0';
  }

public:
  vector<string> decode(vector<string> hundreds, vector<string> tens, vector<string> ones) {
    int n = (int)hundreds.size();
    int m = (int)hundreds[0].size();
    vector<vector<int>> E(n, vector<int>(m));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
        E[i][j] = val(hundreds[i][j]) * 100 + val(tens[i][j]) * 10 + val(ones[i][j]);

    i64 sumE = 0;
    vector<i64> rowSum(n), colSum(m);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        sumE += E[i][j];
        rowSum[i] += E[i][j];
        colSum[j] += E[i][j];
      }
    }
    i64 T = sumE / (n + m - 1);
    vector<i64> A(n), B(m);
    for (int i = 0; i < n; i++)
      A[i] = (rowSum[i] - T) / (m - 1);
    for (int j = 0; j < m; j++)
      B[j] = (colSum[j] - T) / (n - 1);

    vector<vector<int>> G(n, vector<int>(m));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) {
        i64 g = A[i] + B[j] - E[i][j];
        if (g < 0 || g > 9)
          return {};
        G[i][j] = (int)g;
      }

    vector<string> ret(n, string(m, '0'));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
        ret[i][j] = char('0' + G[i][j]);
    return ret;
  }
};

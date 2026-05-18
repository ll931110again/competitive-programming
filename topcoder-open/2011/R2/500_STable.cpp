#include <bits/stdc++.h>
using namespace std;

class STable {
  string s, t;
  int n, m;
  vector<vector<unsigned long long>> cellLen;
  int lessLR[31][31];
  int cmpMemo[31][31][31][31];

  char baseChar(int i, int j) {
    if (i == 0)
      return t[j - 1];
    return s[i - 1];
  }

  unsigned long long getLen(int i, int j) {
    if (i == 0 || j == 0)
      return 1;
    return cellLen[i - 1][j] + cellLen[i][j - 1];
  }

  char charAt(int i, int j, unsigned long long pos) {
    if (i == 0)
      return t[j - 1];
    if (j == 0)
      return s[i - 1];
    unsigned long long la = cellLen[i - 1][j], lb = cellLen[i][j - 1];
    if (lessLR[i][j]) {
      if (pos < la)
        return charAt(i - 1, j, pos);
      return charAt(i, j - 1, pos - la);
    }
    if (pos < lb)
      return charAt(i, j - 1, pos);
    return charAt(i - 1, j, pos - lb);
  }

  int cmpCells(int i1, int j1, int i2, int j2) {
    if (i1 < 0 || j1 < 0 || i2 < 0 || j2 < 0)
      return 0;
    int& res = cmpMemo[i1][j1][i2][j2];
    if (res != -2)
      return res;

    if (i1 == 0 && j1 > 0) {
      char a = t[j1 - 1];
      if (i2 == 0 && j2 > 0) {
        char b = t[j2 - 1];
        res = (a == b) ? 0 : (a < b ? -1 : 1);
        return res;
      }
      char b = charAt(i2, j2, 0);
      if (a != b) {
        res = a < b ? -1 : 1;
        return res;
      }
      if (getLen(i2, j2) == 1) {
        res = 0;
        return res;
      }
      res = -1;
      return res;
    }
    if (j1 == 0 && i1 > 0) {
      res = -cmpCells(i2, j2, i1, j1);
      return res;
    }
    if (i2 == 0 || j2 == 0) {
      res = -cmpCells(i2, j2, i1, j1);
      return res;
    }

    char c1 = charAt(i1, j1, 0);
    char c2 = charAt(i2, j2, 0);
    if (c1 != c2) {
      res = c1 < c2 ? -1 : 1;
      return res;
    }
    if (getLen(i1, j1) == 1 && getLen(i2, j2) == 1) {
      res = 0;
      return res;
    }
    if (getLen(i1, j1) == 1) {
      res = -1;
      return res;
    }
    if (getLen(i2, j2) == 1) {
      res = 1;
      return res;
    }

    res = cmpAtOffset(i1, j1, 1, i2, j2, 1);
    return res;
  }

  int cmpAtOffset(int i1, int j1, unsigned long long o1, int i2, int j2, unsigned long long o2) {
    while (true) {
      if (o1 >= getLen(i1, j1)) {
        if (o2 >= getLen(i2, j2))
          return 0;
        return -1;
      }
      if (o2 >= getLen(i2, j2))
        return 1;

      if (i1 == 0) {
        char a = t[j1 - 1];
        char b = charAt(i2, j2, o2);
        if (a != b)
          return a < b ? -1 : 1;
        return 0;
      }
      if (j1 == 0) {
        char a = s[i1 - 1];
        char b = charAt(i2, j2, o2);
        if (a != b)
          return a < b ? -1 : 1;
        return 0;
      }
      if (i2 == 0 || j2 == 0)
        return -cmpAtOffset(i2, j2, o2, i1, j1, o1);

      unsigned long long la = cellLen[i1 - 1][j1], lb = cellLen[i1][j1 - 1];
      if (lessLR[i1][j1]) {
        if (o1 < la)
          return cmpAtOffset(i1 - 1, j1, o1, i2, j2, o2);
        return cmpAtOffset(i1, j1 - 1, o1 - la, i2, j2, o2);
      }
      if (o1 < lb)
        return cmpAtOffset(i1, j1 - 1, o1, i2, j2, o2);
      return cmpAtOffset(i1 - 1, j1, o1 - lb, i2, j2, o2);
    }
  }

  void buildLess() {
    memset(lessLR, 0, sizeof(lessLR));
    for (int sum = 2; sum <= n + m; sum++) {
      for (int i = 1; i <= n; i++) {
        int j = sum - i;
        if (j < 1 || j > m)
          continue;
        lessLR[i][j] = cmpCells(i - 1, j, i, j - 1) < 0;
      }
    }
  }

public:
  string getString(string s_, string t_, long long pos) {
    s = s_;
    t = t_;
    n = s.size();
    m = t.size();
    cellLen.assign(n + 1, vector<unsigned long long>(m + 1, 1));
    memset(cmpMemo, -2, sizeof(cmpMemo));

    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= m; j++) {
        cellLen[i][j] = cellLen[i - 1][j] + cellLen[i][j - 1];
      }
    }

    buildLess();

    string ans;
    unsigned long long L = cellLen[n][m];
    for (int k = 0; k < 50 && pos + k < L; k++)
      ans.push_back(charAt(n, m, pos + k));
    return ans;
  }
};

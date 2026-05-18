#include <bits/stdc++.h>
using namespace std;

struct CuttingGlass {
  struct DSU {
    vector<int> p, r;
    DSU(int n) : p(n), r(n, 0) {
      for (int i = 0; i < n; i++)
        p[i] = i;
    }
    int find(int x) {
      return p[x] == x ? x : p[x] = find(p[x]);
    }
    void unite(int a, int b) {
      a = find(a);
      b = find(b);
      if (a == b)
        return;
      if (r[a] < r[b])
        swap(a, b);
      p[b] = a;
      if (r[a] == r[b])
        r[a]++;
    }
  };

  int pieces(int W, int H, int startx, int starty, vector<string> program) {
    vector<vector<char>> cutH(H + 1, vector<char>(W, 0));
    vector<vector<char>> cutV(W + 1, vector<char>(H, 0));

    string path;
    for (const string& s : program)
      path += s;

    int x = startx, y = starty;
    for (char c : path) {
      if (c == 'L') {
        cutH[y][x - 1] = 1;
        x--;
      } else if (c == 'R') {
        cutH[y][x] = 1;
        x++;
      } else if (c == 'U') {
        cutV[x][y - 1] = 1;
        y--;
      } else {
        cutV[x][y] = 1;
        y++;
      }
    }

    auto id = [&](int i, int j) { return j * W + i; };
    DSU dsu(W * H);
    for (int i = 0; i < W; i++) {
      for (int j = 0; j < H; j++) {
        if (i + 1 < W && !cutV[i + 1][j])
          dsu.unite(id(i, j), id(i + 1, j));
        if (j + 1 < H && !cutH[j + 1][i])
          dsu.unite(id(i, j), id(i, j + 1));
      }
    }

    int comps = 0;
    for (int i = 0; i < W * H; i++)
      if (dsu.find(i) == i)
        comps++;
    return comps;
  }
};

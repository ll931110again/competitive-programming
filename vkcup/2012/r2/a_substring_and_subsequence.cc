#include <bits/stdc++.h>
#define maxn 5002
using namespace std;

int f[maxn][maxn][2];
string a, b;

static constexpr unsigned MOD = 1'000'000'007;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

int rec(int x, int y, int z) {
  if (x < 0 || y < 0)
    return z;
  int& ans = f[x][y][z];
  if (ans >= 0)
    return ans;
  Mint v = z;
  v += rec(x, y - 1, z);
  if (z)
    v -= 1;
  if (a[x] == b[y])
    v += rec(x - 1, y - 1, 1);
  ans = (int)v.x;
  return ans;
}

int main() {
  cin >> a;
  cin >> b;
  memset(f, -1, sizeof(f));
  Mint ret = 0;
  for (int i = 0; i < (int)a.size(); i++)
    ret += rec(i, (int)b.size() - 1, 0);
  cout << ret << '\n';
}

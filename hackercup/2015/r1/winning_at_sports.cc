#include <bits/stdc++.h>
#define MAXN 2000
using namespace std;

int T;
static constexpr unsigned MOD = 1'000'000'007;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

Mint sfree[MAXN + 2][MAXN + 2], sful[MAXN + 2][MAXN + 2];

void solveCase(int it) {
  string input;
  cin >> input;
  int hypen = input.find("-");
  int a = atoi(input.substr(0, hypen).c_str());
  int b = atoi(input.substr(hypen + 1, input.size()).c_str());
  cout << "Case #" << it << ": " << sfree[a][b] << ' ' << sful[b][b] << '\n';
}

int main() {
  cin >> T;
  sfree[1][0] = 1;
  for (int i = 1; i <= MAXN; i++)
    for (int j = 0; j <= MAXN; j++)
      if (i > j) {
        if (i - 1 > j)
          sfree[i][j] += sfree[i - 1][j];
        if (j > 0)
          sfree[i][j] += sfree[i][j - 1];
      }

  sful[0][0] = 1;
  for (int i = 0; i <= MAXN; i++)
    for (int j = 0; j <= MAXN; j++)
      if (i <= j) {
        if (i > 0)
          sful[i][j] += sful[i - 1][j];
        if (j - 1 >= i)
          sful[i][j] += sful[i][j - 1];
      }

  for (int it = 1; it <= T; it++)
    solveCase(it);
}

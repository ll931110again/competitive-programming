// AHC042 A - Oni wa Soto, Fuku wa Uchi: https://atcoder.jp/contests/ahc042/tasks/ahc042_a
//
// For each oni, use the hint: shift toward an empty direction (i+1) times then back.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<string> g(N);
  for (int i = 0; i < N; i++)
    cin >> g[i];

  vector<pair<char, int>> ops;

  auto has_fuku_row = [&](int r, int c, int dc) {
    for (int j = c + dc; j >= 0 && j < N; j += dc)
      if (g[r][j] == 'o')
        return true;
    return false;
  };
  auto has_fuku_col = [&](int r, int c, int dr) {
    for (int i = r + dr; i >= 0 && i < N; i += dr)
      if (g[i][c] == 'o')
        return true;
    return false;
  };

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (g[i][j] != 'x')
        continue;
      char d = 0;
      int p = 0, k = 0;
      if (!has_fuku_col(i, j, -1)) {
        d = 'U';
        p = j;
        k = i + 1;
      } else if (!has_fuku_col(i, j, 1)) {
        d = 'D';
        p = j;
        k = i + 1;
      } else if (!has_fuku_row(i, j, -1)) {
        d = 'L';
        p = i;
        k = j + 1;
      } else {
        d = 'R';
        p = i;
        k = j + 1;
      }
      char back = d;
      if (d == 'U')
        back = 'D';
      else if (d == 'D')
        back = 'U';
      else if (d == 'L')
        back = 'R';
      else
        back = 'L';
      for (int t = 0; t < k; t++)
        ops.emplace_back(d, p);
      for (int t = 0; t < k; t++)
        ops.emplace_back(back, p);
    }
  }

  for (const auto [d, p] : ops)
    cout << d << ' ' << p << '\n';
  return 0;
}

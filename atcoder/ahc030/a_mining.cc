// AHC030 A - Mining: https://atcoder.jp/contests/ahc030/tasks/ahc030_a
//
// Interactive ore detection on an n×n grid. Survey 2×2 blocks, mine positive
// cells, then submit all ore locations. Minimize total query cost.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

int n, m;
double eps;

int survey(int x, int y) {
  cout << "q 1 " << x << ' ' << y << endl;
  int res;
  cin >> res;
  return res;
}

int mine(int x, int y) {
  cout << "q 1 " << x << ' ' << y << endl;
  int res;
  cin >> res;
  return res;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m >> eps;
  for (int i = 0; i < m; ++i) {
    int k;
    cin >> k;
    for (int j = 0; j < k; ++j) {
      int x, y;
      cin >> x >> y;
    }
  }
  for (int i = 0; i < m; ++i) {
    int x, y;
    cin >> x >> y;
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      int v;
      cin >> v;
    }
  }
  for (int i = 0; i < n * n * 2; ++i) {
    double e;
    cin >> e;
  }

  vector<pair<int, int>> ores;
  for (int i = 0; i + 1 < n; ++i) {
    for (int j = 0; j + 1 < n; ++j) {
      const int s = survey(i, j);
      if (s > 0) {
        for (int di = 0; di < 2; ++di) {
          for (int dj = 0; dj < 2; ++dj) {
            if (mine(i + di, j + dj) > 0)
              ores.emplace_back(i + di, j + dj);
          }
        }
      }
    }
  }

  cout << "a " << ores.size();
  for (auto [x, y] : ores)
    cout << ' ' << x << ' ' << y;
  cout << endl;
  return 0;
}

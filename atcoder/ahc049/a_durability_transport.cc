// AHC049 A - Durability-Constrained Transport: https://atcoder.jp/contests/ahc049/tasks/ahc049_a
//
// Carry one box at a time to the entrance with Manhattan moves.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<vector<int>> w(N, vector<int>(N));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      cin >> w[i][j];
  vector<vector<int>> d(N, vector<int>(N));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      cin >> d[i][j];

  int cr = 0, cc = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == 0 && j == 0)
        continue;
      while (cr < i) {
        cout << "D\n";
        cr++;
      }
      while (cr > i) {
        cout << "U\n";
        cr--;
      }
      while (cc < j) {
        cout << "R\n";
        cc++;
      }
      while (cc > j) {
        cout << "L\n";
        cc--;
      }
      cout << "1\n";
      while (cr > 0) {
        cout << "U\n";
        cr--;
      }
      while (cc > 0) {
        cout << "L\n";
        cc--;
      }
    }
  }
  return 0;
}

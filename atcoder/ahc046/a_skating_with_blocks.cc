// AHC046 A - Skating with Blocks: https://atcoder.jp/contests/ahc046/tasks/ahc046_a
//
// Visit destinations in order using single-step moves (M) only.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;
  vector<pair<int, int>> ps(M);
  for (int i = 0; i < M; i++)
    cin >> ps[i].first >> ps[i].second;

  int cr = ps[0].first, cc = ps[0].second;
  for (int t = 1; t < M; t++) {
    const int tr = ps[t].first, tc = ps[t].second;
    while (cr < tr) {
      cout << "M D\n";
      cr++;
    }
    while (cr > tr) {
      cout << "M U\n";
      cr--;
    }
    while (cc < tc) {
      cout << "M R\n";
      cc++;
    }
    while (cc > tc) {
      cout << "M L\n";
      cc--;
    }
  }
  return 0;
}

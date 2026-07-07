// AHC040 A - Packing: https://atcoder.jp/contests/ahc040/tasks/ahc040_a
//
// Pack each turn in one horizontal row (U, base -1) to keep bounding box narrow.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

int N, T;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int sigma;
  cin >> N >> T >> sigma;
  for (int i = 0; i < N; ++i) {
    int w2, h2;
    cin >> w2 >> h2;
  }
  for (int i = 0; i < N; ++i) {
    int w, h;
    cin >> w >> h;
  }
  for (int turn = 0; turn < T; ++turn) {
    int dx, dy;
    cin >> dx >> dy;
    cout << N << '\n';
    for (int i = 0; i < N; ++i)
      cout << i << ' ' << 0 << ' ' << 'U' << ' ' << -1 << '\n';
  }
  return 0;
}

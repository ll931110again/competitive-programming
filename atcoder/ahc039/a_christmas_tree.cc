// AHC039 A - Christmas Tree: https://atcoder.jp/contests/ahc039/tasks/ahc039_a
//
// Single axis-aligned rectangle covering all "keep" points (first N of 2N).

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr i64 D = 100000;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  i64 minx = D, miny = D, maxx = 0, maxy = 0;
  for (int i = 0; i < 2 * N; ++i) {
    i64 x, y;
    cin >> x >> y;
    if (i < N) {
      minx = min(minx, x);
      miny = min(miny, y);
      maxx = max(maxx, x);
      maxy = max(maxy, y);
    }
  }

  if (minx > maxx) {
    minx = miny = 0;
    maxx = maxy = 1;
  }

  cout << 4 << ' ';
  cout << minx << ' ' << miny << ' ';
  cout << maxx << ' ' << miny << ' ';
  cout << maxx << ' ' << maxy << ' ';
  cout << minx << ' ' << maxy << '\n';
  return 0;
}

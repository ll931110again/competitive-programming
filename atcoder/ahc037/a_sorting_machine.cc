// AHC037 A - Sorting Machine: https://atcoder.jp/contests/ahc037/tasks/ahc037_a
//
// Staircase chain from (0,0): sort by x then y, extend via (px,cy) then (px,py).

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

struct Pt {
  i64 x, y;
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<Pt> pts(n);
  for (int i = 0; i < n; ++i)
    cin >> pts[i].x >> pts[i].y;

  sort(pts.begin(), pts.end(), [](const Pt& a, const Pt& b) {
    if (a.x != b.x)
      return a.x < b.x;
    return a.y < b.y;
  });

  vector<array<i64, 4>> ops;
  Pt cur{0, 0};
  for (const auto& p : pts) {
    if (p.x < cur.x || p.y < cur.y)
      continue;
    if (p.x > cur.x)
      ops.push_back({cur.x, cur.y, p.x, cur.y}), cur.x = p.x;
    if (p.y > cur.y)
      ops.push_back({cur.x, cur.y, cur.x, p.y}), cur.y = p.y;
  }

  cout << ops.size() << '\n';
  for (const auto& o : ops)
    cout << o[0] << ' ' << o[1] << ' ' << o[2] << ' ' << o[3] << '\n';
  return 0;
}

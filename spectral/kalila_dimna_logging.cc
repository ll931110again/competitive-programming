#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

using int64 = long long;
using i128 = __int128_t;

struct Line {
  int64 m = 0; // slope
  int64 c = 0; // intercept
  Line() = default;
  Line(int64 m_, int64 c_) : m(m_), c(c_) {}
  inline i128 value(int64 x) const { return (i128)m * (i128)x + (i128)c; }
};

// Min CHT for lines added with strictly decreasing slopes, queries with
// non-decreasing x.
struct MonotoneCHTMin {
  vector<Line> hull;
  int ptr = 0;

  static bool isBad(const Line &l1, const Line &l2, const Line &l3) {
    // (c3-c1)/(m1-m3) <= (c2-c1)/(m1-m2)  => l2 is unnecessary (for min hull)
    // Cross-multiply with i128.
    // Note: m1 > m2 > m3 (strictly decreasing slopes).
    i128 left = (i128)(l3.c - l1.c) * (i128)(l1.m - l2.m);
    i128 right = (i128)(l2.c - l1.c) * (i128)(l1.m - l3.m);
    return left <= right;
  }

  void addLine(int64 m, int64 c) {
    Line ln(m, c);
    if (!hull.empty() && hull.back().m == m) {
      // Keep better intercept for min.
      if (hull.back().c <= c)
        return;
      hull.pop_back();
      if (ptr > (int)hull.size())
        ptr = (int)hull.size();
    }
    hull.push_back(ln);
    while ((int)hull.size() >= 3 &&
           isBad(hull[hull.size() - 3], hull[hull.size() - 2],
                 hull[hull.size() - 1])) {
      hull[hull.size() - 2] = hull.back();
      hull.pop_back();
      if (ptr > (int)hull.size())
        ptr = (int)hull.size();
    }
  }

  i128 query(int64 x) {
    if (hull.empty())
      return (i128)0;
    if (ptr >= (int)hull.size())
      ptr = (int)hull.size() - 1;
    while (ptr + 1 < (int)hull.size() &&
           hull[ptr + 1].value(x) <= hull[ptr].value(x))
      ptr++;
    return hull[ptr].value(x);
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<int64> a(n + 1), b(n + 1);
  for (int i = 1; i <= n; i++)
    cin >> a[i];
  for (int i = 1; i <= n; i++)
    cin >> b[i];

  vector<int64> dp(n + 1, 0);

  MonotoneCHTMin cht;
  dp[1] = 0;
  cht.addLine(b[1], dp[1]);

  for (int i = 2; i <= n; i++) {
    i128 best = cht.query(a[i]);
    dp[i] =
        (int64)best; // guaranteed to fit in int64 by constraints/problem design
    cht.addLine(b[i], dp[i]);
  }

  cout << dp[n] << "\n";
  return 0;
}

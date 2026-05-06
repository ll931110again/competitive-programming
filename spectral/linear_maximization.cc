#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

using int64 = long long;
using i128 = __int128_t;

struct Point {
  int64 x = 0;
  int64 y = 0;
};

static inline bool operator==(const Point &a, const Point &b) {
  return a.x == b.x && a.y == b.y;
}

static inline i128 cross(const Point &a, const Point &b, const Point &c) {
  // (b-a) x (c-a)
  return (i128)(b.x - a.x) * (c.y - a.y) - (i128)(b.y - a.y) * (c.x - a.x);
}

static inline i128 dot(const Point &p, int64 A, int64 B) {
  return (i128)A * p.x + (i128)B * p.y;
}

static vector<Point> convexHull(vector<Point> pts) {
  sort(pts.begin(), pts.end(), [](const Point &a, const Point &b) {
    if (a.x != b.x)
      return a.x < b.x;
    return a.y < b.y;
  });
  pts.erase(unique(pts.begin(), pts.end()), pts.end());
  if ((int)pts.size() <= 1)
    return pts;

  vector<Point> lo, up;
  for (const auto &p : pts) {
    while ((int)lo.size() >= 2 &&
           cross(lo[(int)lo.size() - 2], lo.back(), p) <= 0)
      lo.pop_back();
    lo.push_back(p);
  }
  for (int i = (int)pts.size() - 1; i >= 0; i--) {
    const auto &p = pts[i];
    while ((int)up.size() >= 2 &&
           cross(up[(int)up.size() - 2], up.back(), p) <= 0)
      up.pop_back();
    up.push_back(p);
  }
  lo.pop_back();
  up.pop_back();
  lo.insert(lo.end(), up.begin(), up.end()); // CCW, starts at leftmost-lowest
  return lo;
}

static i128 maxDotOnHull(const vector<Point> &hull, int64 A, int64 B) {
  int n = (int)hull.size();
  if (n == 0)
    return -(i128)1 << 120;
  if (n <= 50) {
    i128 best = dot(hull[0], A, B);
    for (int i = 1; i < n; i++)
      best = max(best, dot(hull[i], A, B));
    return best;
  }

  auto eval = [&](int idx) { return dot(hull[idx], A, B); };

  // The sequence eval(i) over i=0..n-1 is cyclic unimodal.
  // A robust practical trick used in many AC solutions: split into two ranges
  // and ternary-search each.
  auto ternaryRange = [&](int L, int R) {
    i128 best = eval(L);
    best = max(best, eval(R));
    while (R - L >= 6) {
      int m1 = (2 * L + R) / 3;
      int m2 = (L + 2 * R) / 3;
      i128 v1 = eval(m1);
      i128 v2 = eval(m2);
      best = max(best, max(v1, v2));
      if (v1 <= v2)
        L = m1;
      else
        R = m2;
    }
    for (int i = L; i <= R; i++)
      best = max(best, eval(i));
    return best;
  };

  int mid = n / 2;
  return max(ternaryRange(0, mid), ternaryRange(mid, n - 1));
}

static void printI128(i128 v) {
  if (v == 0) {
    cout << 0 << "\n";
    return;
  }
  if (v < 0) {
    cout << '-';
    v = -v;
  }
  string s;
  while (v) {
    int digit = (int)(v % 10);
    s.push_back(char('0' + digit));
    v /= 10;
  }
  reverse(s.begin(), s.end());
  cout << s << "\n";
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int Q;
  cin >> Q;
  vector<Point> pts(Q);
  vector<int64> A(Q), B(Q);
  for (int i = 0; i < Q; i++) {
    cin >> pts[i].x >> pts[i].y >> A[i] >> B[i];
  }

  int N = 1;
  while (N < Q)
    N <<= 1;
  vector<vector<Point>> seg(2 * N);

  for (int i = 0; i < Q; i++)
    seg[N + i] = {pts[i]};
  for (int i = N - 1; i >= 1; i--) {
    vector<Point> merged;
    merged.reserve(seg[i * 2].size() + seg[i * 2 + 1].size());
    merged.insert(merged.end(), seg[i * 2].begin(), seg[i * 2].end());
    merged.insert(merged.end(), seg[i * 2 + 1].begin(), seg[i * 2 + 1].end());
    if (!merged.empty())
      seg[i] = convexHull(std::move(merged));
  }

  for (int i = 0; i < Q; i++) {
    int l = N;
    int r = N + i + 1; // [0..i] inclusive => [0, i+1)
    i128 best = -(i128)1 << 120;
    while (l < r) {
      if (l & 1)
        best = max(best, maxDotOnHull(seg[l++], A[i], B[i]));
      if (r & 1)
        best = max(best, maxDotOnHull(seg[--r], A[i], B[i]));
      l >>= 1;
      r >>= 1;
    }
    printI128(best);
  }
  return 0;
}

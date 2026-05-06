#include <bits/stdc++.h>

using namespace std;

struct Pt {
  long long x = 0, y = 0;
  bool operator<(const Pt &o) const { return x != o.x ? x < o.x : y < o.y; }
  bool operator==(const Pt &o) const { return x == o.x && y == o.y; }
};

static inline long long crossLL(const Pt &a, const Pt &b, const Pt &c) {
  // cross((b-a),(c-a))
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static vector<Pt> convexHull(vector<Pt> pts) {
  sort(pts.begin(), pts.end());
  pts.erase(unique(pts.begin(), pts.end()), pts.end());
  int n = (int)pts.size();
  if (n <= 1) return pts;

  vector<Pt> h;
  h.reserve(n * 2);

  // lower hull
  for (int i = 0; i < n; i++) {
    while ((int)h.size() >= 2 &&
           crossLL(h[h.size() - 2], h[h.size() - 1], pts[i]) <= 0)
      h.pop_back();
    h.push_back(pts[i]);
  }

  // upper hull
  int lowerSize = (int)h.size();
  for (int i = n - 2; i >= 0; i--) {
    while ((int)h.size() > lowerSize &&
           crossLL(h[h.size() - 2], h[h.size() - 1], pts[i]) <= 0)
      h.pop_back();
    h.push_back(pts[i]);
  }

  h.pop_back(); // last equals first
  return h;     // CCW, no collinear points on edges
}

static inline long double distToLine(const Pt &a, const Pt &b, const Pt &p) {
  // distance from p to line through a->b, using area / |ab|
  long double dx = (long double)(b.x - a.x);
  long double dy = (long double)(b.y - a.y);
  long double num = fabsl((long double)crossLL(a, b, p));
  long double den = sqrtl(dx * dx + dy * dy);
  return num / den;
}

static long double minWidthConvexPolygon(const vector<Pt> &h) {
  int m = (int)h.size();
  if (m <= 2) return 0.0L;

  // Rotating calipers: for each edge i->i+1, find farthest vertex in normal direction.
  int j = 1;
  long double ans = numeric_limits<long double>::infinity();
  const long double EPS = 1e-18L;

  for (int i = 0; i < m; i++) {
    int ni = (i + 1) % m;
    // advance j while area increases
    while (true) {
      int nj = (j + 1) % m;
      long long cur = llabs(crossLL(h[i], h[ni], h[j]));
      long long nxt = llabs(crossLL(h[i], h[ni], h[nj]));
      if ((long double)nxt > (long double)cur + EPS)
        j = nj;
      else
        break;
    }

    ans = min(ans, distToLine(h[i], h[ni], h[j]));
  }

  return ans;
}

static inline long double roundUp2(long double x) {
  // round up to nearest multiple of 0.01
  long double v = x * 100.0L;
  long double c = ceill(v - 1e-12L);
  return c / 100.0L;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  int tc = 1;
  cout.setf(std::ios::fixed);
  cout << setprecision(2);

  while (cin >> n) {
    if (n == 0) break;

    vector<Pt> pts;
    pts.reserve(n);
    for (int i = 0; i < n; i++) {
      Pt p;
      cin >> p.x >> p.y;
      pts.push_back(p);
    }

    vector<Pt> hull = convexHull(std::move(pts));
    long double w = minWidthConvexPolygon(hull);
    w = roundUp2(w);

    cout << "Case " << tc++ << ": " << (double)w << "\n";
  }

  return 0;
}

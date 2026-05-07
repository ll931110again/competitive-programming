#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

struct Pt {
  long double x, y;
};

static long double lerp_y(const Pt& a, const Pt& b, long double x) {
  // a.x < b.x, x in [a.x, b.x]
  long double t = (x - a.x) / (b.x - a.x);
  return a.y + (b.y - a.y) * t;
}

static vector<long double> y_at_xs(const vector<Pt>& poly, const vector<long double>& xs) {
  vector<long double> y(xs.size(), 0.0L);
  int j = 0;
  for (int i = 0; i < (int)xs.size(); i++) {
    long double x = xs[i];
    while (j + 1 < (int)poly.size() && poly[j + 1].x < x) j++;
    if (poly[j].x == x) {
      y[i] = poly[j].y;
    } else if (j + 1 < (int)poly.size() && poly[j + 1].x == x) {
      y[i] = poly[j + 1].y;
    } else {
      // x is between poly[j] and poly[j+1]
      y[i] = lerp_y(poly[j], poly[j + 1], x);
    }
  }
  return y;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  cout.setf(std::ios::fixed);
  cout << setprecision(6);

  for (int tc = 1; tc <= T; tc++) {
    int W, L, U, G;
    cin >> W >> L >> U >> G;

    vector<Pt> low(L), up(U);
    for (int i = 0; i < L; i++) {
      long double x, y;
      cin >> x >> y;
      low[i] = {x, y};
    }
    for (int i = 0; i < U; i++) {
      long double x, y;
      cin >> x >> y;
      up[i] = {x, y};
    }

    vector<long double> xs;
    xs.reserve(L + U);
    for (auto& p : low) xs.push_back(p.x);
    for (auto& p : up) xs.push_back(p.x);
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end(), [](long double a, long double b) { return fabsl(a - b) < 1e-18L; }),
             xs.end());

    // Ensure boundaries
    if (xs.front() != 0) xs.insert(xs.begin(), 0);
    if (xs.back() != (long double)W) xs.push_back((long double)W);

    vector<long double> yl = y_at_xs(low, xs);
    vector<long double> yu = y_at_xs(up, xs);

    int m = (int)xs.size() - 1;  // number of intervals
    vector<long double> pref(m + 1, 0.0L);
    for (int i = 0; i < m; i++) {
      long double dx = xs[i + 1] - xs[i];
      long double d0 = yu[i] - yl[i];
      long double d1 = yu[i + 1] - yl[i + 1];
      pref[i + 1] = pref[i] + (d0 + d1) * dx / 2.0L;
    }

    long double total = pref[m];
    cout << "Case #" << tc << ":\n";

    for (int slice = 1; slice < G; slice++) {
      long double target = total * (long double)slice / (long double)G;

      int idx = (int)(lower_bound(pref.begin(), pref.end(), target) - pref.begin()) - 1;
      if (idx < 0) idx = 0;
      if (idx >= m) idx = m - 1;

      // Move idx to correct interval if needed due to precision.
      while (idx + 1 <= m && pref[idx + 1] < target) idx++;
      while (idx > 0 && pref[idx] > target) idx--;

      long double need = target - pref[idx];
      long double x0 = xs[idx];
      long double x1 = xs[idx + 1];
      long double dx = x1 - x0;

      long double d0 = yu[idx] - yl[idx];
      long double d1 = yu[idx + 1] - yl[idx + 1];
      long double k = (d1 - d0) / dx;  // d(t) = d0 + k*t, t in [0,dx]

      long double t;
      if (fabsl(k) < 1e-18L) {
        t = (d0 == 0.0L) ? 0.0L : (need / d0);
      } else {
        // Solve: d0*t + 0.5*k*t^2 = need, t>=0
        long double disc = d0 * d0 + 2.0L * k * need;
        if (disc < 0) disc = 0;
        t = (-d0 + sqrt(disc)) / k;
      }

      if (t < 0) t = 0;
      if (t > dx) t = dx;

      long double cutx = x0 + t;
      cout << (double)cutx << "\n";
    }
  }

  return 0;
}


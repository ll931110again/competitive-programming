/*
 * Solution for problem D: Dome of Circus.
 *
 * Cone: apex (0,0,h), base circle z=0 radius r. Point (x,y,z) lies inside iff
 *   sqrt(x^2+y^2) <= r * (h-z)/h  <=>  r >= h * d / (h - z),  d = sqrt(x^2+y^2).
 * For fixed h > max z_i, minimal r(h) = max_i h*d_i/(h-z_i). Minimize V ∝ h*r(h)^2.
 * V(h) is unimodal (convex); ternary search on h.
 */

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>

using namespace std;

static const int maxn = 10005;

struct Point {
  double x, y, z;
};

static int n;
static Point a[maxn];

static double max_z() {
  double m = 0;
  for (int i = 0; i < n; ++i)
    m = max(m, a[i].z);
  return m;
}

// Minimal base radius for height h (requires h > z_i for all i).
static double min_radius(double h) {
  double r = 0;
  for (int i = 0; i < n; ++i) {
    double dz = h - a[i].z;
    double d = hypot(a[i].x, a[i].y);
    r = max(r, h * d / dz);
  }
  return r;
}

// Proportional to cone volume (1/3)π r^2 h; constant dropped for optimization.
static double vol_scale(double h) {
  double r = min_radius(h);
  return h * r * r;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("dome.in", "r", stdin);
  freopen("dome.out", "w", stdout);

  cin >> n;
  for (int i = 0; i < n; ++i)
    cin >> a[i].x >> a[i].y >> a[i].z;

  double zmx = max_z();
  double lo = zmx + 1e-7;
  double hi = 1e5;

  for (int it = 0; it < 200; ++it) {
    double m1 = lo + (hi - lo) / 3.0;
    double m2 = hi - (hi - lo) / 3.0;
    double v1 = vol_scale(m1);
    double v2 = vol_scale(m2);
    if (v1 < v2)
      hi = m2;
    else
      lo = m1;
  }

  double h = 0.5 * (lo + hi);
  double r = min_radius(h);

  cout << fixed << setprecision(3) << h << ' ' << r << '\n';
  return 0;
}

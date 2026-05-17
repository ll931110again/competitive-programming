#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <utility>
#include <vector>

using namespace std;

struct Pt {
  double x, y;
};

static double dist(const Pt& a, const Pt& b) {
  double dx = a.x - b.x, dy = a.y - b.y;
  return sqrt(dx * dx + dy * dy);
}

static double ang(const Pt& from, const Pt& to) {
  return atan2(to.y - from.y, to.x - from.x);
}

static double norm_angle(double a) {
  const double TWO = 2.0 * M_PI;
  while (a < 0)
    a += TWO;
  while (a >= TWO)
    a -= TWO;
  return a;
}

static double ccw_delta(double a, double b) {
  // smallest d in [0,2pi) such that a + d == b (mod 2pi)
  a = norm_angle(a);
  b = norm_angle(b);
  double d = b - a;
  if (d < 0)
    d += 2.0 * M_PI;
  return d;
}

static int simulate(const vector<Pt>& pts, double r0) {
  const double EPS = 1e-10;
  const double MIN_DEL = 1e-7; // require a non-trivial rotation to count/move
  const int N = (int)pts.size();
  int bends = 0;

  Pt pivot = pts[0];   // start at (0,0)
  double L = r0;       // remaining rotating segment length
  double theta = M_PI; // starts pointing left
  int prev_hit = -1;
  int repeats = 0;

  auto same_pt = [&](const Pt& a, const Pt& b) {
    return fabs(a.x - b.x) < 1e-12 && fabs(a.y - b.y) < 1e-12;
  };

  // Heuristic loop guard.
  // If we see a very similar state again, assume we are orbiting in a loop.
  vector<tuple<long long, long long, long long>> seen;
  seen.reserve(2048);
  for (int step = 0; step < 5000; step++) {
    {
      long long qx = llround(pivot.x * 1e6);
      long long qy = llround(pivot.y * 1e6);
      long long ql = llround(L * 1e6);
      long long qt = llround(norm_angle(theta) * 1e6);
      auto key = make_tuple((qx << 20) ^ qy, ql, qt);
      // linear scan is fine for heuristic; keep it cheap by limiting.
      for (int i = max(0, (int)seen.size() - 200); i < (int)seen.size(); i++) {
        if (seen[i] == key)
          return bends;
      }
      seen.push_back(key);
    }

    int best = -1;
    double best_d = numeric_limits<double>::infinity();
    double best_len = -1.0;
    double best_abs_angle = 0.0;

    for (int i = 0; i < N; i++) {
      if (same_pt(pivot, pts[i]))
        continue;
      double d = dist(pivot, pts[i]);
      if (d + 1e-9 > L)
        continue;
      double a = ang(pivot, pts[i]);
      double del = ccw_delta(theta, a);
      if (del < MIN_DEL)
        continue;

      // Prefer the first encountered by CCW rotation.
      // If multiple points are collinear (same del), choose the farthest (closest to moving end).
      if (del + EPS < best_d) {
        best_d = del;
        best = i;
        best_len = d;
        best_abs_angle = a;
      } else if (fabs(del - best_d) <= 1e-9) {
        if (d > best_len + 1e-9) {
          best = i;
          best_len = d;
          best_abs_angle = a;
        }
      }
    }

    if (best == -1)
      break;

    // If we're repeatedly hitting the same target with nearly the same state,
    // assume we are essentially orbiting and further bends are not meaningful.
    if (best == prev_hit && best_d < 1e-4) {
      if (++repeats >= 3)
        break;
    } else {
      repeats = 0;
    }

    bends++;

    Pt next = pts[best];
    double used = dist(pivot, next);
    L -= used;
    if (L <= 1e-9)
      break;

    // After wrapping around 'next', we now rotate around 'next'. At the moment
    // of contact, the moving end is further along the same ray, so the new
    // rotating segment starts in the same direction as the old one.
    theta = best_abs_angle;
    pivot = next;
    prev_hit = best;
  }

  return bends;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int N;
    long long Rll;
    cin >> N >> Rll;
    vector<Pt> pts(N);
    for (int i = 0; i < N; i++) {
      long long x, y;
      cin >> x >> y;
      pts[i] = Pt{(double)x, (double)y};
    }

    double R = (double)Rll;

    // Candidate cut lengths: near R, plus just below distances to points.
    vector<double> cand;
    cand.reserve(5000);
    cand.push_back(R);
    // Dense sampling close to R (often optimal is "cut a tiny bit").
    for (int i = 0; i <= 2000; i++) {
      double r = R - i * 1e-3;
      if (r > 0)
        cand.push_back(r);
      else
        break;
    }
    for (int i = 0; i <= 400; i++) {
      double r = R - i * 0.1;
      if (r > 0)
        cand.push_back(r);
      else
        break;
    }
    for (int i = 1; i < N; i++) {
      double d0 = dist(pts[0], pts[i]);
      if (d0 <= R + 1e-9) {
        cand.push_back(max(0.0, d0 - 1e-6));
        cand.push_back(max(0.0, d0 - 1e-3));
        cand.push_back(max(0.0, d0 - 0.5));
      }
    }

    // Randomized sampling to catch "non-obvious" cut lengths.
    // This is purely heuristic; keep it bounded for speed.
    std::mt19937_64 rng(1234567ULL + (unsigned long long)tc * 1000003ULL);
    std::uniform_real_distribution<double> uni01(0.0, 1.0);
    for (int t = 0; t < 2500; t++) {
      double r = R * uni01(rng);
      cand.push_back(r);
      // Also bias toward values close to R.
      double r2 = max(0.0, R - 1.0 * uni01(rng));
      cand.push_back(r2);
    }

    sort(cand.begin(), cand.end());
    cand.erase(
        unique(cand.begin(), cand.end(), [](double a, double b) { return fabs(a - b) < 1e-9; }),
        cand.end());

    int best = 0;
    int evals = 0;
    for (double r : cand) {
      if (r <= 0)
        continue;
      best = max(best, simulate(pts, r));
      if (++evals >= 8000)
        break; // cap work
    }

    cout << "Case #" << tc << ": " << best << "\n";
  }
  return 0;
}
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

using int64 = long long;

static int floor_log2_u64(unsigned long long x) {
  int r = -1;
  while (x) {
    x >>= 1;
    r++;
  }
  return r;
}

struct Points {
  // strict all-in points, increasing, always includes 1, all <= V/2
  vector<int64> xs;
  vector<long double> P; // probability to reach V from xs[i]
  vector<int64> all_in;  // strict + optional all-in points (subset of inflections)
};

static int max_doubles_k(int64 x, int64 M) {
  // Maximum k >= 1 such that x * 2^(k-1) <= M.
  if (x <= 0)
    return 0;
  unsigned long long t = (unsigned long long)(M / x);
  if (t == 0)
    return 0;
  return floor_log2_u64(t) + 1;
}

static long double lose_value_ld(int64 x, int64 M) {
  int k = max_doubles_k(x, M);
  // lose after k consecutive losses: x - (2^k - 1) * x = -(2^k - 2) * x
  // compute as -x * (2^k - 2) in long double without overflow
  long double two_k = ldexpl(1.0L, k);
  return -((two_k - 2.0L) * (long double)x);
}

static long double interp(long double x, long double x0, long double y0, long double x1,
                          long double y1) {
  if (x1 == x0)
    return y0;
  return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

static long double eval_P(const vector<int64>& strictX, const vector<long double>& strictP,
                          int from_idx, int64 V, int64 x) {
  if (x <= 0)
    return 0.0L;
  if (x >= V)
    return 1.0L;
  // find segment in [strictX[from_idx], ..., V]
  int n = (int)strictX.size();
  int lo = from_idx, hi = n; // hi==n means endpoint V
  // upper_bound on strictX to find first > x
  int ub = (int)(upper_bound(strictX.begin() + from_idx, strictX.end(), x) - strictX.begin());
  if (ub == from_idx) {
    // x < strictX[from_idx], shouldn't happen if called correctly
    return interp((long double)x, 0.0L, 0.0L, (long double)strictX[from_idx], strictP[from_idx]);
  }
  int left = ub - 1;
  long double x0 = (long double)strictX[left];
  long double y0 = strictP[left];
  long double x1, y1;
  if (ub == n) {
    x1 = (long double)V;
    y1 = 1.0L;
  } else {
    x1 = (long double)strictX[ub];
    y1 = strictP[ub];
  }
  return interp((long double)x, x0, y0, x1, y1);
}

static Points build_strategy(int64 M, int64 V) {
  // Build strict all-in points by scanning inflection points.
  vector<int64> inf;
  inf.reserve(128);
  for (int i = 0;; i++) {
    int64 x = (i >= 63) ? 0 : (M >> i);
    if (x <= 0)
      break;
    if (x <= V / 2)
      inf.push_back(x);
  }
  inf.push_back(1);
  sort(inf.begin(), inf.end());
  inf.erase(unique(inf.begin(), inf.end()), inf.end());

  Points res;
  res.xs.clear();
  res.all_in.clear();

  long double bestLose = 0.0L; // "lowest total yet" (most negative) seen among strict points
  for (int64 x : inf) {
    if (x < 1)
      continue;
    long double lv = lose_value_ld(x, M);
    if (res.xs.empty()) {
      res.xs.push_back(1);
      bestLose = lose_value_ld(1, M);
      res.all_in.push_back(1);
      continue;
    }
    if (lv + 1e-18L < bestLose) { // strictly lower
      res.xs.push_back(x);
      bestLose = lv;
      res.all_in.push_back(x);
    } else if (fabsl(lv - bestLose) <= 1e-18L) {
      // Optional all-in point (equally good as waiting).
      res.all_in.push_back(x);
    }
  }

  sort(res.xs.begin(), res.xs.end());
  res.xs.erase(unique(res.xs.begin(), res.xs.end()), res.xs.end());
  sort(res.all_in.begin(), res.all_in.end());
  res.all_in.erase(unique(res.all_in.begin(), res.all_in.end()), res.all_in.end());

  // Compute P for strict points from top down.
  int n = (int)res.xs.size();
  res.P.assign(n, 0.0L);

  for (int idx = n - 1; idx >= 0; idx--) {
    int64 y = res.xs[idx];
    int64 z = (idx + 1 < n) ? res.xs[idx + 1] : V;
    int k = max_doubles_k(y, M);
    long double q = ldexpl(1.0L, -k); // 2^{-k}
    long double p = 1.0L - q;

    if (2 * y >= z) {
      // P(2y) is already determined by points >= z
      long double P2y = eval_P(res.xs, res.P, idx + 1, V, 2 * y);
      res.P[idx] = p * P2y;
    } else {
      // 2y is between y and z; linearity gives equation solvable for Py.
      long double Pz = (z == V) ? 1.0L : res.P[idx + 1];
      // Py = (p*y*Pz) / ( q*(z - y) + p*y )
      long double denom = q * (long double)(z - y) + p * (long double)y;
      res.P[idx] = (denom == 0.0L) ? 0.0L : (p * (long double)y * Pz) / denom;
    }
  }

  return res;
}

static long double probability_from(int64 A, int64 M, int64 V, const Points& st) {
  if (A <= 0)
    return 0.0L;
  if (A >= V)
    return 1.0L;
  // If A is strict, return it.
  auto it = lower_bound(st.xs.begin(), st.xs.end(), A);
  if (it != st.xs.end() && *it == A) {
    return st.P[it - st.xs.begin()];
  }
  // interpolate between surrounding strict points (or 0/V)
  int idx = (int)(it - st.xs.begin());
  int64 leftX = (idx == 0) ? 0 : st.xs[idx - 1];
  long double leftP = (idx == 0) ? 0.0L : st.P[idx - 1];
  int64 rightX = (idx == (int)st.xs.size()) ? V : st.xs[idx];
  long double rightP = (idx == (int)st.xs.size()) ? 1.0L : st.P[idx];
  return interp((long double)A, (long double)leftX, leftP, (long double)rightX, rightP);
}

static int64 max_first_bet(int64 A, int64 M, int64 V, const Points& st) {
  int64 cap = min<int64>({A, M, V - A});
  if (cap <= 0)
    return 0;
  // If A is an all-in point (strict or optional), bet all.
  auto ita = lower_bound(st.all_in.begin(), st.all_in.end(), A);
  if (ita != st.all_in.end() && *ita == A)
    return cap;

  // Otherwise, between strict points the probability is linear, so we can bet
  // until we hit a strict point on either side.
  auto it = lower_bound(st.xs.begin(), st.xs.end(), A);
  int idx = (int)(it - st.xs.begin());
  int64 left = (idx == 0) ? 0 : st.xs[idx - 1];
  int64 right = (idx == (int)st.xs.size()) ? V : st.xs[idx];
  int64 room = min(A - left, right - A);
  return min(cap, room);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  cout.setf(std::ios::fixed);
  cout << setprecision(9);

  for (int tc = 1; tc <= T; tc++) {
    int64 A, M, V;
    cin >> A >> M >> V;

    Points st = build_strategy(M, V);
    long double p = probability_from(A, M, V, st);
    int64 z = max_first_bet(A, M, V, st);

    cout << "Case #" << tc << ": " << (double)p << " " << z << "\n";
  }
  return 0;
}

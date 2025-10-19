#include <cmath>
#include <iostream>
using namespace std;

const int MAX_EXACT_H = 100000000;
const long long MAX_CHUNK = 1000000000;
const int SMALL_LIMIT = 100000000;
const double EULER = 0.5772156649015328606065120900824;

double h[MAX_EXACT_H] = {};

double H(long long k) {
  if (k < MAX_EXACT_H) {
    return h[(int)k];
  }
  // If k is high, we use an approximation which should be more accurate
  // than 1e-7.
  return log(k) + EULER;
}

double solve_small(long long N, int P) {
  double p = P / 100.0;
  double ans = 0;

  for (long k = 0; k < N; k++) {
    double bestCost = 999999;

    for (int C = 1; C <= 100; C++) {
      double chanceNew = min((C - 1) * p, 1.0);
      double chance = chanceNew + (1.0 - chanceNew) * (N - k) / (float)N;
      double cost = C / chance;
      if (cost < bestCost) {
        bestCost = cost;
      }
      if (chanceNew >= 1 || P == 0) {
        break;
      }
    }
    ans += bestCost;
  }

  return ans;
}

double solve() {
  long long N;
  int P;
  cin >> N >> P;

  if (N < SMALL_LIMIT) {
    return solve_small(N, P);
  }

  double p = P / 100.0;

  // M is the maximum number of coins we would ever use,
  // the amount that guarantees a new toy
  double M = P > 0 ? ceil(100.0 / P) + 1 : 999;

  // B1 is the breakpoint where we switch from 1 to M-1 coins
  long long B1 = (long long)ceil(N - (N * p / (p + 1)));

  // Want to find ceil(K) such that (M-1)/((M-2)p + (1-(M-2)p) * (N-K) / N) = M
  auto binary_search = [=]() -> long long {
    long long lo = 0, hi = N;
    while (lo + 1 < hi) {
      long long mid = (lo + hi) / 2;
      double val =
          (M - 1) / ((M - 2) * p + (1 - (M - 2) * p) * (float)(N - mid) / N);
      if (val > M) {
        hi = mid;
      } else if (val < M) {
        lo = mid + 1;
      }
    }
    return hi;
  };

  // B2 is the breakpoint where we switch from M-1 coins to M
  long long B2 = P > 0 ? binary_search() : N;

  // Rounding in th ebinary search can cause B2 to be less than B1, but
  // that's never correct.
  B2 = max(B2, B1);

  double ans = 0;

  // Until we reach B1 toys, we spend 1 coin.
  // Our expected expenditure is then the sum of
  // 1 / (1 - x/N) for x in [0, B1-1]
  //
  // That's annoying to sum, so let's rearrange it to the sum of
  // N * (1/x) for x in [N, N-B1+1]. And luckily we can approximate
  // the sum of arbitrary segments of the harmonic series.
  ans += N * (H(N) - H(N - B1));

  // For x in [B1, B2-1], we spend M-1 coins.
  // We'll use a trapezoid approximation of the function.
  long long chunkSize = (B2 - B1) / MAX_CHUNK + 1;
  long long left = B1;
  while (left < B2) {
    long long right = min(left + chunkSize - 1, B2 - 1);
    double size = right - left + 1;
    double leftval =
        (M - 1) / ((M - 2) * p + (1 - (M - 2) * p) * (float)(N - left) / N);
    double rightval =
        (M - 1) / ((M - 2) * p + (1 - (M - 2) * p) * (float)(N - right) / N);
    double midval = (leftval + rightval) / 2;
    ans += midval * size;
    left = right + 1;
  }

  // And for x in [B2, N-1] we spend M coins.
  ans += (N - B2) * M;

  return ans;
}

int main() {
  // Precompute the first terms of the harmonic series
  h[0] = 0;
  for (int i = 1; i < MAX_EXACT_H; i++) {
    h[i] = h[i - 1] + 1.0 / i;
  }

  int T;
  cin >> T;
  cout.precision(10);
  for (int t = 1; t <= T; t++) {
    cerr << t << endl;
    cout << "Case #" << t << ": " << fixed << solve() << endl;
  }
  return 0;
}

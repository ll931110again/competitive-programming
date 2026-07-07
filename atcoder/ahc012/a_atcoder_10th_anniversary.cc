// AHC012 A - AtCoder 10th Anniversary: https://atcoder.jp/contests/ahc012/tasks/ahc012_a
//
// Grid of horizontal/vertical cuts (slightly skewed so no strawberry lies on a line).
// Simulated annealing moves cut positions; early SA weights larger d to avoid local minima.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int kMaxN = 10000;
constexpr int kCuts = 100;
constexpr int kFar = 100000000;
constexpr int kMaxCoord = 10020;
constexpr double kTimeLimit = 2.96;

struct XorShift {
  uint32_t x = 123456789u;
  uint32_t next() {
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
  }
  int nextInt(int hi) { return (int)(next() % (uint32_t)hi); }
  int nextInt(int lo, int hi) { return lo + nextInt(hi - lo); }
  double nextDouble() { return next() * (1.0 / 4294967296.0); }
} rng;

double now() {
  using clock = chrono::steady_clock;
  static const auto start = clock::now();
  return chrono::duration<double>(clock::now() - start).count();
}

struct Solver {
  int n = 0, k = 0;
  int need[11]{};
  array<pair<int, int>, kMaxN> berries{};

  int xCuts = 9;
  int yCuts = kCuts - xCuts;
  vector<int> cutX, cutY, xPos;
  int cur[kCuts]{}, best[kCuts]{};

  int cnt[11]{};
  int yGroup[2 * kMaxCoord]{};
  int groups[kCuts]{};
  int yLo[kCuts], yHi[kCuts];

  int rawScore = 0;
  double moment = 1.0;

  void read() {
    cin >> n >> k;
    int total = 0;
    for (int d = 1; d <= 10; ++d) {
      cin >> need[d];
      total += need[d];
    }
    for (int i = 0; i < n; ++i)
      cin >> berries[i].first >> berries[i].second;

    while ((xCuts + 1) * (yCuts + 1) < total)
      ++xCuts, --yCuts;
  }

  void buildCandidates() {
    sort(berries.begin(), berries.begin() + n);
    for (int i = 0; i + 1 < n; ++i)
      if (berries[i + 1].first > berries[i].first) {
        cutX.push_back(berries[i].first);
        xPos.push_back(i + 1);
      }

    vector<int> ys;
    ys.reserve(n);
    for (int i = 0; i < n; ++i)
      ys.push_back(berries[i].second);
    sort(ys.begin(), ys.end());
    for (int i = 0; i + 1 < n; ++i)
      if (ys[i + 1] > ys[i])
        cutY.push_back(ys[i]);
  }

  double evaluate(bool yGroupValid) {
    array<int, kCuts> sorted{};
    copy(cur, cur + kCuts, sorted.begin());
    sort(sorted.begin(), sorted.begin() + xCuts);
    sort(sorted.begin() + xCuts, sorted.end());

    fill(begin(cnt), end(cnt), 0);

    if (!yGroupValid) {
      for (int i = 0; i <= yCuts; ++i) {
        const int lo = i == 0 ? 0 : kMaxCoord + cutY[sorted[xCuts + i - 1]] + 1;
        const int hi =
            i == yCuts ? 2 * kMaxCoord : kMaxCoord + cutY[sorted[xCuts + i]] + 1;
        if (lo == yLo[i] && hi == yHi[i])
          continue;
        yLo[i] = lo;
        yHi[i] = hi;
        for (int j = lo; j < hi; ++j)
          yGroup[j] = i;
      }
    }

    for (int i = 0; i <= xCuts; ++i) {
      fill(begin(groups), end(groups), 0);
      const int lo = i == 0 ? 0 : xPos[sorted[i - 1]];
      const int hi = i == xCuts ? n : xPos[sorted[i]];
      for (int j = lo; j < hi; ++j)
        ++groups[yGroup[berries[j].second + kMaxCoord]];
      for (int j = 0; j <= yCuts; ++j)
        ++cnt[groups[j]];
    }

    double score = 0;
    rawScore = 0;
    for (int d = 1; d <= 10; ++d) {
      const int v = min(cnt[d], need[d]);
      score += v * pow(d / 10.0, moment * 2.85);
      rawScore += v;
    }
    return score;
  }

  void anneal() {
    for (int i = 0; i < xCuts; ++i)
      cur[i] = rng.nextInt((int)cutX.size());
    for (int i = 0; i < yCuts; ++i)
      cur[xCuts + i] = rng.nextInt((int)cutY.size());

    moment = 1.0;
    double bestEval = evaluate(false);
    double curEval = bestEval;
    int bestRaw = rawScore;
    copy(cur, cur + kCuts, best);

    const int maxX = max(0, (int)cutX.size() - 1);
    const int maxY = max(0, (int)cutY.size() - 1);
    bool yGroupValid = true;

    for (;;) {
      const double t = now() / kTimeLimit;
      if (t >= 1.0)
        break;
      moment = 1.0 - t;

      const bool local = rng.nextDouble() >= 0.35;
      const int axis = rng.nextInt(2);
      const int p = axis == 0 ? rng.nextInt(xCuts) : rng.nextInt(xCuts, kCuts);

      int v;
      if (local) {
        const int delta =
            (int)(1 + rng.nextDouble() * rng.nextDouble() * 13.75) * (rng.nextInt(2) ? 1 : -1);
        v = max(0, min(axis == 0 ? maxX : maxY, cur[p] + delta));
      } else {
        v = rng.nextInt(axis == 0 ? (int)cutX.size() : (int)cutY.size());
      }

      if (axis == 1)
        yGroupValid = false;
      const int old = cur[p];
      if (v == old)
        continue;

      cur[p] = v;
      const double nextEval = evaluate(yGroupValid);
      yGroupValid = true;

      const double temp = 0.8 * pow(0.05 / 0.8, t);
      if (rawScore > bestRaw) {
        bestRaw = rawScore;
        copy(cur, cur + kCuts, best);
      }
      if (nextEval >= curEval || rng.nextDouble() < exp((nextEval - curEval) / temp)) {
        curEval = nextEval;
      } else {
        cur[p] = old;
        if (axis == 1)
          yGroupValid = false;
      }
    }
  }

  void output() const {
    array<int, kCuts> sorted{};
    copy(best, best + kCuts, sorted.begin());
    sort(sorted.begin(), sorted.begin() + xCuts);
    sort(sorted.begin() + xCuts, sorted.end());

    cout << kCuts << '\n';
    for (int i = 0; i < xCuts; ++i) {
      const int x = cutX[sorted[i]];
      cout << x << ' ' << -kFar << ' ' << x + 1 << ' ' << kFar << '\n';
    }
    for (int i = 0; i < yCuts; ++i) {
      const int y = cutY[sorted[xCuts + i]];
      cout << -kFar << ' ' << y << ' ' << kFar << ' ' << y + 1 << '\n';
    }
  }
};

}  // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver solver;
  solver.read();
  solver.buildCandidates();
  solver.anneal();
  solver.output();
  return 0;
}

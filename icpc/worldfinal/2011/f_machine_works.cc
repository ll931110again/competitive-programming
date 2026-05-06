#include <bits/stdc++.h>

using namespace std;

struct Machine {
  long long dayAvailable;
  long long buyPrice;
  long long resalePrice;
  long long dailyProfit;
};

struct Line {
  long long slope = 0;
  long long intercept = -(1LL << 60);
  Line() = default;
  Line(long long slope_, long long intercept_)
      : slope(slope_), intercept(intercept_) {}
  inline long long value(long long x) const {
    __int128 v = (__int128)slope * (__int128)x + (__int128)intercept;
    if (v < (__int128)LLONG_MIN)
      return LLONG_MIN;
    if (v > (__int128)LLONG_MAX)
      return LLONG_MAX;
    return (long long)v;
  }
};

// Dynamic convex hull trick for maximum queries (arbitrary insertion order).
// Adapted from the well-known "LineContainer" (KACTL style), but using
// __int128-safe division.
struct DynamicCHTMax {
  struct HullLine {
    mutable long long m = 0;            // slope
    mutable long long b = -(1LL << 60); // intercept
    mutable long long p = 0; // first x where this line is best (inclusive)
    bool operator<(const HullLine &o) const { return m < o.m; }
    bool operator<(long long x) const { return p < x; }
  };

  static constexpr long long INF_X = numeric_limits<long long>::max();

  multiset<HullLine, less<>> hull;

  static long long floordiv(__int128 a, __int128 b) {
    // floor(a / b) for b > 0 or b < 0.
    if (b < 0)
      a = -a, b = -b;
    if (a >= 0)
      return (long long)(a / b);
    return (long long)(-(((-a) + b - 1) / b));
  }

  static long long intersectX(const HullLine &x, const HullLine &y) {
    // Smallest integer x where y(x) >= x(x). Both are linear. Assumes x.m !=
    // y.m. Solve y.m*t + y.b >= x.m*t + x.b  =>  (y.b - x.b) >= (x.m - y.m)*t
    // If denom > 0: t <= (y.b - x.b)/denom ; but we want smallest t where y
    // overtakes x KACTL uses: p = floor((b2-b1)/(m1-m2)) for max with
    // increasing slopes, with care. We'll compute as floor((x.b - y.b)/(y.m -
    // x.m)) to match standard form. We want: x.m*t + x.b >= y.m*t + y.b for t
    // <= p ; so p is the last t where x dominates. p = floor((y.b - x.b)/(x.m -
    // y.m)).
    __int128 num2 = (__int128)y.b - (__int128)x.b;
    __int128 den2 = (__int128)x.m - (__int128)y.m;
    return floordiv(num2, den2);
  }

  bool isect(multiset<HullLine>::iterator x, multiset<HullLine>::iterator y) {
    if (y == hull.end()) {
      x->p = INF_X;
      return false;
    }
    if (x->m == y->m) {
      x->p = (x->b >= y->b) ? INF_X : numeric_limits<long long>::min();
    } else {
      x->p = intersectX(*x, *y);
    }
    return x->p >= y->p;
  }

  void addLine(long long m, long long b) {
    auto z = hull.insert({m, b, 0});
    auto y = z++;
    auto x = y;

    while (isect(y, z))
      z = hull.erase(z);
    if (x != hull.begin() && isect(--x, y))
      isect(x, hull.erase(y));
    while ((y = x) != hull.begin() && (--x)->p >= y->p)
      isect(x, hull.erase(y));
  }

  long long query(long long x) const {
    if (hull.empty())
      return LLONG_MIN;
    auto l = *hull.lower_bound(x);
    __int128 v = (__int128)l.m * (__int128)x + (__int128)l.b;
    if (v < (__int128)LLONG_MIN)
      return LLONG_MIN;
    if (v > (__int128)LLONG_MAX)
      return LLONG_MAX;
    return (long long)v;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int caseNo = 1;

  int machineCount;
  long long initialCash, lastDay;
  while (cin >> machineCount >> initialCash >> lastDay) {
    if (machineCount == 0 && initialCash == 0 && lastDay == 0)
      break;

    vector<Machine> machines;
    machines.reserve(machineCount);
    for (int i = 0; i < machineCount; i++) {
      Machine machine;
      cin >> machine.dayAvailable >> machine.buyPrice >> machine.resalePrice >>
          machine.dailyProfit;
      machines.push_back(machine);
    }
    sort(machines.begin(), machines.end(),
         [](const Machine &a, const Machine &b) {
           if (a.dayAvailable != b.dayAvailable)
             return a.dayAvailable < b.dayAvailable;
           return a.buyPrice < b.buyPrice;
         });

    DynamicCHTMax cashAtDay;
    cashAtDay.addLine(0, initialCash); // keep cash, no machine

    for (int i = 0; i < machineCount;) {
      long long day = machines[i].dayAvailable;
      long long cashToday = cashAtDay.query(day);

      vector<Line> toAdd;
      for (; i < machineCount && machines[i].dayAvailable == day; i++) {
        const auto &machine = machines[i];
        if (cashToday < machine.buyPrice)
          continue;
        // If we buy this machine on 'day' using 'money' cash, then selling on
        // day x yields: money - p + r + g*(x - day - 1) = g*x + (money - p + r
        // - g*(day+1))
        long long intercept = cashToday - machine.buyPrice +
                              machine.resalePrice -
                              machine.dailyProfit * (day + 1);
        toAdd.push_back(Line(machine.dailyProfit, intercept));
      }

      for (const auto &line : toAdd)
        cashAtDay.addLine(line.slope, line.intercept);
    }

    long long maxCashEnd = cashAtDay.query(lastDay + 1);
    cout << "Case " << caseNo++ << ": " << maxCashEnd << "\n";
  }

  return 0;
}

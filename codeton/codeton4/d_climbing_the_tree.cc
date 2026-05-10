#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;

int T, Q;

struct Range {
  long long minValue;
  long long maxValue;
};

Range getRange(int a, int b, int n) {
  if (n == 1) {
    return Range{.minValue = 1, .maxValue = a};
  }

  long long baseHeight = 1LL * (n - 1) * (a - b);
  return Range{.minValue = baseHeight + b + 1, .maxValue = baseHeight + a};
}

Range intersect(const Range &lhs, const Range &rhs, bool *ok) {
  (*ok) = false;

  if (lhs.maxValue < rhs.minValue || rhs.maxValue < lhs.minValue) {
    return lhs;
  }

  Range result = {.minValue = max(lhs.minValue, rhs.minValue),
                  .maxValue = min(lhs.maxValue, rhs.maxValue)};
  if (result.minValue > result.maxValue) {
    return lhs;
  }

  (*ok) = true;
  return result;
}

long long query(int a, int b, long long treeHeight) {
  if (treeHeight <= a) {
    return 1;
  }

  long long n = (treeHeight - a) / (a - b);
  if ((treeHeight - a) % (a - b)) {
    n++;
  }

  // Last day
  n++;

  return n;
}

void solve() {
  cin >> Q;
  Range lhs = {.minValue = 1, .maxValue = (long long)1e18};

  while (Q--) {
    int eventType, a, b, n;
    cin >> eventType >> a >> b;
    if (eventType == 1) {
      cin >> n;
      bool ok;
      Range rhs = getRange(a, b, n);
      lhs = intersect(lhs, rhs, &ok);
      cout << ok << ' ';
    } else {
      long long minDates = query(a, b, lhs.minValue);
      long long maxDates = query(a, b, lhs.maxValue);
      if (minDates != maxDates) {
        cout << "-1 ";
      } else {
        cout << minDates << ' ';
      }
    }
  }
  cout << '\n';
}

int main() {
  cin >> T;
  while (T--) {
    solve();
  }
  return 0;
}

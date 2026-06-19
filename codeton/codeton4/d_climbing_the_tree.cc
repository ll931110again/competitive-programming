// Codeforces 1810 (CodeTON Round 4 (Div. 1 + Div. 2, Rated, Prizes!)) — D. Climbing the Tree
// Submission: https://codeforces.com/contest/1810/submission/201604776

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int T, Q;

struct Range {
  i64 min_value;
  i64 max_value;
};

Range get_range(int a, int b, int n) {
  if (n == 1) {
    return Range{.min_value = 1, .max_value = a};
  }

  i64 base_height = 1LL * (n - 1) * (a - b);
  return Range{.min_value = base_height + b + 1, .max_value = base_height + a};
}

Range intersect(const Range& lhs, const Range& rhs, bool* ok) {
  (*ok) = false;

  if (lhs.max_value < rhs.min_value || rhs.max_value < lhs.min_value) {
    return lhs;
  }

  Range result = {.min_value = max(lhs.min_value, rhs.min_value),
                  .max_value = min(lhs.max_value, rhs.max_value)};
  if (result.min_value > result.max_value) {
    return lhs;
  }

  (*ok) = true;
  return result;
}

i64 query(int a, int b, i64 tree_height) {
  if (tree_height <= a) {
    return 1;
  }

  i64 n = (tree_height - a) / (a - b);
  if ((tree_height - a) % (a - b)) {
    n++;
  }

  // Last day
  n++;

  return n;
}

void solve() {
  scanf("%d", &Q);
  Range lhs = {.min_value = 1, .max_value = (i64)1e18};

  while (Q--) {
    int event_type, a, b, n;
    scanf("%d %d %d", &event_type, &a, &b);
    if (event_type == 1) {
      scanf("%d", &n);
      bool ok;
      Range rhs = get_range(a, b, n);
      lhs = intersect(lhs, rhs, &ok);
      printf("%d ", ok);
    } else {
      i64 min_dates = query(a, b, lhs.min_value);
      i64 max_dates = query(a, b, lhs.max_value);
      if (min_dates != max_dates) {
        printf("-1 ");
      } else {
        printf("%lld ", min_dates);
      }
    }
  }
  printf("\n");
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    solve();
  }
  return 0;
}

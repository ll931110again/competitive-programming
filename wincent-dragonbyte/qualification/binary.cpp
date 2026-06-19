
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int T;
i64 n;
i64 MAX_N = 1e18 + 1;

i64 evaluate(vector<int> polynomial, i64 mid) {
  i64 ans = 0;
  for (auto coef : polynomial) {
    if (MAX_N / mid < ans) {
      return MAX_N;
    }
    ans = ans * mid + coef;
  }
  return ans;
}

i64 solve_polynomial(i64 n, vector<int> polynomial) {
  i64 low = 1, high = n, ans = -1;
  while (low <= high) {
    i64 mid = (low + high) / 2;
    i64 tmp = evaluate(polynomial, mid);
    if (tmp == n) {
      return mid;
    }
    if (tmp > n) {
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }
  return -1;
}

bool is_valid(i64 n, i64 base) {
  while (n) {
    if (n % base > 1) {
      return false;
    }
    n /= base;
  }
  return true;
}

i64 solve(i64 n) {
  if (n == 1) {
    return 3;
  }
  if (n == 2) {
    return -1;
  }
  if (n == 3) {
    return 3;
  }

  i64 ans = n - 1;
  vector<vector<int>> polynomials = {{1, 0, 0},    {1, 0, 1},    {1, 1, 0},    {1, 1, 1},
                                     {1, 0, 0, 0}, {1, 0, 0, 1}, {1, 0, 1, 0}, {1, 0, 1, 1},
                                     {1, 1, 0, 0}, {1, 1, 0, 1}, {1, 1, 1, 0}, {1, 1, 1, 1}};
  for (const auto polynomial : polynomials) {
    auto value = solve_polynomial(n, polynomial);
    if (value > 2 && value < ans) {
      ans = value;
    }
  }

  for (i64 base = 3; base * base * base * base <= n; base++) {
    if (is_valid(n, base)) {
      ans = min(ans, base);
      break;
    }
  }

  return ans;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    cout << solve(n) << endl;
  }
  return 0;
}

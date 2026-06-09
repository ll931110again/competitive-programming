#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 1000005;
int T, n;
pair<int, int> ts[k_max_n];

double solve() {
  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> ts[i].first >> ts[i].second;
  }

  double lower_bound = 0, upper_bound = 1e9;
  for (int i = 0; i < n; i++) {
    // distance is (i + 1)
    lower_bound = max(lower_bound, 1.0 * (i + 1) / ts[i].second);
    if (ts[i].first > 0) {
      upper_bound = min(upper_bound, 1.0 * (i + 1) / ts[i].first);
    }
  }

  if (lower_bound > upper_bound + 1e-6) {
    return -1;
  }
  return lower_bound;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  for (int it = 1; it <= T; it++) {
    cout << fixed << setprecision(9) << "Case #" << it << ": " << solve() << '\n';
  }
  return 0;
}

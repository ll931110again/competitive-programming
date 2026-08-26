// Codeforces Spectral::Cup 2026 Round 2 — Roadworks
// https://codeforces.com/contest/2229/problem/G
//
// Push DP on prefix maxima (editorial "absolute cinema" solution).
// Complexity: O(n log n) per test.

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int64_t kNegInf = -(int64_t)4e18;

void solve_one() {
  int n, start;
  int64_t days;
  cin >> n >> days >> start;
  start--;
  vector<int64_t> h(n);
  for (int64_t& x : h) {
    cin >> x;
  }
  vector<int64_t> road(n - 1);
  for (int64_t& x : road) {
    cin >> x;
  }

  vector<int64_t> arrive(n);
  arrive[start] = 1;
  for (int i = start; i + 1 < n; i++) {
    arrive[i + 1] = max(arrive[i] + 1, road[i]);
    if (i == start && road[i] == 1) {
      arrive[i + 1] = 1;
    }
  }
  for (int i = start - 1; i >= 0; i--) {
    arrive[i] = max(arrive[i + 1] + 1, road[i]);
    if (i == start - 1 && road[i] == 1) {
      arrive[i] = 1;
    }
  }

  vector<int> next_higher(n);
  vector<pair<int, int64_t>> stack;
  stack.push_back({n, (int64_t)2e18});
  for (int i = n - 1; i >= 0; i--) {
    while (stack.back().second <= h[i]) {
      stack.pop_back();
    }
    next_higher[i] = stack.back().first;
    stack.push_back({i, h[i]});
  }
  vector<int> prev_higher(n);
  stack.clear();
  stack.push_back({-1, (int64_t)2e18});
  for (int i = 0; i < n; i++) {
    while (stack.back().second <= h[i]) {
      stack.pop_back();
    }
    prev_higher[i] = stack.back().first;
    stack.push_back({i, h[i]});
  }

  vector<int64_t> pref(n + 1);
  for (int i = 0; i < n; i++) {
    pref[i + 1] = pref[i] + h[i];
  }
  auto interior = [&](int i, int j) -> int64_t {
    if (i > j) {
      swap(i, j);
    }
    return pref[j] - pref[i + 1];
  };

  vector<int64_t> dp(n, kNegInf);
  for (int i = 0; i < n; i++) {
    dp[i] = 0;
  }
  vector<pair<int64_t, int>> order;
  order.reserve(n);
  for (int i = 0; i < n; i++) {
    order.push_back({arrive[i], i});
  }
  sort(order.begin(), order.end());

  int64_t ans = 0;
  for (const auto& [time, i] : order) {
    if (dp[i] < 0) {
      continue;
    }
    if (time <= days) {
      ans = max(ans, dp[i] + (days - time + 1) * h[i]);
    }
    for (int j : {next_higher[i], prev_higher[i]}) {
      if (j < 0 || j >= n) {
        continue;
      }
      const int dist = abs(j - i);
      if (dist > arrive[j] - arrive[i]) {
        continue;
      }
      const int64_t wait = arrive[j] - arrive[i] - dist + 1;
      dp[j] = max(dp[j], dp[i] + wait * h[i] + interior(i, j));
    }
  }
  cout << ans << '\n';
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int tests;
  cin >> tests;
  while (tests--) {
    solve_one();
  }
  return 0;
}

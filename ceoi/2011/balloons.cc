/*
 * Solution for CEOI 2011 Day 1: Balloons.
 *
 * Inflate balloons left to right. Maintain a stack of visible previous balloons
 * and cap the radius when a smaller balloon on the stack blocks expansion.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

using Balloon = pair<long double, long double>;

long double inflate_balloon(long double x, long double r, vector<Balloon>& stack) {
  while (!stack.empty()) {
    auto [x1, r1] = stack.back();
    long double bound = (x1 - x) * (x1 - x) / (4.0L * r1);
    r = min(r, bound);
    if (r > r1) {
      stack.pop_back();
    } else {
      break;
    }
  }
  stack.push_back({x, r});
  return r;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << fixed << setprecision(3);

  int n;
  cin >> n;
  vector<Balloon> stack;
  stack.reserve(n);
  for (int i = 0; i < n; i++) {
    long double x;
    long double r;
    cin >> x >> r;
    cout << inflate_balloon(x, r, stack) << '\n';
  }
  return 0;
}

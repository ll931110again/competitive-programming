/*
 * Solution for BOI 2011 Day 1: Ice Cream.
 *
 * Brute-force all flavor triples and reject those containing a forbidden pair.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_flavors = 205;

bool forbidden[k_max_flavors][k_max_flavors];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int flavor_count;
  int pair_count;
  cin >> flavor_count >> pair_count;
  for (int i = 0; i < pair_count; i++) {
    int first;
    int second;
    cin >> first >> second;
    forbidden[first][second] = forbidden[second][first] = true;
  }

  long long answer = 0;
  for (int first = 1; first <= flavor_count; first++) {
    for (int second = first + 1; second <= flavor_count; second++) {
      for (int third = second + 1; third <= flavor_count; third++) {
        if (forbidden[first][second] || forbidden[first][third] || forbidden[second][third]) {
          continue;
        }
        answer++;
      }
    }
  }

  cout << answer << '\n';
  return 0;
}

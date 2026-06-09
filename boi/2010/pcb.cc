/*
 * Solution for BOI 2010 Day 1: Printed Circuit Board.
 *
 * Sort components by the first endpoint, then take a longest decreasing subsequence
 * on the second endpoint.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int component_count;
  cin >> component_count;
  vector<pair<int, int>> components(component_count);
  for (int i = 0; i < component_count; i++) {
    cin >> components[i].first >> components[i].second;
  }
  sort(components.begin(), components.end());

  multiset<int, greater<int>> tails;
  for (const auto& component : components) {
    auto it = tails.upper_bound(component.second);
    if (it != tails.end()) {
      tails.erase(it);
    }
    tails.insert(component.second);
  }

  cout << tails.size() << '\n';
  return 0;
}

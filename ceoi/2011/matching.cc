/*
 * Solution for CEOI 2011 Day 1: Matching.
 *
 * Build the rank pattern from the logo permutation, then run order-isomorphic KMP
 * with a failure function based on nearest smaller and greater neighbors.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

int n, m;
vector<int> pattern;
vector<int> heights;
vector<int> greater_neighbor;
vector<int> lesser_neighbor;
vector<int> fail;

bool isomorphic_prefix(int len, int text_end) {
  if (len == 0) {
    return true;
  }
  int u = greater_neighbor[len];
  int w = lesser_neighbor[len];
  if (u != 0) {
    bool pattern_less = pattern[u] < pattern[len];
    bool text_less = heights[text_end - len + u] < heights[text_end - len + len];
    if (pattern_less != text_less) {
      return false;
    }
  }
  if (w != 0) {
    bool pattern_less = pattern[w] < pattern[len];
    bool text_less = heights[text_end - len + w] < heights[text_end - len + len];
    if (pattern_less != text_less) {
      return false;
    }
  }
  return true;
}

bool isomorphic_suffix(int len, int suffix_start) {
  if (len == 0) {
    return true;
  }
  int u = greater_neighbor[len];
  int w = lesser_neighbor[len];
  if (u != 0) {
    bool left_less = pattern[u] < pattern[len];
    bool right_less = pattern[suffix_start + u - 1] < pattern[suffix_start + len - 1];
    if (left_less != right_less) {
      return false;
    }
  }
  if (w != 0) {
    bool left_less = pattern[w] < pattern[len];
    bool right_less = pattern[suffix_start + w - 1] < pattern[suffix_start + len - 1];
    if (left_less != right_less) {
      return false;
    }
  }
  return true;
}

void build_neighbors() {
  vector<int> previous_value(n + 2);
  vector<int> next_value(n + 2);
  for (int value = 0; value <= n + 1; value++) {
    previous_value[value] = value - 1;
    next_value[value] = value + 1;
  }
  previous_value[0] = n;
  next_value[0] = 1;
  next_value[n] = 0;

  vector<int> value_position(n + 1);
  for (int i = 1; i <= n; i++) {
    value_position[pattern[i]] = i;
  }

  greater_neighbor.assign(n + 1, 0);
  lesser_neighbor.assign(n + 1, 0);
  for (int i = n; i >= 1; i--) {
    int value = pattern[i];
    int lower_value = previous_value[value];
    int upper_value = next_value[value];
    if (lower_value != 0) {
      greater_neighbor[i] = value_position[lower_value];
    }
    if (upper_value != 0) {
      lesser_neighbor[i] = value_position[upper_value];
    }
    next_value[previous_value[value]] = next_value[value];
    previous_value[next_value[value]] = previous_value[value];
  }
}

void build_failure() {
  fail.assign(n + 1, 0);
  for (int i = 2; i <= n; i++) {
    int border = fail[i - 1];
    while (border > 0 && !isomorphic_suffix(border + 1, i - border)) {
      border = fail[border];
    }
    if (isomorphic_suffix(border + 1, i - border)) {
      fail[i] = border + 1;
    }
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  pattern.assign(n + 1, 0);
  vector<int> permutation(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> permutation[i];
    pattern[permutation[i]] = i;
  }

  heights.assign(m + 1, 0);
  for (int i = 1; i <= m; i++) {
    cin >> heights[i];
  }

  build_neighbors();
  build_failure();

  vector<int> matches;
  int matched = 0;
  for (int i = 1; i <= m; i++) {
    while (matched > 0 && !isomorphic_prefix(matched + 1, i)) {
      matched = fail[matched];
    }
    if (isomorphic_prefix(matched + 1, i)) {
      matched++;
    }
    if (matched == n) {
      matches.push_back(i - n + 1);
      matched = fail[matched];
    }
  }

  cout << matches.size() << '\n';
  for (int position : matches) {
    cout << position << '\n';
  }
  return 0;
}

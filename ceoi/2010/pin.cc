/*
 * Solution for CEOI 2010 Day 2: PIN.
 *
 * For each mask of matching positions, bucket PINs by those characters and
 * count pairs. Recover the answer for exactly D differences by inclusion-exclusion.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

int n, target;
vector<array<int, 4>> pins;

long long count_pairs_matching_mask(int mask) {
  unordered_map<string, int> freq;
  freq.reserve(n * 2);
  for (const auto& pin : pins) {
    string key;
    key.reserve(4);
    for (int pos = 0; pos < 4; pos++) {
      if (mask & (1 << pos)) {
        key.push_back(static_cast<char>(pin[pos]));
      }
    }
    freq[key]++;
  }

  long long total = 0;
  for (const auto& [_, cnt] : freq) {
    total += 1LL * cnt * (cnt - 1) / 2;
  }
  return total;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> target;
  pins.resize(n);
  for (int i = 0; i < n; i++) {
    string s;
    cin >> s;
    for (int j = 0; j < 4; j++) {
      pins[i][j] = s[j];
    }
  }

  long long match_count[16];
  for (int mask = 0; mask < 16; mask++) {
    match_count[mask] = count_pairs_matching_mask(mask);
  }

  long long exact[16];
  memcpy(exact, match_count, sizeof exact);
  for (int mask = 0; mask < 16; mask++) {
    for (int superset = 0; superset < 16; superset++) {
      if (mask == superset || (superset & mask) != mask) {
        continue;
      }
      int diff = __builtin_popcount(superset) - __builtin_popcount(mask);
      if (diff & 1) {
        exact[mask] -= exact[superset];
      } else {
        exact[mask] += exact[superset];
      }
    }
  }

  long long answer = 0;
  for (int mask = 0; mask < 16; mask++) {
    if (__builtin_popcount(mask) == 4 - target) {
      answer += exact[mask];
    }
  }

  cout << answer << '\n';
  return 0;
}

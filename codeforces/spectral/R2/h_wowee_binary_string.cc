// Codeforces Spectral::Cup 2026 Round 2 — Wowee Binary String
// https://codeforces.com/contest/2229/problem/H
//
// Sketch
// ------
// After replacing '?', a target t can be obtained from s iff t is a subsequence
// whose gaps (the deleted pieces between consecutive kept characters, plus the
// suffix after the last keep) each contain an even number of 1s. For a fully
// specified s the leftmost greedy matching is correct: if some later matching
// works, the interval between two even-1 gaps is itself even-1, so the earlier
// match can be substituted.
//
// '?' characters break that greedy because they also choose the running parity
// of 1s. Track both parities at once: for the characters matched so far, store
// the pair (earliest even-1-prefix match, earliest odd-1-prefix match), where
// prefix parity counts only the fixed '1's. A '?' in a gap can repair the
// wrong parity, so a gap is blocked only while the 1-count since the previous
// keep is odd and no '?' has been seen.
//
// Precompute, for every previous keep i, the earliest later match of bit b
// with prefix parity p. DP over greedy pairs (even, odd): appending a 0 or a 1
// updates the pair in O(1). A pair is a completed string iff at least one of
// its two positions has a deletable suffix. A dummy leading 0 starts the empty
// string at pair (0, n).
//
// Complexity: O(n^2) per test, and the sum of n is 3000.

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int kMod = 998244353;

struct ModInt {
  int value = 0;

  ModInt() = default;
  explicit ModInt(int64_t x) {
    x %= kMod;
    if (x < 0) {
      x += kMod;
    }
    value = static_cast<int>(x);
  }

  ModInt& operator+=(ModInt other) {
    value += other.value;
    if (value >= kMod) {
      value -= kMod;
    }
    return *this;
  }
};

int count_strings(string s) {
  s = '0' + s;
  const int n = static_cast<int>(s.size());

  vector<int> ones_prefix(n);
  ones_prefix[0] = (s[0] == '1');
  for (int i = 1; i < n; i++) {
    ones_prefix[i] = ones_prefix[i - 1] + (s[i] == '1');
  }

  // next_match[i][bit][parity] = earliest j > i matching `bit` whose fixed-1
  // prefix has this parity, with an even-1 (or '?'-repairable) gap after i.
  vector<array<array<int, 2>, 2>> next_match(n + 1);
  for (int i = 0; i <= n; i++) {
    next_match[i][0][0] = n;
    next_match[i][0][1] = n;
    next_match[i][1][0] = n;
    next_match[i][1][1] = n;
    int ones = 0;
    bool seen_wildcard = false;
    for (int j = i + 1; j < n; j++) {
      if ((ones & 1) && !seen_wildcard) {
        ones += (s[j] == '1');
        seen_wildcard |= (s[j] == '?');
        continue;
      }
      ones += (s[j] == '1');
      seen_wildcard |= (s[j] == '?');
      const int parity = ones_prefix[j] & 1;
      if (s[j] == '?') {
        next_match[i][0][parity] = min(next_match[i][0][parity], j);
        next_match[i][1][parity] = min(next_match[i][1][parity], j);
        continue;
      }
      const int bit = s[j] - '0';
      next_match[i][bit][parity] = min(next_match[i][bit][parity], j);
    }
  }

  vector<char> suffix_deletable(n + 1);
  int ones = 0;
  bool seen_wildcard = false;
  for (int i = n - 1; i >= 0; i--) {
    suffix_deletable[i] = ((ones % 2 == 0) || seen_wildcard);
    ones += (s[i] == '1');
    seen_wildcard |= (s[i] == '?');
  }

  vector<vector<ModInt>> dp(n + 1, vector<ModInt>(n + 1));
  dp[0][n] = ModInt(1);

  vector<array<int, 2>> order;
  order.reserve(static_cast<size_t>(n + 1) * (n + 1));
  for (int i = 0; i <= n; i++) {
    for (int j = i; j <= n; j++) {
      order.push_back({i, j});
      if (j != i) {
        order.push_back({j, i});
      }
    }
  }

  ModInt ans;
  for (const array<int, 2>& pos : order) {
    const int even = pos[0];
    const int odd = pos[1];
    for (int bit = 0; bit < 2; bit++) {
      const int next_even = min(next_match[even][bit][0], next_match[odd][bit][0]);
      const int next_odd = min(next_match[even][bit][1], next_match[odd][bit][1]);
      dp[next_even][next_odd] += dp[even][odd];
    }
    if (suffix_deletable[even] || suffix_deletable[odd]) {
      ans += dp[even][odd];
    }
  }
  return ans.value;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int test_count;
  cin >> test_count;
  while (test_count--) {
    int n;
    string s;
    cin >> n >> s;
    cout << count_strings(std::move(s)) << '\n';
  }
  return 0;
}

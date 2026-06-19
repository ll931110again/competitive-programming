/*
 * Solution for CEOI 2011 Day 0: Similarity.
 *
 * For each text position, count how many valid alignments match that letter
 * using a sliding-window frequency array over the pattern.
 */

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

string pattern;
string text;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> pattern >> text;
  int n = static_cast<int>(pattern.size());
  int m = static_cast<int>(text.size());

  vector<int> counts(26);
  i64 similarity = 0;
  for (int i = 0; i < m; i++) {
    if (i < n) {
      counts[pattern[i] - 'a']++;
    }
    similarity += counts[text[i] - 'a'];
    if (i >= m - n) {
      counts[pattern[i - (m - n)] - 'a']--;
    }
  }

  cout << similarity << '\n';
  return 0;
}

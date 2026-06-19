/*
 * Solution for BOI 2011 Day 1: Plagiarism.
 *
 * Sort file sizes and count pairs with 10 * min >= 9 * max.
 */

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int file_count;
  cin >> file_count;
  vector<int> sizes(file_count);
  for (int i = 0; i < file_count; i++) {
    cin >> sizes[i];
  }
  sort(sizes.begin(), sizes.end());

  i64 pairs = 0;
  for (int left = 0, right = 0; left < file_count; left++) {
    while (right < file_count && 10LL * sizes[right] < 9LL * sizes[left]) {
      right++;
    }
    pairs += left - right;
  }

  cout << pairs << '\n';
  return 0;
}

/*
 * Solution for BOI 2010 Day 2: Matching Bins.
 *
 * Maintain surplus counts D(x) while shrinking k. The leftmost k bins fit into the
 * next k bins iff all D(x) stay non-negative.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_bins = 20005;
constexpr int k_max_size = 1005;

int bin_count, max_size;
int sizes[k_max_bins];
int left_count[k_max_size];
int right_count[k_max_size];
int surplus[k_max_size];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> max_size >> bin_count;
  for (int i = 0; i < bin_count; i++) {
    cin >> sizes[i];
  }

  int k = bin_count / 2;
  int right_end = 2 * k;
  fill(left_count, left_count + max_size + 2, 0);
  fill(right_count, right_count + max_size + 2, 0);
  for (int i = 0; i < k; i++) {
    left_count[sizes[i]]++;
  }
  for (int i = k; i < right_end; i++) {
    right_count[sizes[i]]++;
  }

  surplus[max_size + 1] = 0;
  for (int size = max_size; size >= 1; size--) {
    surplus[size] = surplus[size + 1] + right_count[size + 1] - left_count[size];
  }

  while (k > 0) {
    int size = max_size;
    while (size > 0) {
      if (surplus[size] < 0) {
        right_count[sizes[right_end - 1]]--;
        right_count[sizes[right_end - 2]]--;
        left_count[sizes[k - 1]]--;
        right_count[sizes[k]]++;
        size = max_size;
        if (sizes[right_end - 1] > size) {
          size = sizes[right_end - 1];
        }
        if (sizes[right_end - 2] > size) {
          size = sizes[right_end - 2];
        }
        if (sizes[k - 1] > size) {
          size = sizes[k - 1];
        }
        k--;
        right_end -= 2;
        for (int value = size; value >= 1; value--) {
          surplus[value] = surplus[value + 1] + right_count[value + 1] - left_count[value];
        }
        break;
      }
      size--;
    }
    if (size == 0) {
      break;
    }
  }

  cout << k << '\n';
  return 0;
}

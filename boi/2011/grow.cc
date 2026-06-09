/*
 * Solution for BOI 2011 Day 1: Growing Trees.
 *
 * Keep heights sorted by (height, index) in a Fenwick tree of differences. A
 * fertilize operation raises a height band and the rightmost trees at the top
 * band boundary.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_trees = 100005;

int tree_count;
long long fenwick[k_max_trees];

void fenwick_add(int index, long long value) {
  for (; index <= tree_count; index += index & -index) {
    fenwick[index] += value;
  }
}

long long fenwick_prefix(int index) {
  long long sum = 0;
  for (; index > 0; index -= index & -index) {
    sum += fenwick[index];
  }
  return sum;
}

void fenwick_range_add(int left, int right, long long value) {
  if (left > right) {
    return;
  }
  right = min(right, tree_count);
  fenwick_add(left, value);
  fenwick_add(right + 1, -value);
}

long long height_at(int index) {
  return fenwick_prefix(index);
}

int first_at_least(long long value) {
  int low = 1;
  int high = tree_count + 1;
  while (low < high) {
    int mid = (low + high) / 2;
    if (fenwick_prefix(mid) >= value) {
      high = mid;
    } else {
      low = mid + 1;
    }
  }
  return low;
}

void fertilize(int count, long long min_height) {
  int left = first_at_least(min_height);
  int right = min(tree_count, left + count - 1);
  if (left > tree_count) {
    return;
  }
  count = right - left + 1;
  long long top_height = height_at(right);
  int band_start = first_at_least(top_height);
  fenwick_range_add(left, band_start - 1, 1);
  int remaining = count - (band_start - left);
  if (remaining > 0) {
    int band_end = first_at_least(top_height + 1) - 1;
    int pointer = band_end - remaining + 1;
    fenwick_range_add(pointer, band_end, 1);
  }
}

int count_range(long long low, long long high) {
  return first_at_least(high + 1) - first_at_least(low);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int query_count;
  cin >> tree_count >> query_count;

  vector<pair<long long, int>> trees(tree_count);
  for (int i = 0; i < tree_count; i++) {
    cin >> trees[i].first;
    trees[i].second = i;
  }
  sort(trees.begin(), trees.end());

  for (int i = 1; i <= tree_count; i++) {
    long long diff = trees[i - 1].first - (i == 1 ? 0 : trees[i - 2].first);
    fenwick_add(i, diff);
  }

  while (query_count--) {
    char type;
    cin >> type;
    if (type == 'F') {
      int count;
      long long min_height;
      cin >> count >> min_height;
      fertilize(count, min_height);
    } else {
      long long low;
      long long high;
      cin >> low >> high;
      cout << count_range(low, high) << '\n';
    }
  }
  return 0;
}

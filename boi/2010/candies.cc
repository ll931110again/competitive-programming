/*
 * Solution for BOI 2010 Day 2: Candies.
 *
 * Divide-and-conquer knapsack to pick the removed box, then add the smallest
 * new size that doubles the reachable order set.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_packages = 100;
constexpr int k_max_candies = 7000;

int package_count;
int package_sizes[k_max_packages];
int best_orders;
int removed_size;

int count_reachable(const char* reachable, int size) {
  int count = 0;
  for (int sum = 0; sum <= size; sum++) {
    count += reachable[sum] != 0;
  }
  return count;
}

void choose_removed_recursive(int* boxes, int box_count, char* reachable, int max_sum) {
  if (box_count == 1) {
    int served = count_reachable(reachable, max_sum);
    if (served > best_orders || (served == best_orders && removed_size > boxes[0])) {
      best_orders = served;
      removed_size = boxes[0];
    }
    return;
  }

  char partial[k_max_packages * k_max_candies + 1];
  pair<int, int> halves[2] = {{0, box_count / 2 - 1}, {box_count / 2, box_count - 1}};
  for (int half = 0; half < 2; half++) {
    fill(partial, partial + k_max_packages * k_max_candies + 1, 0);
    for (int sum = 0; sum <= max_sum; sum++) {
      partial[sum] = reachable[sum];
    }
    int current_max = max_sum;
    for (int index = halves[half].first; index <= halves[half].second; index++) {
      current_max += boxes[index];
      for (int sum = current_max; sum >= boxes[index]; sum--) {
        partial[sum] |= partial[sum - boxes[index]];
      }
    }
    choose_removed_recursive(boxes + halves[half ^ 1].first,
                             halves[half ^ 1].second - halves[half ^ 1].first + 1, partial,
                             current_max);
  }
}

int choose_removed_package() {
  best_orders = -1;
  removed_size = -1;
  char reachable[k_max_packages * k_max_candies + 1];
  reachable[0] = 1;
  choose_removed_recursive(package_sizes, package_count, reachable, 1);
  return removed_size;
}

int choose_added_package() {
  int limit = k_max_candies * package_count + 3;
  int shift = limit;
  vector<char> reachable(2 * limit + 10);
  reachable[shift] = 1;

  for (int i = 0; i < package_count; i++) {
    for (int sum = limit; sum >= package_sizes[i] - limit; sum--) {
      reachable[sum + shift] |= reachable[sum - package_sizes[i] + shift];
    }
    for (int sum = -limit; sum <= limit - package_sizes[i]; sum++) {
      reachable[sum + shift] |= reachable[sum + package_sizes[i] + shift];
    }
  }

  for (int added = 0; added <= limit; added++) {
    if (!reachable[added + shift]) {
      return added;
    }
  }
  return -1;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> package_count;
  for (int i = 0; i < package_count; i++) {
    cin >> package_sizes[i];
  }

  int removed = choose_removed_package();
  int write_index = 0;
  for (int i = 0; i < package_count; i++) {
    if (package_sizes[i] != removed) {
      package_sizes[write_index++] = package_sizes[i];
    }
  }
  package_count = write_index;

  cout << removed << ' ' << choose_added_package() << '\n';
  return 0;
}

/*
 * Solution for BOI 2010 Day 1: BEARs.
 *
 * Binary search the largest safe distance. Expand the reachable rectangle by
 * absorbing any street that intersects the current forbidden square.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Street {
  int x1, y1, x2, y2;

  int min_x() const {
    return min(x1, x2);
  }
  int max_x() const {
    return max(x1, x2);
  }
  int min_y() const {
    return min(y1, y2);
  }
  int max_y() const {
    return max(y1, y2);
  }
};

int warehouse_x, warehouse_y;
int street_count;
Street streets[501];

bool inside_open_square(int left, int right, int bottom, int top, int x, int y) {
  return left < x && x < right && bottom < y && y < top;
}

bool can_reach(int distance) {
  int left = -distance;
  int right = distance;
  int bottom = -distance;
  int top = distance;

  bool changed = true;
  while (changed && !inside_open_square(left, right, bottom, top, warehouse_x, warehouse_y)) {
    changed = false;
    for (int i = 0; i < street_count; i++) {
      bool fully_inside =
          inside_open_square(left, right, bottom, top, streets[i].x1, streets[i].y1) &&
          inside_open_square(left, right, bottom, top, streets[i].x2, streets[i].y2);
      if (fully_inside) {
        continue;
      }
      bool fully_outside = streets[i].max_x() <= left || streets[i].min_x() >= right ||
                           streets[i].max_y() <= bottom || streets[i].min_y() >= top;
      if (fully_outside) {
        continue;
      }
      left = min(left, streets[i].min_x() - 1);
      right = max(right, streets[i].max_x() + 1);
      bottom = min(bottom, streets[i].min_y() - 1);
      top = max(top, streets[i].max_y() + 1);
      changed = true;
    }
  }
  return inside_open_square(left, right, bottom, top, warehouse_x, warehouse_y);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> warehouse_x >> warehouse_y >> street_count;
  for (int i = 0; i < street_count; i++) {
    cin >> streets[i].x1 >> streets[i].y1 >> streets[i].x2 >> streets[i].y2;
  }

  int low = 0;
  int high = max(abs(warehouse_x), abs(warehouse_y)) + 1;
  while (high - low > 1) {
    int mid = (low + high) / 2;
    if (can_reach(mid)) {
      high = mid;
    } else {
      low = mid;
    }
  }
  cout << low << '\n';
  return 0;
}

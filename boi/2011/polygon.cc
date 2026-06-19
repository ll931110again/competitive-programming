/*
 * Solution for BOI 2011 Day 2: Polygon.
 *
 * Twice the interior grid length equals 2 * area minus horizontal and vertical
 * boundary contributions.
 */

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int vertex_count;
  cin >> vertex_count;
  vector<i64> x(vertex_count + 1);
  vector<i64> y(vertex_count + 1);
  for (int i = 1; i <= vertex_count; i++) {
    cin >> x[i] >> y[i];
  }
  x[0] = x[vertex_count];
  y[0] = y[vertex_count];

  i64 twice_area = 0;
  i64 horizontal_boundary = 0;
  i64 vertical_boundary = 0;
  for (int i = 1; i <= vertex_count; i++) {
    twice_area += x[i] * y[i - 1] - x[i - 1] * y[i];
    if (y[i] == y[i - 1]) {
      horizontal_boundary += abs(x[i] - x[i - 1]);
    }
    if (x[i] == x[i - 1]) {
      vertical_boundary += abs(y[i] - y[i - 1]);
    }
  }
  twice_area = abs(twice_area);

  i64 twice_interior = 2 * twice_area - horizontal_boundary - vertical_boundary;
  cout << twice_interior / 2 << '.' << setfill('0') << setw(3) << (twice_interior % 2) * 500
       << '\n';
  return 0;
}

/*
 * Solution for BOI 2011 Day 2: Polygon.
 *
 * Twice the interior grid length equals 2 * area minus horizontal and vertical
 * boundary contributions.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int vertex_count;
  cin >> vertex_count;
  vector<long long> x(vertex_count + 1);
  vector<long long> y(vertex_count + 1);
  for (int i = 1; i <= vertex_count; i++) {
    cin >> x[i] >> y[i];
  }
  x[0] = x[vertex_count];
  y[0] = y[vertex_count];

  long long twice_area = 0;
  long long horizontal_boundary = 0;
  long long vertical_boundary = 0;
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

  long long twice_interior = 2 * twice_area - horizontal_boundary - vertical_boundary;
  cout << twice_interior / 2 << '.' << setfill('0') << setw(3) << (twice_interior % 2) * 500
       << '\n';
  return 0;
}

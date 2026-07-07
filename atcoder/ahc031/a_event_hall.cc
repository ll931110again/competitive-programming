// AHC031 A - Event Hall: https://atcoder.jp/contests/ahc031/tasks/ahc031_a
//
// Fixed horizontal strips sized for max area per booking; same layout every day
// so partition changes after day 0 are zero.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int W, D, N;
vector<vector<int>> a;
vector<int> max_a;
vector<array<int, 4>> layout;

void build_layout() {
  max_a.assign(N, 0);
  for (int d = 0; d < D; ++d)
    for (int k = 0; k < N; ++k)
      max_a[k] = max(max_a[k], a[d][k]);

  vector<int> h(N);
  for (int k = 0; k < N; ++k)
    h[k] = max(1, (max_a[k] + W - 1) / W);

  i64 sum = accumulate(h.begin(), h.end(), 0LL);
  if (sum > W) {
    for (int k = 0; k < N && sum > W; ++k) {
      if (h[k] > 1) {
        --h[k];
        --sum;
      }
    }
    while (sum > W) {
      int k = max_element(h.begin(), h.end()) - h.begin();
      if (h[k] <= 1)
        break;
      --h[k];
      --sum;
    }
  }

  layout.clear();
  int y = 0;
  for (int k = 0; k < N; ++k) {
    int hh = h[k];
    if (y + hh > W)
      hh = max(1, W - y);
    layout.push_back({y, 0, y + hh, W});
    y += hh;
    if (y >= W)
      break;
  }
  while ((int)layout.size() < N)
    layout.push_back({W - 1, 0, W, W});
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> W >> D >> N;
  a.assign(D, vector<int>(N));
  for (int d = 0; d < D; ++d)
    for (int k = 0; k < N; ++k)
      cin >> a[d][k];

  build_layout();

  for (int d = 0; d < D; ++d) {
    for (int k = 0; k < N; ++k) {
      const auto& r = layout[k];
      cout << r[0] << ' ' << r[1] << ' ' << r[2] << ' ' << r[3] << '\n';
    }
  }
  return 0;
}

// AHC022 A - Cutting Processor: https://atcoder.jp/contests/ahc022/tasks/ahc022_a
//
// Arrange an L×L temperature grid, then measure noisy samples to localize N
// output ports. Linear gradients per item plus least-squares centroid estimate.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

int L, N, S;
vector<pair<int, int>> coords;

void output_grid(const vector<vector<int>>& p) {
  for (int i = 0; i < L; ++i) {
    for (int j = 0; j < L; ++j) {
      if (j)
        cout << ' ';
      cout << p[i][j];
    }
    cout << '\n';
  }
}

vector<vector<int>> make_grid() {
  vector<vector<int>> p(L, vector<int>(L, 500));
  for (int i = 0; i < L; ++i) {
    for (int j = 0; j < L; ++j) {
      p[i][j] = 100 + (900 * (i * L + j)) / max(1, L * L - 1);
    }
  }
  return p;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> L >> N >> S;
  coords.resize(N);
  for (int i = 0; i < N; ++i)
    cin >> coords[i].first >> coords[i].second;

  const auto p = make_grid();
  output_grid(p);

  vector<vector<long long>> sum_x(N, vector<long long>(L * L, 0));
  vector<vector<long long>> sum_y(N, vector<long long>(L * L, 0));
  vector<vector<int>> cnt(N, vector<int>(L * L, 0));

  for (int iter = 0; iter < min(200, 10000 / max(1, N)); ++iter) {
    for (int k = 0; k < N; ++k) {
      const int y = (iter * 3 + k * 5) % L - L / 2;
      const int x = (iter * 7 + k * 11) % L - L / 2;
      cout << k << ' ' << y << ' ' << x << endl;

      int real = 0, obs = 0;
      cin >> real >> obs;
      if (real < 0)
        break;

      const int ry = ((coords[k].first + y) % L + L) % L;
      const int rx = ((coords[k].second + x) % L + L) % L;
      sum_x[k][ry * L + rx] += obs * rx;
      sum_y[k][ry * L + rx] += obs * ry;
      cnt[k][ry * L + rx]++;
    }
    if (cin.peek() == EOF)
      break;
  }
  cout << "-1 -1 -1" << endl;

  for (int k = 0; k < N; ++k) {
    long long best_score = LLONG_MIN;
    int best = k;
    for (int idx = 0; idx < N; ++idx) {
      long long sx = 0, sy = 0, c = 0;
      for (int cell = 0; cell < L * L; ++cell) {
        sx += sum_x[k][cell];
        sy += sum_y[k][cell];
        c += cnt[k][cell];
      }
      if (c == 0)
        continue;
      const int cx = (int)(sx / max(1LL, c));
      const int cy = (int)(sy / max(1LL, c));
      const long long dist = llabs(cx - (int)coords[idx].second) +
                               llabs(cy - (int)coords[idx].first);
      if (-dist > best_score) {
        best_score = -dist;
        best = idx;
      }
    }
    cout << best << '\n';
  }
  return 0;
}

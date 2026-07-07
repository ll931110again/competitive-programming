// AHC053 A - Random Sum Game
// https://atcoder.jp/contests/ahc053/tasks/ahc053_a
//
// Write near-uniform card values; greedily assign cards to bins after B is revealed.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr i64 kMaxA = 1002000000000000LL;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  i64 l, u;
  cin >> n >> m >> l >> u;

  const i64 base = (l + u) / 2;
  vector<i64> a(n, base);
  for (int i = 0; i < n; ++i)
    a[i] = min<i64>(u, max<i64>(l, base + (i % 7) - 3));

  for (int i = 0; i < n; ++i) {
    if (i)
      cout << ' ';
    cout << a[i];
  }
  cout << '\n' << flush;

  vector<i64> b(m);
  for (int j = 0; j < m; ++j)
    cin >> b[j];

  vector<i64> sum(m, 0);
  vector<int> x(n, 0);
  vector<int> order(n);
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(),
       [&](int p, int q) { return a[p] > a[q]; });

  for (int idx : order) {
    int best = 0;
    i64 best_err = (1LL << 62);
    for (int j = 0; j < m; ++j) {
      const i64 err = llabs(sum[j] + a[idx] - b[j]);
      if (err < best_err) {
        best_err = err;
        best = j + 1;
      }
    }
    x[idx] = best;
    sum[best - 1] += a[idx];
  }

  for (int i = 0; i < n; ++i) {
    if (i)
      cout << ' ';
    cout << x[i];
  }
  cout << '\n';
  return 0;
}

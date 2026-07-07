// AHC032 A - Mod Stamp: https://atcoder.jp/contests/ahc032/tasks/ahc032_a
//
// Greedy hill-climb: each step applies the stamp placement that maximizes
// the board sum mod 998244353.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr i64 MOD = 998244353;

int N, M, K;
vector<vector<i64>> a;
vector<vector<vector<i64>>> s;

i64 board_sum() {
  i64 t = 0;
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      t += a[i][j] % MOD;
  return t;
}

void apply(int m, int p, int q) {
  for (int x = 0; x < 3; ++x)
    for (int y = 0; y < 3; ++y)
      a[p + x][q + y] += s[m][x][y];
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> M >> K;
  a.assign(N, vector<i64>(N));
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      cin >> a[i][j];

  s.assign(M, vector<vector<i64>>(3, vector<i64>(3)));
  for (int m = 0; m < M; ++m)
    for (int x = 0; x < 3; ++x)
      for (int y = 0; y < 3; ++y)
        cin >> s[m][x][y];

  vector<array<int, 3>> ops;
  for (int step = 0; step < K; ++step) {
    i64 best = board_sum();
    int bm = -1, bp = -1, bq = -1;
    for (int m = 0; m < M; ++m) {
      for (int p = 0; p <= N - 3; ++p) {
        for (int q = 0; q <= N - 3; ++q) {
          apply(m, p, q);
          const i64 cur = board_sum();
          if (cur > best) {
            best = cur;
            bm = m;
            bp = p;
            bq = q;
          }
          apply(m, p, q);
          for (int x = 0; x < 3; ++x)
            for (int y = 0; y < 3; ++y)
              a[p + x][q + y] -= s[m][x][y];
        }
      }
    }
    if (bm < 0)
      break;
    apply(bm, bp, bq);
    ops.push_back({bm, bp, bq});
  }

  cout << ops.size() << '\n';
  for (const auto [m, p, q] : ops)
    cout << m << ' ' << p << ' ' << q << '\n';
  return 0;
}

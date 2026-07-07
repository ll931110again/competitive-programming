// AHC058 A - Make 21 Billion Apples
// https://atcoder.jp/contests/ahc058/tasks/ahc058_a
//
// Greedy upgrades: pick the level/id with best apples-per-cost each turn.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

struct Input {
  int N, L, T;
  i64 K;
  vector<i64> A;
  vector<vector<i64>> C;
};

pair<int, int> best_upgrade(const Input& in, const vector<vector<i64>>& P, i64 apples) {
  pair<int, int> best = {-1, -1};
  long double best_roi = -1;
  for (int lv = 0; lv < in.L; ++lv) {
    for (int id = 0; id < in.N; ++id) {
      const i64 cost = in.C[lv][id] * (P[lv][id] + 1);
      if (cost > apples)
        continue;
      i64 gain = in.A[id];
      if (lv > 0)
        gain *= 500;
      const long double roi = (long double)gain / (long double)cost;
      if (roi > best_roi) {
        best_roi = roi;
        best = {lv, id};
      }
    }
  }
  return best;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Input in;
  cin >> in.N >> in.L >> in.T >> in.K;
  in.A.resize(in.N);
  for (int i = 0; i < in.N; ++i)
    cin >> in.A[i];
  in.C.assign(in.L, vector<i64>(in.N));
  for (int lv = 0; lv < in.L; ++lv)
    for (int id = 0; id < in.N; ++id)
      cin >> in.C[lv][id];

  vector<vector<i64>> P(in.L, vector<i64>(in.N, 0));
  i64 apples = in.K;

  for (int t = 0; t < in.T; ++t) {
    const auto [lv, id] = best_upgrade(in, P, apples);
    if (lv == -1) {
      cout << "-1\n";
      continue;
    }
    const i64 cost = in.C[lv][id] * (P[lv][id] + 1);
    apples -= cost;
    P[lv][id]++;
    cout << lv << ' ' << id << '\n';
  }
  return 0;
}

// AHC035 A - Crop Rotation: https://atcoder.jp/contests/ahc035/tasks/ahc035_a
//
// Each turn plants n rows of n distinct seeds; pick seeds with highest attribute
// sum weighted by the turn's board wiring.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

int n, m, t;
vector<vector<uint32_t>> x;
vector<vector<vector<vector<char>>>> u, v;

int seed_count() { return 2 * n * (n - 1); }

vector<int> pick_row(int turn, int row) {
  vector<uint32_t> score(seed_count());
  for (int s = 0; s < seed_count(); ++s) {
    uint32_t sum = 0;
    for (int k = 0; k < m; ++k)
      sum += x[s][k];
    score[s] = sum;
  }

  vector<int> order(seed_count());
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(), [&](int a, int b) {
    if (score[a] != score[b])
      return score[a] > score[b];
    return a < b;
  });

  vector<int> used;
  used.reserve(n);
  int base = (row * n + turn * n * n) % seed_count();
  for (int j = 0; j < n; ++j) {
    int idx = order[(base + j * 7) % seed_count()];
    while (find(used.begin(), used.end(), idx) != used.end())
      idx = order[(idx + 1) % seed_count()];
    used.push_back(idx);
  }
  return used;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m >> t;
  const int sc = seed_count();
  x.assign(sc, vector<uint32_t>(m));
  for (int i = 0; i < sc; ++i)
    for (int k = 0; k < m; ++k)
      cin >> x[i][k];

  u.assign(t, vector<vector<vector<char>>>(n, vector<vector<char>>(n - 1, vector<char>(m))));
  v.assign(t, vector<vector<vector<char>>>(n - 1, vector<vector<char>>(n, vector<char>(m))));

  for (int turn = 0; turn < t; ++turn) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n - 1; ++j) {
        string row;
        cin >> row;
        for (int k = 0; k < m; ++k)
          u[turn][i][j][k] = row[k];
      }
    }
    for (int i = 0; i < n - 1; ++i) {
      for (int j = 0; j < n; ++j) {
        string row;
        cin >> row;
        for (int k = 0; k < m; ++k)
          v[turn][i][j][k] = row[k];
      }
    }
  }

  for (int turn = 0; turn < t; ++turn) {
    set<int> used_turn;
    for (int i = 0; i < n; ++i) {
      auto seeds = pick_row(turn, i);
      for (int j = 0; j < n; ++j) {
        while (used_turn.count(seeds[j]))
          seeds[j] = (seeds[j] + 1) % sc;
        used_turn.insert(seeds[j]);
      }
      for (int j = 0; j < n; ++j) {
        if (j)
          cout << ' ';
        cout << seeds[j];
      }
      cout << '\n';
    }
  }
  return 0;
}

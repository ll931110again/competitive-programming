// AHC059 A - Sorting Robot
// https://atcoder.jp/contests/ahc059/tasks/ahc059_a
//
// Visit each value pair: take from first cell, take from second (auto-match).

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int kDr[4] = {-1, 1, 0, 0};
constexpr int kDc[4] = {0, 0, -1, 1};
constexpr char kMove[4] = {'U', 'D', 'L', 'R'};

int N;
vector<vector<int>> grid;
int cr, cc;
string out;

void emit_move(int d) {
  out.push_back(kMove[d]);
  cr += kDr[d];
  cc += kDc[d];
}

void go_to(int tr, int tc) {
  while (cr > tr)
    emit_move(0);
  while (cr < tr)
    emit_move(1);
  while (cc > tc)
    emit_move(2);
  while (cc < tc)
    emit_move(3);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N;
  grid.assign(N, vector<int>(N));
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      cin >> grid[i][j];

  cr = cc = 0;
  map<int, vector<pair<int, int>>> pos;
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      pos[grid[i][j]].push_back({i, j});

  for (auto& [v, cells] : pos) {
    if (cells.size() != 2)
      continue;
    const auto [r1, c1] = cells[0];
    const auto [r2, c2] = cells[1];
    go_to(r1, c1);
    out.push_back('Z');
    grid[r1][c1] = -1;
    go_to(r2, c2);
    out.push_back('Z');
    grid[r2][c2] = -1;
  }

  cout << out << '\n';
  return 0;
}

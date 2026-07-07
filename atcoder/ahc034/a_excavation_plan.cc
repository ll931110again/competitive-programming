// AHC034 A - Excavation Plan: https://atcoder.jp/contests/ahc034/tasks/ahc034_a
//
// Row-major walk: load positive soil, unload on negative cells; Manhattan tour.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int N;
vector<vector<i64>> h;
int cr = 0, cc = 0;
i64 carry = 0;
vector<string> ops;

void move_dir(char d) {
  if (d == 'R')
    ++cc;
  else if (d == 'L')
    --cc;
  else if (d == 'D')
    ++cr;
  else if (d == 'U')
    --cr;
  ops.emplace_back(string(1, d));
}

void load(i64 d) {
  if (d > 0) {
    carry += d;
    h[cr][cc] -= d;
    ops.push_back("+" + to_string(d));
  } else if (d < 0) {
    carry += d;
    h[cr][cc] -= d;
    ops.push_back("-" + to_string(-d));
  }
}

void visit() {
  i64 need = h[cr][cc];
  if (need > 0) {
    load(need);
  } else if (need < 0 && carry > 0) {
    const i64 d = min(carry, -need);
    load(-d);
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N;
  h.assign(N, vector<i64>(N));
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      cin >> h[i][j];

  visit();
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (i == 0 && j == 0)
        continue;
      if (j > 0)
        move_dir('R');
      else
        move_dir('D');
      visit();
    }
  }

  for (int j = N - 1; j > 0; --j) {
    move_dir('L');
    if (carry > 0 && h[cr][cc] < 0) {
      const i64 d = min(carry, -h[cr][cc]);
      load(-d);
    }
  }
  while (carry > 0) {
    int br = -1, bc = -1;
    i64 best = 0;
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        if (h[i][j] < best) {
          best = h[i][j];
          br = i;
          bc = j;
        }
    if (br < 0)
      break;
    while (cr < br)
      move_dir('D');
    while (cr > br)
      move_dir('U');
    while (cc < bc)
      move_dir('R');
    while (cc > bc)
      move_dir('L');
    const i64 d = min(carry, -h[cr][cc]);
    if (d <= 0)
      break;
    load(-d);
  }

  for (const auto& s : ops)
    cout << s << '\n';
  return 0;
}

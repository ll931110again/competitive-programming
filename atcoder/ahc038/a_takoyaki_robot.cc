// AHC038 A - Takoyaki Robot: https://atcoder.jp/contests/ahc038/tasks/ahc038_a
//
// Single-vertex robot (V=1): move root to each mismatched cell and toggle P.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

int N, M, V;
vector<vector<bool>> s, t;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> M >> V;
  s.assign(N, vector<bool>(N));
  t.assign(N, vector<bool>(N));
  string row;
  for (int i = 0; i < N; ++i) {
    cin >> row;
    for (int j = 0; j < N; ++j)
      s[i][j] = row[j] == '1';
  }
  for (int i = 0; i < N; ++i) {
    cin >> row;
    for (int j = 0; j < N; ++j)
      t[i][j] = row[j] == '1';
  }

  cout << 1 << '\n';
  cout << 0 << ' ' << 0 << '\n';

  int cr = 0, cc = 0;
  vector<string> ops;

  auto step = [&](int tr, int tc) {
    while (cr < tr) {
      ops.push_back("D.");
      ++cr;
    }
    while (cr > tr) {
      ops.push_back("U.");
      --cr;
    }
    while (cc < tc) {
      ops.push_back("R.");
      ++cc;
    }
    while (cc > tc) {
      ops.push_back("L.");
      --cc;
    }
  };

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (s[i][j] == t[i][j])
        continue;
      step(i, j);
      ops.push_back(".P");
      s[i][j] = !s[i][j];
    }
  }

  if (ops.empty())
    ops.push_back("..");
  for (const auto& line : ops)
    cout << line << '\n';
  return 0;
}

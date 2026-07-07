// AHC026 A - Warehouse: https://atcoder.jp/contests/ahc026/tasks/ahc026_a
//
// Ten stacks of boxes; output boxes 1..n in order. Recursive DFS moves blocking
// boxes to other stacks (pick lightest destination), then take from top.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

int n, m;
vector<vector<int>> stacks;
vector<pair<int, int>> ops;

int find_box(int v) {
  for (int s = 0; s < m; ++s) {
    for (int i = 0; i < (int)stacks[s].size(); ++i) {
      if (stacks[s][i] == v)
        return s;
    }
  }
  return -1;
}

int find_top(int s, int v) {
  for (int i = (int)stacks[s].size() - 1; i >= 0; --i) {
    if (stacks[s][i] == v)
      return i;
  }
  return -1;
}

void move_range(int from, int start, int dest) {
  if (from == dest)
    return;
  for (int i = start; i < (int)stacks[from].size(); ++i) {
    stacks[dest].push_back(stacks[from][i]);
    ops.emplace_back(stacks[from][i], dest + 1);
  }
  stacks[from].resize(start);
}

int best_dest(int avoid1, int avoid2) {
  int best = -1, best_sz = INT_MAX;
  for (int s = 0; s < m; ++s) {
    if (s == avoid1 || s == avoid2)
      continue;
    if ((int)stacks[s].size() < best_sz) {
      best_sz = stacks[s].size();
      best = s;
    }
  }
  return best;
}

void expose(int v) {
  const int s = find_box(v);
  const int idx = find_top(s, v);
  if (idx + 1 == (int)stacks[s].size())
    return;
  const int dest = best_dest(s, -1);
  move_range(s, idx + 1, dest);
}

void take(int v) {
  expose(v);
  const int s = find_box(v);
  stacks[s].pop_back();
  ops.emplace_back(v, 0);
}

void solve() {
  for (int v = 1; v <= n; ++v)
    take(v);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  stacks.assign(m, {});
  for (int i = 0; i < m; ++i) {
    stacks[i].resize(n / m);
    for (int j = 0; j < n / m; ++j)
      cin >> stacks[i][j];
  }

  solve();

  for (auto [v, to] : ops)
    cout << v << ' ' << to << '\n';
  return 0;
}

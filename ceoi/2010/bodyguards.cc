/*
 * Solution for CEOI 2010 Day 1: Bodyguards.
 *
 * Sort rows and columns by required counts descending. A placement exists iff
 * for every prefix of columns, the left-aligned greedy fill has enough guards.
 */

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

vector<i64> expand(int groups) {
  vector<i64> values;
  for (int i = 0; i < groups; i++) {
    i64 need, count;
    cin >> need >> count;
    values.insert(values.end(), count, need);
  }
  sort(values.begin(), values.end(), greater<i64>());
  return values;
}

bool feasible(const vector<i64>& rows, const vector<i64>& cols) {
  int n = rows.size();
  int m = cols.size();
  vector<i64> col_prefix(m + 1);
  for (int j = 0; j < m; j++) {
    col_prefix[j + 1] = col_prefix[j] + cols[j];
  }

  for (int k = 1; k <= m; k++) {
    i64 need = col_prefix[k];
    i64 have = 0;
    for (i64 row_need : rows) {
      have += min(row_need, static_cast<i64>(k));
    }
    if (have < need) {
      return false;
    }
  }
  return true;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int row_groups;
  cin >> row_groups;
  auto rows = expand(row_groups);

  int col_groups;
  cin >> col_groups;
  auto cols = expand(col_groups);

  cout << (feasible(rows, cols) ? 1 : 0) << '\n';
  return 0;
}

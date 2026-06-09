/*
 * Solution for CEOI 2010 Day 1: Bodyguards.
 *
 * Sort rows and columns by required counts descending. A placement exists iff
 * for every prefix of columns, the left-aligned greedy fill has enough guards.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

vector<long long> expand(int groups) {
  vector<long long> values;
  for (int i = 0; i < groups; i++) {
    long long need, count;
    cin >> need >> count;
    values.insert(values.end(), count, need);
  }
  sort(values.begin(), values.end(), greater<long long>());
  return values;
}

bool feasible(const vector<long long>& rows, const vector<long long>& cols) {
  int n = rows.size();
  int m = cols.size();
  vector<long long> col_prefix(m + 1);
  for (int j = 0; j < m; j++) {
    col_prefix[j + 1] = col_prefix[j] + cols[j];
  }

  for (int k = 1; k <= m; k++) {
    long long need = col_prefix[k];
    long long have = 0;
    for (long long row_need : rows) {
      have += min(row_need, static_cast<long long>(k));
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

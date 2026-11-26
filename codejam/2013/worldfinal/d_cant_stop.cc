/*
 * Solution for D: Can't Stop
 *
 * We choose a starting position i, and iterate to the right the maximum possible endpoint j.
 * At a given interval [i, j], we try to see if we can extend to j + 1, given the set of numbers we
 have chosen ops.
    * If any number in dices[j + 1] is contained in ops, we include j + 1 and proceed
    * Otherwise, we need to pick a number from dice j + 1 and recursively proceed

 * Optimization: if a number in dices[j + 1] is already included in dices[i - 1], we skip that
 (because we would had a more optimal solution choosing that number in i - 1 and extend it further).

 * Claim: the solution runs in O(N).
 */

#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

namespace {
int T;
int n, d, k;

vector<set<int>> dices;
pair<int, int> output;
} // namespace

bool cmp(pair<int, int> x, pair<int, int> y) {
  if (x.second - x.first != y.second - y.first) {
    return (x.second - x.first > y.second - y.first);
  }
  return (x.first < y.first);
}

void update(pair<int, int> candidate) {
  if (!cmp(output, candidate)) {
    output = candidate;
  }
}

void recursive(int low, int high, set<int>& forbidden, set<int> ops) {
  while (high < n) {
    bool proceed = false;
    for (auto dice : dices[high]) {
      if (ops.find(dice) != ops.end()) {
        update({low, high});
        proceed = true;
        high++;
        break;
      }
    }

    if (!proceed) {
      break;
    }
  }

  if (high >= n) {
    return;
  }

  if (ops.size() >= k) {
    return;
  }

  for (auto dice : dices[high]) {
    if (forbidden.find(dice) != forbidden.end()) {
      continue;
    }

    update({low, high});
    set<int> tmp = ops;
    tmp.insert(dice);

    recursive(low, high + 1, forbidden, tmp);
  }
}

void solve() {
  output = {0, 0};
  for (int i = 0; i < n; i++) {
    auto forbidden = (i) ? dices[i - 1] : set<int>();
    recursive(i, i, forbidden, {});
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;

  for (int tc = 1; tc <= T; tc++) {
    cin >> n >> d >> k;
    dices.resize(n);
    for (int i = 0; i < n; i++) {
      dices[i].clear();
      int x;
      for (int j = 0; j < d; j++) {
        cin >> x;
        dices[i].insert(x);
      }
    }

    solve();
    cout << "Case #" << tc << ": " << output.first << ' ' << output.second << endl;
  }
  return 0;
}

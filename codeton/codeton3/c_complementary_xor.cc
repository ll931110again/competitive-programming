// Codeforces 1750 (CodeTON Round 3 (Div. 1 + Div. 2, Rated, Prizes!)) — C. Complementary XOR
// Submission: https://codeforces.com/contest/1750/submission/334472404

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int T, n;
string a, b;

map<pair<int, int>, int> ops;
vector<pair<int, int>> outputs;

int xa[k_max_n], xb[k_max_n];

bool solve() {
  ops.clear();
  outputs.clear();
  for (int i = 0; i < n; i++) {
    xa[i] = a[i] - '0';
    xb[i] = b[i] - '0';
  }

  int xvalue = -1;
  for (int i = 0; i < n; i++) {
    int tmp = (xa[i] == xb[i]) ? 0 : 1;
    if (xvalue < 0) {
      xvalue = tmp;
    } else if (xvalue != tmp) {
      return false;
    }
  }

  if (xvalue) {
    for (int i = 0; i < n; i++) {
      xa[i] ^= 1;
    }
    ops[{1, n}] ^= 1;
  }

  for (int i = n - 1; i > 0; i--)
    if (xa[i]) {
      ops[{1, i + 1}] ^= 1;
      ops[{1, i}] ^= 1;
    }

  if (xa[0]) {
    ops[{1, n}] ^= 1;
    ops[{2, n}] ^= 1;
  }

  for (auto it : ops)
    if (it.second) {
      outputs.push_back(it.first);
    }

  return true;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    cin >> a >> b;
    if (solve()) {
      cout << "YES" << endl;
      cout << outputs.size() << endl;
      for (auto [x, y] : outputs) {
        cout << x << ' ' << y << endl;
      }
    } else {
      cout << "NO" << endl;
    }
  }
  return 0;
}

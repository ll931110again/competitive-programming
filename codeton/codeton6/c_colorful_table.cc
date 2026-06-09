// Codeforces 1870 (CodeTON Round 6 (Div. 1 + Div. 2, Rated, Prizes!)) — C. Colorful Table
// Submission: https://codeforces.com/contest/1870/submission/307444815

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 100005;
int T, n, k;
int a[k_max_n];
vector<int> pos[k_max_n];

vector<int> solve() {
  for (int i = 1; i <= k; i++) {
    pos[i].clear();
  }
  for (int i = 1; i <= n; i++) {
    pos[a[i]].push_back(i);
  }

  vector<int> ans(k);
  int ls = -1, rs = -1;
  for (int i = k; i > 0; i--) {
    if (pos[i].empty()) {
      ans[i - 1] = 0;
      continue;
    }

    for (auto x : pos[i]) {
      if (ls < 0 || ls > x) {
        ls = x;
      }
      if (rs < 0 || rs < x) {
        rs = x;
      }
    }

    ans[i - 1] = 2 * (rs - ls + 1);
  }
  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }

    auto ans = solve();
    for (auto x : ans) {
      cout << x << ' ';
    }
    cout << endl;
  }
  return 0;
}

// Codeforces 1810 (CodeTON Round 4 (Div. 1 + Div. 2, Rated, Prizes!)) — B. Candies
// Submission: https://codeforces.com/contest/1810/submission/201603434

#include <bits/stdc++.h>
using namespace std;

namespace {

int T, n;

vector<int> solve(int n) {
  if (n % 2 == 0) {
    return {-1};
  }

  vector<int> binary_rep;
  while (n > 0) {
    binary_rep.push_back(n % 2);
    n /= 2;
  }

  vector<int> ans = {2};
  for (int i = binary_rep.size() - 2; i > 0; i--) {
    if (binary_rep[i] == 0) {
      ans.push_back(1);
    } else {
      ans.push_back(2);
    }
  }
  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    auto ans = solve(n);
    if (ans[0] == -1) {
      cout << -1 << endl;
    } else {
      cout << ans.size() << ' ';
      for (auto x : ans) {
        cout << x << ' ';
      }
      cout << endl;
    }
  }
  return 0;
}

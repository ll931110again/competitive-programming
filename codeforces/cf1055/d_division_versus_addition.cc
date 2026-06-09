
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 250005;
int T, n, Q;
int a[k_max_n];
int logs[k_max_n], bs[k_max_n], cs[k_max_n];
unordered_set<int> p2, p2o;

int solve(int L, int R) {
  int ans = logs[R] - logs[L - 1] + cs[R] - cs[L - 1] + (bs[R] - bs[L - 1]) / 2;
  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  for (int mult = 1; mult <= 1e9; mult *= 2) {
    p2.insert(mult);
    p2o.insert(mult + 1);
  }

  cin >> T;
  while (T--) {
    cin >> n >> Q;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }

    for (int i = 1; i <= n; i++) {
      logs[i] = log(a[i]) / log(2);
      if (p2.find(a[i]) == p2.end() && p2o.find(a[i]) != p2o.end()) {
        bs[i] = 1;
      } else {
        bs[i] = 0;
      }

      if (p2.find(a[i]) == p2.end() && p2o.find(a[i]) == p2o.end()) {
        cs[i] = 1;
      } else {
        cs[i] = 0;
      }

      logs[i] += logs[i - 1];
      bs[i] += bs[i - 1];
      cs[i] += cs[i - 1];
    }

    while (Q--) {
      int L, R;
      cin >> L >> R;
      cout << solve(L, R) << endl;
    }
  }
  return 0;
}

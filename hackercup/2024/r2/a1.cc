#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 1000005;
int T, n;

long long solve() {
  long long A, B, M;
  cin >> A >> B >> M;

  long long ret = 0;
  for (int d = 1; d < 19; d += 2) {
    for (int st = 1; st < 10; st++) {
      vector<int> digits;
      int _st = st;
      for (int k = 0; k < d; k++) {
        digits.push_back(_st);
        if (k < d / 2) {
          _st++;
        } else {
          _st--;
        }
      }

      long long x = 0;
      bool ok = true;
      for (auto d : digits) {
        if (d > 9) {
          ok = false;
          break;
        }
        x = x * 10 + d;
      }
      if (ok && x >= A && x <= B && x % M == 0) {
        ret++;
      }
    }
  }
  return ret;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  for (int it = 1; it <= T; it++) {
    cout << "Case #" << it << ": " << solve() << '\n';
  }
  return 0;
}

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int M = 1e6;
i64 b, c, r, d;

i64 solve() {
  i64 z = (b * M + c) / r;
  if (c + d >= M - 1) {
    return z;
  }

  unordered_set<i64> seen;
  seen.insert(c);

  for (int it = 0;; it++) {
    if (b * M + c < r) {
      return it;
    }

    i64 new_b = b - r / M, new_c = c, new_d = d;
    if (c >= r % M) {
      new_c = c - r % M;
      new_d = d + r % M;
    } else {
      new_b--;
      int changes = M - r % M;
      new_c = c + changes;
      new_d = d - changes;
    }

    if (new_b < 0 || new_c < 0 || new_d < 0) {
      return it;
    }

    b = new_b;
    c = new_c;
    d = new_d;

    if (seen.find(c) != seen.end()) {
      return z;
    }

    seen.insert(c);
  }

  return z;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> b >> c >> r >> d;
  cout << solve() << endl;
  return 0;
}

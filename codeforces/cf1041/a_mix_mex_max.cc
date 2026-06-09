
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 105;
int T, n;
int a[k_max_n];

bool solve() {
  int found = -1;
  for (int i = 0; i < n; i++) {
    if (a[i] >= 0) {
      if (found < 0) {
        found = a[i];
      }
      if (found != a[i]) {
        return false;
      }
    }
  }

  if (found == 0) {
    return false;
  }

  return true;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    cout << (solve() ? "YES" : "NO") << endl;
  }
  return 0;
}

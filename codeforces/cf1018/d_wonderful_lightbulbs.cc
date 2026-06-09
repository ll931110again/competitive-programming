
#include <bits/stdc++.h>
using namespace std;

namespace {

int T, n;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    int fx = 0, fxy = 0;
    for (int i = 0; i < n; i++) {
      int x, y;
      cin >> x >> y;
      fx ^= x;
      fxy ^= (x + y);
    }
    cout << fx << ' ' << fxy - fx << endl;
  }
  return 0;
}

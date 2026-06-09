
#include <bits/stdc++.h>
using namespace std;

namespace {

int T, m, n;

bool solve(int m, int n) {
  if (m > n) {
    swap(m, n);
  }
  if (m == 1) {
    return false;
  }
  if (m == 2 && n == 2) {
    return false;
  }
  return true;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> m >> n;
    cout << (solve(m, n) ? "YES" : "NO") << endl;
  }
  return 0;
}

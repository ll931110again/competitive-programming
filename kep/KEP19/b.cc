#include <bits/stdc++.h>
using namespace std;

namespace {

int n, x, delta;

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> x;
    delta += x;
  }

  cout << (delta == 0 ? "OK" : "BUG") << endl;
  return 0;
}

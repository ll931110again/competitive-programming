
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 105;
int T, n;
int a[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }

    set<int> s;
    for (int i = 0; i < n; i++) {
      s.insert(a[i]);
    }

    cout << 2 * s.size() - 1 << endl;
  }
  return 0;
}

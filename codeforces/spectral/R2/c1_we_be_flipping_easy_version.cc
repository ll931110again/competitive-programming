
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int T, n;
int a[k_max_n];

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

    int flag = 0;
    vector<int> ops;
    for (int i = n - 1; i >= 0; i--) {
      if (a[i] == 0) {
        continue;
      }
      if (((a[i] > 0) ^ flag) == 0) {
        continue;
      }
      ops.push_back(i);
      flag ^= 1;
    }

    cout << (int)ops.size() << endl;
    for (auto x : ops) {
      cout << x + 1 << ' ';
    }
    cout << endl;
  }

  return 0;
}

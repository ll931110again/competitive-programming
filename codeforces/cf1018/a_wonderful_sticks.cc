
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 105;
int T, n;
string s;
int a[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    cin >> s;
    int low = 1, high = n;
    for (int i = n - 2; i >= 0; i--) {
      if (s[i] == '<') {
        a[i + 1] = low++;
      } else {
        a[i + 1] = high--;
      }
    }
    a[0] = low;

    for (int i = 0; i < n; i++) {
      cout << a[i] << ' ';
    }
    cout << endl;
  }
  return 0;
}

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 105;
int T;
int n;
int b[k_max_n], a[2 * k_max_n], used[2 * k_max_n];

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> T;
  while (T--) {
    cin >> n;
    memset(used, false, sizeof used);
    for (int i = 0; i < n; i++) {
      cin >> b[i];
    }
    for (int i = 0; i < n; i++) {
      a[2 * i] = b[i];
      used[b[i]] = true;
    }

    bool found_solutions = true;
    for (int i = 0; i < n; i++) {
      bool ok = false;
      for (int j = a[2 * i] + 1; j <= 2 * n; j++)
        if (!used[j]) {
          a[2 * i + 1] = j;
          used[j] = true;
          ok = true;
          break;
        }

      if (!ok) {
        found_solutions = false;
        break;
      }
    }

    if (!found_solutions) {
      cout << -1 << endl;
    } else {
      for (int i = 0; i < 2 * n; i++) {
        cout << a[i] << ' ';
      }
      cout << endl;
    }
  }

  return 0;
}

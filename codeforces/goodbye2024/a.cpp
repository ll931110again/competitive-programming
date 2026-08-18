#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }

    bool canMerge = false;
    for (int i = 0; i < n - 1; i++) {
      int mn = min(a[i], a[i + 1]);
      int mx = max(a[i], a[i + 1]);
      if (2 * mn > mx) {
        canMerge = true;
        break;
      }
    }

    cout << (canMerge ? "YES" : "NO") << '\n';
  }
  return 0;
}

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int T, n;
pair<int, int> a[k_max_n];

bool ok(int mid) {
  int maxy = 0;
  int j = 0;

  for (int i = 0; i < n; i++) {
    while (j < n && a[j].first < a[i].first - mid) {
      maxy = max(maxy, a[j].second);
      j++;
    }
    if (a[i].second + mid < maxy) {
      return false;
    }
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
      cin >> a[i].first >> a[i].second;
    }
    sort(a, a + n);

    int low = 0, high = 1e9, ans = high;
    while (low <= high) {
      int mid = (low + high) / 2;
      if (ok(mid)) {
        ans = mid;
        high = mid - 1;
      } else {
        low = mid + 1;
      }
    }

    cout << ans << endl;
  }
  return 0;
}

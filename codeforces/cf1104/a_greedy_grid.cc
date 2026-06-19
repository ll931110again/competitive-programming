
#include <bits/stdc++.h>
#define maxn 105
using namespace std;

int T, n;
int a[maxn];

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }

    int ans = 0, mx = a[0];
    for (int i = 0; i < n; i++) {
      mx = min(mx, a[i]);
      ans += mx;
    }

    cout << ans << endl;
  }
  return 0;
}

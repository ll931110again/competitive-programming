
#include <bits/stdc++.h>
#define maxn 200005
using namespace std;

using i64 = long long;
int T, n;
int a[maxn];

i64 solve() {
  i64 ans = 0;
  i64 mx = 0;
  for (int i = 0; i < n; i++) {
    mx = max(mx, (i64)a[i]);
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    cout << solve() << endl;
  }
  return 0;
}

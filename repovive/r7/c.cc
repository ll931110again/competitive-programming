#ifdef ONLINE_JUDGE
  #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#define maxn 200005
using namespace std;

namespace {
  int T, n, m;
  pair<int,int> events[maxn];
  int loc[maxn], ans[maxn], delta[maxn];
} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
      cin >> events[i].first >> events[i].second;
    }

    for (int i = 1; i <= n; i++) {
      ans[i] = -1;
      loc[i] = i;
      delta[i] = 0;
    }

    for (int i = 0; i < m; i++) {
      delta[events[i].first]--;
      delta[events[i].second]++;
    }

    bool ok = true;
    for (int i = 1; i <= n; i++) {
      int pos = i + delta[i];
      if (pos <= 0 || pos > n || ans[pos] != -1) {
        ok = false;
        break;
      }
      ans[pos] = i;
    }

    if (ok) {
      cout << "Yes" << endl;
      for (int i = 1; i <= n; i++) {
        cout << ans[i] << ' ';
      }
      cout << endl;
    } else {
      cout << "No" << endl;
    }
  }
  return 0;
}

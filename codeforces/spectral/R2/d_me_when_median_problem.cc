#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#define maxn 200005
using namespace std;

int T, n;
int a[maxn], b[maxn], events[2 * maxn];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
      events[i] = a[i];
    }
    for (int i = 0; i < n; i++) {
      cin >> b[i];
      events[i + n] = b[i];
    }

    auto ok = [&](int x) {
      int delta = 0;
      int last_value = 0;
      for (int i = 0; i < n; i++) {
        if (a[i] >= x && b[i] >= x) {
          last_value = 1;
          delta++;
        } else if (a[i] < x && b[i] < x) {
          if (last_value != -1) {
            delta--;
          }
          last_value = -1;
        }
      }
      return (delta > 0);
    };

    sort(events, events + 2 * n);
    int low = 0, high = 2 * n - 1, ans = 0;
    while (low <= high) {
      int mid = (low + high) / 2;
      if (ok(events[mid])) {
        ans = events[mid];
        low = mid + 1;
      } else {
        high = mid - 1;
      }
    }

    cout << ans << endl;
  }

  return 0;
}

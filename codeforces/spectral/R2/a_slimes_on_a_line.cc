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
#define maxn 1005
using namespace std;

int T, n;
int a[maxn];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    sort(a, a + n);
    int ans = (a[n - 1] - a[0] + 1) / 2;
    cout << ans << endl;
  }

  return 0;
}

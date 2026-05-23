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
#define maxn 100005
using namespace std;

int T, n;
int a[maxn], b[maxn];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    for (int i = 0; i < n; i++) {
      cin >> b[i];
    }
    for (int i = 0; i < n; i++) {
      if (a[i] > b[i]) {
        swap(a[i], b[i]);
      }
    }

    int max_a = 0;
    long long ans = 0;
    for (int i = 0; i < n; i++) {
      max_a = max(max_a, a[i]);
      ans += b[i];
    }
    ans += max_a;
    cout << ans << endl;
  }

  return 0;
}

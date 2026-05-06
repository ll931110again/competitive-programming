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
#define maxn 500005
using namespace std;

int T;
int n, k, x, y;
int a[maxn];

int solve(int st) {
  for (int i = 0; i < k; i++) {
    if (st == x) {
      st = y;
    }
    st = (st + a[i]) % n;
    if (st == x) {
      st = y;
    }
  }
  return st;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n >> k >> x >> y;
    for (int i = 0; i < k; i++) {
      cin >> a[i];
    }
    for (int i = 0; i < n; i++)
      if (i != x) {
        cout << solve(i) << ' ';
      }
    cout << endl;
  }

  return 0;
}

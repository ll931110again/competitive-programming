#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <unordered_set>
#include <vector>
using namespace std;

int T, n;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    int fx = 0, fxy = 0;
    for (int i = 0; i < n; i++) {
      int x, y;
      cin >> x >> y;
      fx ^= x;
      fxy ^= (x + y);
    }
    cout << fx << ' ' << fxy - fx << endl;
  }
  return 0;
}
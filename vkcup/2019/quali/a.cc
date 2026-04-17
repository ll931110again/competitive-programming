#ifdef ONLINE_JUDGE
  #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#include <unordered_set>
#include <set>
using namespace std;

int T;
int m, n, x, y;

int main() {
  cin >> T;
  while (T--) {
    cin >> m >> n >> x >> y;
    int ans = max(x, m - x - 1) * n;
    ans = max(ans, m * max(y, n - y - 1));
    cout << ans << endl;
  }
  return 0;
}
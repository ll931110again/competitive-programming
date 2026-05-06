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
#define maxn 15
using namespace std;

int T, n;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    bool found = false;
    for (int i = 0; i < n; i++) {
      int x;
      cin >> x;
      if (x == 100) {
        found = true;
      }
    }
    cout << (found ? "Yes" : "No") << endl;
  }

  return 0;
}

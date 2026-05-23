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

    int flag = 0;
    vector<int> ops;
    for (int i = n - 1; i >= 0; i--) {
      if (a[i] == 0) {
        continue;
      }
      if (((a[i] > 0) ^ flag) == 0) {
        continue;
      }
      ops.push_back(i);
      flag ^= 1;
    }

    cout << (int)ops.size() << endl;
    for (auto x : ops) {
      cout << x + 1 << ' ';
    }
    cout << endl;
  }

  return 0;
}

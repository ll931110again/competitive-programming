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
long long ss[maxn], abs_s[maxn];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }
    ss[0] = 0;
    abs_s[0] = 0;
    for (int i = 1; i <= n; i++) {
      ss[i] = ss[i - 1] + a[i];
      abs_s[i] = abs_s[i - 1] + abs(a[i]);
    }

    long long max_value = 0;
    for (int i = 1; i <= n; i++) {
      max_value += a[i];
    }

    int max_pos = 0;

    for (int i = 1; i <= n; i++) {
      if (a[i] > 0) {
        long long tmp = abs_s[i - 1] - a[i] + (ss[n] - ss[i]);
        if (max_value < tmp) {
          max_value = tmp;
          max_pos = i;
        }
      }
    }

    vector<int> ops;
    int flag = 0;
    for (int i = max_pos - 1; i > 0; i--) {
      if (((a[i] > 0) ^ flag) == 0) {
        continue;
      }
      flag ^= 1;
      ops.push_back(i);
    }
    if (max_pos > 0) {
      ops.push_back(max_pos);
    }

    cout << (int)ops.size() << endl;
    for (auto x : ops) {
      cout << x << ' ';
    }
    cout << endl;
  }

  return 0;
}

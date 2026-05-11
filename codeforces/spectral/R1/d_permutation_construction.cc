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
long long prefix[maxn];

int id[maxn], ans[maxn];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }

    prefix[0] = 0;
    for (int i = 0; i < n; i++) {
      prefix[i + 1] = prefix[i] + a[i];
    }

    for (int i = 0; i < n; i++) {
      id[i] = i;
    }

    sort(id, id + n, [&](int u, int v) { return prefix[u] > prefix[v]; });

    for (int i = 0; i < n; i++) {
      ans[id[i]] = i;
    }

    for (int i = 0; i < n; i++) {
      cout << ans[i] + 1 << ' ';
    }

    cout << endl;
  }

  return 0;
}

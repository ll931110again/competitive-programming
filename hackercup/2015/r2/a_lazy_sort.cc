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
#define maxn 50005
using namespace std;

int T, n;
int a[maxn];

int longest_suffix[maxn];
int min_prefix[maxn], max_prefix[maxn], min_suffix[maxn], max_suffix[maxn];
int inf = 1e9;

bool ok() {
  memset(longest_suffix, -1, sizeof longest_suffix);
  longest_suffix[0] = 1;
  longest_suffix[1] = 0;

  min_prefix[0] = inf;
  max_prefix[0] = -1;
  min_suffix[0] = inf;
  max_suffix[0] = -1;

  min_prefix[1] = a[1];
  max_prefix[1] = a[1];
  for (int i = 2; i <= n; i++) {
    min_prefix[i] = min(min_prefix[i - 1], a[i]);
    max_prefix[i] = max(max_prefix[i - 1], a[i]);
  }

  min_suffix[1] = a[n];
  max_suffix[1] = a[n];
  for (int i = 2; i <= n; i++) {
    min_suffix[i] = min(min_suffix[i - 1], a[n - i + 1]);
    max_suffix[i] = max(max_suffix[i - 1], a[n - i + 1]);
  }

  queue<int> q;
  q.push(0);
  q.push(1);

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    if (u + longest_suffix[u] >= n) {
      continue;
    }

    // Check if we can extend longest_suffix[u] through two ways:
    // * Extend longest_suffix[u + 1]
    // * Extend longest_suffix[u] + 1

    int ls = min(min_prefix[u], min_suffix[longest_suffix[u]]);
    int rs = max(max_prefix[u], max_suffix[longest_suffix[u]]);
    if (a[u + 1] == ls - 1 || a[u + 1] == rs + 1) {
      if (longest_suffix[u + 1] < longest_suffix[u]) {
        longest_suffix[u + 1] = longest_suffix[u];
        q.push(u + 1);
      }
    }

    int v = n - longest_suffix[u];
    if (a[v] == ls - 1 || a[v] == rs + 1) {
      longest_suffix[u]++;
      q.push(u);
    }
  }

  for (int i = 0; i <= n; i++) {
    if (longest_suffix[i] == n - i) {
      return true;
    }
  }

  return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
      cin >> n;
      for (int i = 1; i <= n; i++) {
        cin >> a[i];
      }

      cout << "Case #" << it << ": " << (ok() ? "yes" : "no") << endl;
    }

    return 0;
}
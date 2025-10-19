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
#define maxn 200005
using namespace std;

constexpr unsigned MOD = 998'244'353;

int T, n, m;
int a[maxn];
vector<int> adj[maxn];

long long ans = 0;

vector<long long> DFS(int u, int p) {
  vector<long long> values;
  for (auto v : adj[u]) if (v != p) {
    auto tmp = DFS(v, u);
    if (values.empty()) {
      values = tmp;
    } else {
      for (int i = 0; i < values.size(); i++) {
        values[i] = min(values[i], tmp[i]);
      }
    }
  }

  if (values.empty()) {
    values.resize(m + 1);
  }
  vector<long long> tmp = values;
  for (int i = 1; i <= m; i++) {
    values[i] = max(values[i], tmp[i - 1] + a[u]);
  }

  for (int i = 1; i <= m; i++) {
    ans += values[i];
    ans %= MOD;
  }

  return values;
}

long long solve() {
  ans = 0;
  DFS(1, -1);

  return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
        cerr << it << endl;

        cin >> n >> m;
        for (int i = 1; i <= n; i++) {
            adj[i].clear();
        }

        for (int i = 1; i <= n; i++) {
          cin >> a[i];
        }

        for (int i = 1; i < n; i++) {
          int u, v;
          cin >> u >> v;
          adj[u].push_back(v);
          adj[v].push_back(u);
        }

        cout << "Case #" << it << ": " << solve() << endl;
    }

    return 0;
}
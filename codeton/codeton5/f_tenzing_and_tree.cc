// Codeforces 1842 (CodeTON Round 5 (Div. 1 + Div. 2, Rated, Prizes!)) — F. Tenzing and Tree
// Submission: https://codeforces.com/contest/1842/submission/334322452

#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 5005
using namespace std;

int n;
vector<int> adj[maxn];
int ans[maxn];
bool vis[maxn];

void solve(int root) {
  memset(vis, false, sizeof vis);

  queue<pair<int, int>> q;
  q.push({root, 0});
  vis[root] = true;

  int k = 0, adds = 0, subs = 0;
  while (!q.empty()) {
    auto u = q.front().first, depth = q.front().second;
    q.pop();

    k++;
    adds += (n - 1);
    subs += 2 * depth;
    ans[k] = max(ans[k], adds - subs);

    for (auto v : adj[u])
      if (!vis[v]) {
        vis[v] = true;
        q.push({v, depth + 1});
      }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 1; i < n; i++) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  for (int root = 1; root <= n; root++) {
    solve(root);
  }

  for (int i = 0; i <= n; i++) {
    cout << ans[i] << ' ';
  }
  cout << endl;

  return 0;
}

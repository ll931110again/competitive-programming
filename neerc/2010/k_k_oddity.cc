/*
 * Solution for problem K: K-Oddity.
 *
 */

#ifdef ONLINE_JUDGE
  #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <unordered_set>
#include <vector>
#define maxn 10005
using namespace std;

int m, n;
vector<int> adj[maxn];
int color[maxn];
int ks;

void DFS(int u) {
  unordered_set<int> found_colors;
  for (auto v : adj[u]) if (color[v]) {
    found_colors.insert(color[v]);
  }

  color[u] = 1;
  while (found_colors.find(color[u]) != found_colors.end()) {
    color[u]++;
  }

  for (auto v : adj[u]) if (!color[v]) {
    DFS(v);
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("kgraph.in", "r", stdin);
  freopen("kgraph.out", "w", stdout);

  cin >> n >> m;
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  for (int i = 1; i <= n; i++) {
    ks = max(ks, (int) adj[i].size());
  }
  if (ks % 2 == 0) {
    ks++;
  }

  int start_idx = -1;
  for (int i = 1; i <= n; i++) if (adj[i].size() < ks) {
    start_idx = i;
  }

  DFS(start_idx);

  cout << ks << endl;
  for (int i = 1; i <= n; i++) {
    cout << color[i] << endl;
  }
  return 0;
}

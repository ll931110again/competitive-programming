/*
 * Solution for problem K: Kingdom Roadmap.
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
#define maxn 100005
using namespace std;

int n;
vector<int> adj[maxn];
vector<int> leaves;

void DFS(int u, int par) {
  for (auto v : adj[u]) if (v != par) {
    DFS(v, u);
  }
  if (adj[u].size() == 1) {
    leaves.push_back(u);
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("kingdom.in", "r", stdin);
  freopen("kingdom.out", "w", stdout);

  cin >> n;
  for (int i = 1; i < n; i++) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  DFS(1, -1);
  int s = leaves.size();
  cout << (s + 1) / 2 << endl;
  for (int i = 0; i < s/2; i++) {
    cout << leaves[i] << ' ' << leaves[s/2 + i] << endl;
  }
  if (s & 1) {
    cout << leaves[s/2] << ' ' << leaves[s - 1] << endl;
  }

  return 0;
}

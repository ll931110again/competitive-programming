#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 100005;
int init[k_max_n];
bool touch[k_max_n], ford[k_max_n], back[k_max_n];
vector<int> adj[k_max_n], rev[k_max_n];
int n, m;

void BFS() {
  queue<int> q;
  for (int i = 1; i <= n; i++)
    if (init[i] == 1) {
      ford[i] = true;
      q.push(i);
      touch[i] = true;
    }
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int i = 0; i < adj[u].size(); i++) {
      int v = adj[u][i];
      if (init[v] == 1 || touch[v])
        continue;
      ford[v] = true;
      touch[v] = true;
      q.push(v);
    }
  }

  memset(touch, false, sizeof(touch));
  for (int i = 1; i <= n; i++)
    if (init[i] == 2) {
      touch[i] = back[i] = true;
      q.push(i);
    }
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    if (init[u] == 1)
      continue;
    for (int i = 0; i < rev[u].size(); i++) {
      int v = rev[u][i];
      if (touch[v])
        continue;
      back[v] = touch[v] = true;
      q.push(v);
    }
  }

  for (int i = 1; i <= n; i++)
    if (back[i] && ford[i])
      cout << "1\n";
    else
      cout << "0\n";
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  for (int i = 1; i <= n; i++)
    cin >> init[i];
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    rev[v].push_back(u);
  }
  BFS();
}

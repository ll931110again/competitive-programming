// Codeforces 82 (Yandex.Algorithm 2011: Qualification 2) — C. General Mobilization
// Submission: https://codeforces.com/contest/82/submission/437093

#include <bits/stdc++.h>
using namespace std;

int cap[5002], cur[5002], ret[5002], pos[5002], parent[5002];
int n;
pair<int, int> p[5002];
vector<pair<int, int>> adj[5002];

void DFS(int u, int pre) {
  for (int i = 0; i < adj[u].size(); i++) {
    int v = adj[u][i].first;
    if (pre == v)
      continue;
    DFS(v, u);
    parent[v] = u;
    cap[v] = adj[u][i].second;
  }
}

int main() {
  // freopen("c.in","r",stdin);

  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%d", &p[i].first);
    p[i].second = i;
  }
  sort(p, p + n);

  // for (int i = 0; i < n; i++) cout << p[i].first << ' ' << p[i].second << endl;

  for (int i = 1; i < n; i++) {
    int a, b, c;
    scanf("%d %d %d", &a, &b, &c);
    a--;
    b--;
    adj[a].push_back(make_pair(b, c));
    adj[b].push_back(make_pair(a, c));
  }

  DFS(0, -1);
  memset(ret, 0, sizeof(ret));
  for (int i = 0; i < n; i++)
    pos[i] = i;

  // for (int i = 0; i < n; i++) cout << p[i].first << ' ' << p[i].second << endl;

  for (int iter = 1; iter <= n; iter++) {
    for (int i = 0; i < n; i++)
      cur[i] = cap[i];
    for (int i = 0; i < n; i++) {
      int u = p[i].second;
      if (pos[u] > 0) {
        ret[u]++;
        if (cur[pos[u]] > 0) {
          cur[pos[u]]--;
          pos[u] = parent[pos[u]];
        }
      }
    }
  }

  for (int i = 0; i < n; i++) {
    printf("%d", ret[i]);
    if (i < n - 1)
      printf(" ");
    else
      printf("\n");
  }
}

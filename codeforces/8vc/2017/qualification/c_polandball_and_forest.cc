// Codeforces 755 (8VC Venture Cup 2017 - Elimination Round) — C. PolandBall and Forest
// Submission: https://codeforces.com/contest/755/submission/23854630

#include <bits/stdc++.h>
using namespace std;

#define MAXN 10005

namespace {

int n;
int a[MAXN];
vector<int> adj[MAXN];
bool mark[MAXN];

void DFS(int u) {
  for (auto v : adj[u])
    if (!mark[v]) {
      mark[v] = true;
      DFS(v);
    }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
    if (a[i] != i) {
      adj[i].push_back(a[i]);
      adj[a[i]].push_back(i);
    }
  }
  int cnt = 0;
  for (int i = 1; i <= n; i++)
    if (!mark[i]) {
      mark[i] = true;
      DFS(i);
      cnt++;
    }
  printf("%d\n", cnt);
  return 0;
}

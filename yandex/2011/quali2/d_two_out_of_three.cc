// Codeforces 82 (Yandex.Algorithm 2011: Qualification 2) — D. Two out of Three
// Submission: https://codeforces.com/contest/82/submission/438682

#include <bits/stdc++.h>
using namespace std;

int f[1005][1005];
int cost[1005];
int n;
int INF = (1 << 30) - 5;
pair<int, int> trace[1005][1005];

int rec(int a, int b) {
  if (a > n)
    return 0;
  if (b > n)
    return f[a][b] = cost[a];

  if (f[a][b] >= 0)
    return f[a][b];
  f[a][b] = INF;
  int tmp = max(cost[a], cost[b]) + rec(b + 1, b + 2);
  if (f[a][b] > tmp) {
    f[a][b] = tmp;
    trace[a][b] = make_pair(a, b);
  }
  if (b < n) {
    tmp = max(cost[a], cost[b + 1]) + rec(b, b + 2);
    if (f[a][b] > tmp) {
      f[a][b] = tmp;
      trace[a][b] = make_pair(a, b + 1);
    }
  }
  if (b < n) {
    tmp = max(cost[b], cost[b + 1]) + rec(a, b + 2);
    if (f[a][b] > tmp) {
      f[a][b] = tmp;
      trace[a][b] = make_pair(b, b + 1);
    }
  }
  return f[a][b];
}

void track(int a, int b) {
  if (a > n)
    return;
  if (b > n) {
    printf("%d\n", a);
    return;
  }
  int u = trace[a][b].first, v = trace[a][b].second;
  printf("%d %d\n", u, v);
  if (u == a && v == b)
    track(b + 1, b + 2);
  else if (u == a && v == b + 1)
    track(b, b + 2);
  else if (u == b && v == b + 1)
    track(a, b + 2);
}

int main() {
  // freopen("d.in","r",stdin);
  scanf("%d", &n);
  for (int i = 1; i <= n; i++)
    scanf("%d", &cost[i]);
  memset(f, -1, sizeof(f));
  printf("%d\n", rec(1, 2));
  track(1, 2);
}

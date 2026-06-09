#include <bits/stdc++.h>
using namespace std;

#define maxm 20002

namespace {

int cap[maxm], flow[maxm], cost[maxm], a[maxm], b[maxm], c[maxm];
int task_idx[maxm];

int n, k, m, end;
vector<pair<int, int>> adj[maxm];
int INF = (1e9) + 4;

void add(int u, int v, int f, int c) {
  adj[u].push_back(make_pair(v, m));
  adj[v].push_back(make_pair(u, m + 1));
  cap[m] = f;
  cost[m] = c;
  m += 2;
}

void max_flow() {
  int trace[2005], trace_edge[2005], d[2005];
  bool inqueue[2005];

  while (1) {
    memset(trace, -1, sizeof(trace));
    trace[0] = -2;
    for (int i = 0; i <= end; i++)
      d[i] = (i == 0) ? 0 : INF;
    memset(inqueue, false, sizeof(inqueue));

    queue<int> q;
    q.push(0);
    inqueue[0] = true;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      inqueue[u] = false;
      for (int i = 0; i < adj[u].size(); i++) {
        int v = adj[u][i].first, road = adj[u][i].second;
        if (cap[road] <= flow[road])
          continue;
        int fcost = (flow[road] >= 0) ? cost[road] : -cost[road ^ 1];
        if (d[v] > d[u] + fcost) {
          d[v] = d[u] + fcost;
          trace[v] = u;
          trace_edge[v] = road;

          if (!inqueue[v])
            q.push(v);
          inqueue[v] = true;
        }
      }
    }

    if (d[end] >= INF)
      return;
    for (int v = end, u = trace[v]; v != 0; v = u, u = trace[v]) {
      int x = trace_edge[v];
      flow[x]++;
      flow[x ^ 1]--;
    }
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> k;
  for (int i = 0; i < n; i++) {
    cin >> a[i] >> b[i] >> c[i];
    b[i] += a[i];
  }
  vector<pair<int, int>> v;
  for (int i = 0; i < n; i++) {
    v.push_back(make_pair(a[i], i));
    v.push_back(make_pair(b[i], i + n));
  }
  sort(v.begin(), v.end());
  end = -1;
  for (int i = 0; i < v.size(); i++) {
    if (!i || v[i].first > v[i - 1].first)
      end++;
    if (v[i].second < n)
      a[v[i].second] = end;
    else
      b[v[i].second - n] = end;
  }
  end++;
  for (int i = 0; i < end; i++)
    add(i, i + 1, k, 0);
  for (int i = 0; i < n; i++) {
    task_idx[i] = m;
    add(a[i], b[i], 1, -c[i]);
  }
  max_flow();
  for (int i = 0; i < n; i++)
    if (flow[task_idx[i]] > 0)
      cout << "1 ";
    else
      cout << "0 ";
  cout << '\n';
}

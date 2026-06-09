#include <bits/stdc++.h>
using namespace std;

namespace {

vector<int> adj[1000005];
long long sub[1000005];
int weight[1000005];

int LocateCentre(int n, int p[], int s[], int d[]) {
  for (int i = 0; i < n; ++i) {
    adj[i].clear();
  }
  for (int i = 0; i < n - 1; ++i) {
    adj[s[i]].push_back(d[i]);
    adj[d[i]].push_back(s[i]);
  }
  for (int i = 0; i < n; ++i) {
    weight[i] = p[i];
  }

  vector<int> parent(n, -1), order;
  order.reserve(n);
  vector<int> stack;
  stack.push_back(0);
  parent[0] = -1;
  while (!stack.empty()) {
    const int u = stack.back();
    stack.pop_back();
    order.push_back(u);
    for (int v : adj[u]) {
      if (v == parent[u]) {
        continue;
      }
      parent[v] = u;
      stack.push_back(v);
    }
  }
  reverse(order.begin(), order.end());

  long long total = 0;
  for (int u : order) {
    sub[u] = weight[u];
    for (int v : adj[u]) {
      if (parent[v] == u) {
        sub[u] += sub[v];
      }
    }
    total += weight[u];
  }

  long long best_val = (1LL << 60);
  int best_node = 0;
  struct Frame {
    int u;
    int p;
    long long up;
  };
  vector<Frame> st;
  st.push_back({0, -1, 0});
  while (!st.empty()) {
    const frame cur = st.back();
    st.pop_back();

    long long worst = cur.p == -1 ? 0 : cur.up;
    for (int v : adj[cur.u]) {
      if (v == cur.p) {
        continue;
      }
      worst = max(worst, sub[v]);
    }
    if (worst < best_val) {
      best_val = worst;
      best_node = cur.u;
    }

    for (int v : adj[cur.u]) {
      if (v == cur.p) {
        continue;
      }
      st.push_back({v, cur.u, total - sub[v]});
    }
  }

  return best_node;
}

} // namespace

// Codeforces Spectral::Cup 2026 Round 2 — The Endians
// https://codeforces.com/contest/2229/problem/I
//
// Tree knapsack + rerooting (editorial boring solution).
// Complexity: O(n^2) per test, sum n <= 4000.

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int kMaxN = 4005;

int n, k;
int64_t down[kMaxN][kMaxN];
int64_t down_take[kMaxN][kMaxN];
int64_t up[kMaxN][kMaxN];
int64_t pref_merge[kMaxN][kMaxN];
int64_t suff_merge[kMaxN][kMaxN];
vector<int> adj[kMaxN];
vector<int> children[kMaxN];
int64_t weight[kMaxN];
int subtree[kMaxN];

void dfs_down(int node, int parent) {
  subtree[node] = 1;
  children[node].clear();
  for (int to : adj[node]) {
    if (to == parent) {
      continue;
    }
    children[node].push_back(to);
    dfs_down(to, node);
    for (int i = min(subtree[node] - 1, k); i >= 0; i--) {
      for (int j = 1; j <= subtree[to] && i + j <= k; j++) {
        down[node][i + j] = max(down[node][i + j], down[node][i] + down_take[to][j]);
      }
    }
    subtree[node] += subtree[to];
  }
  for (int i = min(k, subtree[node]); i > 0; i--) {
    down_take[node][i] = max(down[node][i], down[node][i - 1] + int64_t(i) * weight[node]);
  }
}

void dfs_up(int node) {
  const int deg = static_cast<int>(children[node].size());
  if (deg == 1) {
    for (int i = 0; i <= k; i++) {
      up[children[node][0]][i] = up[node][i];
    }
  } else if (deg >= 2) {
    int prefix_size = subtree[children[node][0]];
    for (int i = 0; i <= k; i++) {
      pref_merge[0][i] = down_take[children[node][0]][i];
      suff_merge[deg][i] = up[node][i];
    }
    for (int c = 1; c < deg; c++) {
      for (int i = 0; i <= k; i++) {
        pref_merge[c][i] = 0;
      }
      const int child = children[node][c];
      prefix_size += subtree[child];
      for (int i = min(k, prefix_size); i >= 0; i--) {
        for (int j = min(i, subtree[child]); j >= 0 && i - j <= prefix_size - subtree[child]; j--) {
          pref_merge[c][i] = max(pref_merge[c][i], pref_merge[c - 1][i - j] + down_take[child][j]);
        }
      }
    }
    int suffix_size = prefix_size;
    for (int c = deg - 1; c >= 0; c--) {
      for (int i = 0; i <= k; i++) {
        suff_merge[c][i] = 0;
      }
      const int child = children[node][c];
      suffix_size -= subtree[child];
      for (int i = k; i >= max(0, k - suffix_size - 1); i--) {
        for (int j = min(i, subtree[child]); j >= 0; j--) {
          suff_merge[c][i] = max(suff_merge[c][i], suff_merge[c + 1][i - j] + down_take[child][j]);
        }
      }
      for (int i = k; i >= max(0, k - subtree[child] - 1); i--) {
        for (int j = min(i, suffix_size); j >= 0; j--) {
          up[child][i] = max(up[child][i], suff_merge[c + 1][i - j] + (c > 0 ? pref_merge[c - 1][j] : 0));
        }
      }
    }
  }
  for (int child : children[node]) {
    for (int i = min(k, n - subtree[child]); i >= 1; i--) {
      up[child][i] = max(up[child][i], up[child][i - 1] + int64_t(i) * weight[node]);
    }
    dfs_up(child);
  }
}

void solve_case() {
  cin >> n >> k;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= k; j++) {
      down[i][j] = 0;
      down_take[i][j] = 0;
      up[i][j] = 0;
    }
    adj[i].clear();
    children[i].clear();
    cin >> weight[i];
  }
  for (int i = 0; i < n - 1; i++) {
    int u, v;
    cin >> u >> v;
    u--;
    v--;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  dfs_down(0, -1);
  dfs_up(0);

  for (int i = 0; i < n; i++) {
    int64_t best = 0;
    for (int j = 0; j < min(k, subtree[i]); j++) {
      best = max(best, down[i][j] + up[i][k - 1 - j] + int64_t(k) * weight[i]);
    }
    cout << best << (i + 1 == n ? '\n' : ' ');
  }
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int tests;
  cin >> tests;
  while (tests--) {
    solve_case();
  }
  return 0;
}

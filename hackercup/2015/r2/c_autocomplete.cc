#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 20002
#define maxk 102
#define maxl 26
using namespace std;

int T, n, K;
string s[maxn];

int nodes[maxn][maxl];
int dp[maxn][maxk];
int depth[maxn], is_terminal[maxn];

int cur[maxk], nxt[maxk];
int num_nodes = 1;

int solve() {
  memset(nodes, -1, sizeof nodes);
  memset(is_terminal, false, sizeof is_terminal);
  depth[0] = 0;
  num_nodes = 1;

  for (int i = 0; i < n; i++) {
    int root = 0;
    for (auto c : s[i]) {
      int cx = c - 'a';
      if (nodes[root][cx] < 0) {
        nodes[root][cx] = num_nodes++;
        depth[num_nodes - 1] = depth[root] + 1;
      }
      root = nodes[root][cx];
    }
    is_terminal[root] = true;
  }

  memset(dp, -1, sizeof dp);
  for (int node = num_nodes - 1; node >= 0; node--) {
    dp[node][0] = 0;

    memset(cur, -1, sizeof cur);
    memset(nxt, -1, sizeof nxt);
    cur[0] = 0;
    for (int j = 0; j < maxl; j++) if (nodes[node][j] >= 0) {
      int nxt_node = nodes[node][j];
      for (int k1 = 0; k1 <= K; k1++) if (cur[k1] >= 0) {
        for (int k2 = 0; k1 + k2 <= K; k2++) if (dp[nxt_node][k2] >= 0) {
          if (nxt[k1 + k2] < 0 || nxt[k1 + k2] > cur[k1] + dp[nxt_node][k2]) {
            nxt[k1 + k2] = cur[k1] + dp[nxt_node][k2];
          }
        }
      }

      for (int k = 0; k <= K; k++) {
        cur[k] = nxt[k];
      }

      memset(nxt, -1, sizeof nxt);
    }

    for (int k = 1; k <= K; k++) {
      if (cur[k] >= 0) {
        dp[node][k] = cur[k];
      }
      if (is_terminal[node] && cur[k - 1] >= 0 && (dp[node][k] < 0 || dp[node][k] > depth[node] + cur[k - 1])) {
        dp[node][k] = depth[node] + cur[k - 1];
      }
    }

    if (node > 0) {
      dp[node][1] = depth[node];
    }
  }

  return dp[0][K];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
      cin >> n >> K;
      for (int i = 0; i < n; i++) {
        cin >> s[i];
      }

      cout << "Case #" << it << ": " << solve() << endl;
    }

    return 0;
}
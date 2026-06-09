// IOI 2011 Day 1 – Race (centroid decomposition)
// Find minimum edge count among paths of total length exactly K.
// Time: O(N log N), memory: O(N + K).

#include "race.h"
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_inf = 1e9;

struct Edge {
  int to;
  int len;
};

vector<Edge> adj[200005];

int n, target_k;
int subtree_size[200005];
bool removed[200005];
int min_edges[1000005];
vector<int> touched;

int dfs_size(int u, int parent) {
  subtree_size[u] = 1;
  for (const Edge& edge : adj[u]) {
    if (edge.to == parent || removed[edge.to]) {
      continue;
    }
    subtree_size[u] += dfs_size(edge.to, u);
  }
  return subtree_size[u];
}

int find_centroid(int u, int parent, int tree_size) {
  for (const Edge& edge : adj[u]) {
    if (edge.to == parent || removed[edge.to]) {
      continue;
    }
    if (subtree_size[edge.to] > tree_size / 2) {
      return find_centroid(edge.to, u, tree_size);
    }
  }
  return u;
}

struct DistEntry {
  int dist;
  int edges;
};

vector<DistEntry> collected;

void collect(int u, int parent, int dist, int edges) {
  if (dist > target_k) {
    return;
  }
  collected.push_back({dist, edges});
  for (const Edge& edge : adj[u]) {
    if (edge.to == parent || removed[edge.to]) {
      continue;
    }
    collect(edge.to, u, dist + edge.len, edges + 1);
  }
}

void mark_best(int dist, int edges) {
  if (dist > target_k) {
    return;
  }
  if (min_edges[dist] == k_inf) {
    touched.push_back(dist);
  }
  min_edges[dist] = min(min_edges[dist], edges);
}

void clear_best() {
  for (int dist : touched) {
    min_edges[dist] = k_inf;
  }
  touched.clear();
}

int answer = k_inf;

void process_centroid(int centroid) {
  mark_best(0, 0);
  for (const Edge& edge : adj[centroid]) {
    if (removed[edge.to]) {
      continue;
    }

    collected.clear();
    collect(edge.to, centroid, edge.len, 1);
    for (const DistEntry& entry : collected) {
      const int need = target_k - entry.dist;
      if (need >= 0 && min_edges[need] < k_inf) {
        answer = min(answer, min_edges[need] + entry.edges);
      }
    }
    for (const DistEntry& entry : collected) {
      mark_best(entry.dist, entry.edges);
    }
  }
  clear_best();
}

void decompose(int entry) {
  const int tree_size = dfs_size(entry, -1);
  const int centroid = find_centroid(entry, -1, tree_size);
  process_centroid(centroid);
  removed[centroid] = true;
  for (const Edge& edge : adj[centroid]) {
    if (!removed[edge.to]) {
      decompose(edge.to);
    }
  }
}

} // namespace

int best_path(int N, int K, int H[][2], int L[]) {
  n = N;
  target_k = K;
  answer = k_inf;
  for (int i = 0; i < N; ++i) {
    adj[i].clear();
  }
  fill(removed, removed + N, false);
  fill(min_edges, min_edges + K + 1, k_inf);

  for (int i = 0; i < N - 1; ++i) {
    const int u = H[i][0];
    const int v = H[i][1];
    adj[u].push_back({v, L[i]});
    adj[v].push_back({u, L[i]});
  }

  decompose(0);
  return answer == k_inf ? -1 : answer;
}

#ifdef RACE_LOCAL_MAIN
#include <iostream>

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, K;
  if (!(cin >> N >> K)) {
    return 0;
  }
  int H[200005][2];
  int L[200005];
  for (int i = 0; i < N - 1; ++i) {
    cin >> H[i][0] >> H[i][1] >> L[i];
  }
  cout << best_path(N, K, H, L) << '\n';
  return 0;
}
#endif

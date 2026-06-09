// IOI 2011 Day 1 – Race (centroid + small-to-large map merging)
// At each centroid, child distance maps are merged light-into-heavy instead of
// scanning a full length-K array. Time: O(N log^2 N).

#include "race.h"
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_inf = 1e9;
using DistMap = map<long long, int>;

struct Edge {
  int to;
  int len;
};

vector<Edge> adj[200005];

int n, target_k;
int subtree_size[200005];
bool removed[200005];
int answer = k_inf;

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

void collect(int u, int parent, int dist, int edges, DistMap& out) {
  if (dist > target_k) {
    return;
  }
  auto it = out.find(dist);
  if (it == out.end()) {
    out.emplace(dist, edges);
  } else {
    it->second = min(it->second, edges);
  }
  for (const Edge& edge : adj[u]) {
    if (edge.to == parent || removed[edge.to]) {
      continue;
    }
    collect(edge.to, u, dist + edge.len, edges + 1, out);
  }
}

void merge_maps(DistMap& big, DistMap& small) {
  if (small.size() > big.size()) {
    big.swap(small);
  }
  for (const auto& [dist, edges] : small) {
    auto it = big.find(dist);
    if (it == big.end()) {
      big.emplace(dist, edges);
    } else {
      it->second = min(it->second, edges);
    }
  }
  small.clear();
}

void query_cross(const DistMap& left, const DistMap& right) {
  if (left.size() > right.size()) {
    for (const auto& [dist, edges] : right) {
      const long long need = target_k - dist;
      auto it = left.find(need);
      if (it != left.end()) {
        answer = min(answer, it->second + edges);
      }
    }
  } else {
    for (const auto& [dist, edges] : left) {
      const long long need = target_k - dist;
      auto it = right.find(need);
      if (it != right.end()) {
        answer = min(answer, it->second + edges);
      }
    }
  }
}

void process_centroid(int centroid) {
  DistMap processed;
  processed[0] = 0;

  for (const Edge& edge : adj[centroid]) {
    if (removed[edge.to]) {
      continue;
    }
    DistMap child;
    collect(edge.to, centroid, edge.len, 1, child);
    query_cross(processed, child);
    merge_maps(processed, child);
  }
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

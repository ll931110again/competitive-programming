/*
 * Solution for BOI 2011 Day 1: Switch the Lamp On.
 *
 * 0-1 BFS on tile corners. Intra-tile edges cost 0 when the wire matches and 1
 * when the tile would need to be rotated.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_dim = 505;
constexpr int k_inf = 1e9;

int row_count;
int col_count;
char tiles[k_max_dim][k_max_dim];
int distance_to[k_max_dim][k_max_dim];
int delta_row[4] = {0, 0, 1, -1};
int delta_col[4] = {0, 1, 0, -1};

int vertex_id(int row, int col) {
  return row * (col_count + 1) + col;
}

void add_edge(vector<vector<pair<int, int>>>& graph, int from, int to, int weight) {
  graph[from].push_back({to, weight});
  graph[to].push_back({from, weight});
}

void add_tile_edges(vector<vector<pair<int, int>>>& graph, int row, int col, char wire) {
  int top_left = vertex_id(row - 1, col - 1);
  int top_right = vertex_id(row - 1, col);
  int bottom_left = vertex_id(row, col - 1);
  int bottom_right = vertex_id(row, col);
  if (wire == '/') {
    add_edge(graph, top_right, bottom_left, 0);
    add_edge(graph, top_left, bottom_right, 1);
  } else {
    add_edge(graph, top_left, bottom_right, 0);
    add_edge(graph, top_right, bottom_left, 1);
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> row_count >> col_count;
  for (int row = 1; row <= row_count; row++) {
    string line;
    cin >> line;
    for (int col = 1; col <= col_count; col++) {
      tiles[row][col] = line[col - 1];
    }
  }

  if ((row_count + col_count) % 2 == 1) {
    cout << -1 << '\n';
    return 0;
  }

  int vertex_count = (row_count + 1) * (col_count + 1);
  vector<vector<pair<int, int>>> graph(vertex_count);
  for (int row = 1; row <= row_count; row++) {
    for (int col = 1; col <= col_count; col++) {
      add_tile_edges(graph, row, col, tiles[row][col]);
    }
  }

  vector<int> dist(vertex_count, k_inf);
  deque<pair<int, int>> queue;
  int source = vertex_id(0, 0);
  int target = vertex_id(row_count, col_count);
  dist[source] = 0;
  queue.push_front({source, 0});

  while (!queue.empty()) {
    auto [vertex, recorded] = queue.front();
    queue.pop_front();
    if (recorded != dist[vertex]) {
      continue;
    }
    if (vertex == target) {
      break;
    }
    for (const auto& [next_vertex, weight] : graph[vertex]) {
      if (dist[next_vertex] > dist[vertex] + weight) {
        dist[next_vertex] = dist[vertex] + weight;
        if (weight == 0) {
          queue.push_front({next_vertex, dist[next_vertex]});
        } else {
          queue.push_back({next_vertex, dist[next_vertex]});
        }
      }
    }
  }

  if (dist[target] == k_inf) {
    cout << -1 << '\n';
  } else {
    cout << dist[target] << '\n';
  }
  return 0;
}

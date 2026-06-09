/*
 * Solution for BOI 2011 Day 2: Tree Mirroring.
 *
 * Find mirror roots, BFS from both sides to locate axis vertices, then match
 * parent chains outward and verify the edge count of two identical trees.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_vertices = 100005;

int vertex_count;
int edge_count;
vector<int> adjacency[k_max_vertices];
int degree_count[k_max_vertices];
int distance_from_root[2][k_max_vertices];
int parent_toward_root[2][k_max_vertices];
int match_partner[k_max_vertices];
bool on_cycle[k_max_vertices];

void bfs_from(int root_index, int root_id) {
  fill(distance_from_root[root_index], distance_from_root[root_index] + vertex_count + 1, -1);
  fill(parent_toward_root[root_index], parent_toward_root[root_index] + vertex_count + 1, -1);
  queue<int> queue;
  distance_from_root[root_index][root_id] = 0;
  queue.push(root_id);
  while (!queue.empty()) {
    int vertex = queue.front();
    queue.pop();
    for (int neighbor : adjacency[vertex]) {
      if (distance_from_root[root_index][neighbor] != -1) {
        continue;
      }
      distance_from_root[root_index][neighbor] = distance_from_root[root_index][vertex] + 1;
      parent_toward_root[root_index][neighbor] = vertex;
      queue.push(neighbor);
    }
  }
}

bool extend_match(int left, int right) {
  while (left != -1 && right != -1) {
    if (match_partner[left] != 0 && match_partner[left] != right) {
      return false;
    }
    if (match_partner[right] != 0 && match_partner[right] != left) {
      return false;
    }
    match_partner[left] = right;
    match_partner[right] = left;
    left = parent_toward_root[0][left];
    right = parent_toward_root[1][right];
  }
  return true;
}

bool check_roots(int root_a, int root_b) {
  fill(match_partner, match_partner + vertex_count + 1, 0);
  bfs_from(0, root_a);
  bfs_from(1, root_b);

  int axis_count = 0;
  bool valid = true;
  for (int vertex = 1; vertex <= vertex_count; vertex++) {
    if (distance_from_root[0][vertex] == -1 || distance_from_root[1][vertex] == -1) {
      valid = false;
      break;
    }
    if (distance_from_root[0][vertex] == distance_from_root[1][vertex]) {
      axis_count++;
      valid &= extend_match(parent_toward_root[0][vertex], parent_toward_root[1][vertex]);
    }
  }
  return valid && vertex_count + axis_count - 2 == edge_count;
}

void mark_cycle_vertices() {
  vector<int> current_degree(degree_count, degree_count + vertex_count + 1);
  vector<bool> removed(vertex_count + 1);
  queue<int> queue;
  for (int vertex = 1; vertex <= vertex_count; vertex++) {
    if (current_degree[vertex] != 2) {
      queue.push(vertex);
    }
  }
  while (!queue.empty()) {
    int vertex = queue.front();
    queue.pop();
    if (removed[vertex]) {
      continue;
    }
    removed[vertex] = true;
    for (int neighbor : adjacency[vertex]) {
      if (removed[neighbor]) {
        continue;
      }
      if (--current_degree[neighbor] != 2) {
        queue.push(neighbor);
      }
    }
  }
  fill(on_cycle, on_cycle + vertex_count + 1, false);
  for (int vertex = 1; vertex <= vertex_count; vertex++) {
    on_cycle[vertex] = !removed[vertex] && current_degree[vertex] == 2;
  }
}

bool bfs_to_cycle(int start) {
  vector<int> visited(vertex_count + 1, 0);
  vector<int> cycle_hits;
  queue<int> queue;
  visited[start] = 1;
  queue.push(start);
  while (!queue.empty()) {
    int vertex = queue.front();
    queue.pop();
    for (int neighbor : adjacency[vertex]) {
      if (on_cycle[neighbor]) {
        if (visited[neighbor] == 0) {
          visited[neighbor] = 2;
          cycle_hits.push_back(neighbor);
        } else if (visited[neighbor] == 2) {
          cycle_hits.push_back(neighbor);
        }
        continue;
      }
      if (visited[neighbor]) {
        continue;
      }
      visited[neighbor] = 1;
      queue.push(neighbor);
    }
  }
  if (static_cast<int>(cycle_hits.size()) != 2) {
    return false;
  }
  return check_roots(cycle_hits[0], cycle_hits[1]);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> vertex_count >> edge_count;
  for (int edge = 0; edge < edge_count; edge++) {
    int first;
    int second;
    cin >> first >> second;
    adjacency[first].push_back(second);
    adjacency[second].push_back(first);
    degree_count[first]++;
    degree_count[second]++;
  }

  vector<int> degree_one;
  for (int vertex = 1; vertex <= vertex_count; vertex++) {
    if (degree_count[vertex] == 1) {
      degree_one.push_back(vertex);
    }
  }
  if (static_cast<int>(degree_one.size()) != 0 && static_cast<int>(degree_one.size()) != 2) {
    cout << "NO\n";
    return 0;
  }
  if (static_cast<int>(degree_one.size()) == 2) {
    cout << (check_roots(degree_one[0], degree_one[1]) ? "YES" : "NO") << '\n';
    return 0;
  }

  bool all_degree_two = true;
  for (int vertex = 1; vertex <= vertex_count; vertex++) {
    if (degree_count[vertex] != 2) {
      all_degree_two = false;
      break;
    }
  }
  if (all_degree_two && edge_count == vertex_count) {
    cout << (vertex_count % 2 == 0 ? "YES" : "NO") << '\n';
    return 0;
  }

  mark_cycle_vertices();
  vector<bool> explored(vertex_count + 1);
  for (int vertex = 1; vertex <= vertex_count; vertex++) {
    if (on_cycle[vertex] || explored[vertex]) {
      continue;
    }
    explored[vertex] = true;
    if (bfs_to_cycle(vertex)) {
      cout << "YES\n";
      return 0;
    }
  }

  cout << "NO\n";
  return 0;
}

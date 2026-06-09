/*
 * Solution for CEOI 2011 Day 2: Traffic.
 *
 * Reachable eastern junctions form a contiguous interval by planarity. Sweep
 * western junctions in increasing y and reuse visited nodes to compute north/south.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Junction {
  int id;
  int y;
};

int n, m, width;
vector<vector<int>> adjacency;
vector<vector<int>> reverse_adjacency;
vector<int> can_reach_east;

void bfs_from(int start, vector<char>& visited) {
  queue<int> q;
  visited[start] = 1;
  q.push(start);
  while (!q.empty()) {
    int node = q.front();
    q.pop();
    for (int next : adjacency[node]) {
      if (!visited[next]) {
        visited[next] = 1;
        q.push(next);
      }
    }
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int height;
  cin >> n >> m >> width >> height;
  vector<int> x_coord(n + 1);
  vector<int> y_coord(n + 1);
  vector<Junction> west;
  vector<Junction> east;
  for (int i = 1; i <= n; i++) {
    cin >> x_coord[i] >> y_coord[i];
    if (x_coord[i] == 0) {
      west.push_back({i, y_coord[i]});
    }
    if (x_coord[i] == width) {
      east.push_back({i, y_coord[i]});
    }
  }

  sort(west.begin(), west.end(),
       [](const Junction& lhs, const Junction& rhs) { return lhs.y < rhs.y; });
  sort(east.begin(), east.end(),
       [](const Junction& lhs, const Junction& rhs) { return lhs.y < rhs.y; });

  adjacency.assign(n + 1, {});
  reverse_adjacency.assign(n + 1, {});
  for (int i = 0; i < m; i++) {
    int from, to, kind;
    cin >> from >> to >> kind;
    adjacency[from].push_back(to);
    reverse_adjacency[to].push_back(from);
    if (kind == 2) {
      adjacency[to].push_back(from);
      reverse_adjacency[from].push_back(to);
    }
  }

  can_reach_east.assign(n + 1, 0);
  queue<int> q;
  for (const Junction& junction : east) {
    can_reach_east[junction.id] = 1;
    q.push(junction.id);
  }
  while (!q.empty()) {
    int node = q.front();
    q.pop();
    for (int prev : reverse_adjacency[node]) {
      if (!can_reach_east[prev]) {
        can_reach_east[prev] = 1;
        q.push(prev);
      }
    }
  }

  vector<int> answer_by_id(n + 1, 0);
  vector<char> visited(n + 1, 0);
  int east_count = static_cast<int>(east.size());
  int current_north = 0;
  int current_south = 0;

  for (const Junction& junction : west) {
    if (!can_reach_east[junction.id]) {
      continue;
    }
    if (!visited[junction.id]) {
      bfs_from(junction.id, visited);
    }

    int north = current_north;
    int south = east_count + 1;
    for (int i = 0; i < east_count; i++) {
      if (!visited[east[i].id]) {
        continue;
      }
      north = max(north, i + 1);
      south = min(south, i + 1);
    }

    if (north == current_north) {
      north = current_north;
    }
    if (south == east_count + 1) {
      south = current_south;
    }

    if (north > 0 && south > 0 && north >= south) {
      answer_by_id[junction.id] = north - south + 1;
    }
    current_north = north;
    if (south <= east_count) {
      current_south = south;
    }
  }

  sort(west.begin(), west.end(),
       [](const Junction& lhs, const Junction& rhs) { return lhs.y > rhs.y; });
  for (const Junction& junction : west) {
    cout << answer_by_id[junction.id] << '\n';
  }
  return 0;
}

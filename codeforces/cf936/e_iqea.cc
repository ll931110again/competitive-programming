// CF 936E — Iqea
// https://codeforces.com/contest/936/problem/E
//
// City graph = grid adjacency within Gridland. Offline queries: divide & conquer on time;
// shops in the right half are temporary multi-source BFS sources (rollback dist updates).

#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

constexpr int kInf = 1e9;
constexpr int kDx[4] = {1, -1, 0, 0};
constexpr int kDy[4] = {0, 0, 1, -1};

struct Query {
  int type;
  int city;
};

int n, q;
vector<int> xs, ys;
vector<vector<int>> adj;
vector<Query> queries;
vector<int> dist;
vector<int> answers;
vector<pair<int, int>> dist_stack;
deque<int> bfs_q;

unordered_map<long long, int> cell_id;

long long pack_cell(int x, int y) {
  return static_cast<long long>(x) * 300001LL + y;
}

int city_at(int x, int y) {
  const auto it = cell_id.find(pack_cell(x, y));
  return it == cell_id.end() ? -1 : it->second;
}

void add_sources(int l, int r) {
  for (int i = l; i < r; ++i) {
    if (queries[i].type != 1) {
      continue;
    }
    const int s = queries[i].city;
    if (dist[s] <= 0) {
      continue;
    }
    dist_stack.push_back({s, dist[s]});
    dist[s] = 0;
    bfs_q.push_back(s);
  }
  while (!bfs_q.empty()) {
    const int u = bfs_q.front();
    bfs_q.pop_front();
    const int du = dist[u];
    for (int v : adj[u]) {
      if (dist[v] > du + 1) {
        dist_stack.push_back({v, dist[v]});
        dist[v] = du + 1;
        bfs_q.push_back(v);
      }
    }
  }
}

void rollback_dist(int snap) {
  while (static_cast<int>(dist_stack.size()) > snap) {
    const auto [v, old] = dist_stack.back();
    dist_stack.pop_back();
    dist[v] = old;
  }
  bfs_q.clear();
}

void solve_time(int l, int r) {
  if (r - l == 1) {
    if (queries[l].type == 2) {
      const int d = dist[queries[l].city];
      answers[l] = d >= kInf ? -1 : d;
    }
    return;
  }
  const int mid = (l + r) >> 1;

  const int snap = static_cast<int>(dist_stack.size());
  add_sources(l, mid);
  for (int i = mid; i < r; ++i) {
    if (queries[i].type == 2) {
      const int d = dist[queries[i].city];
      answers[i] = d >= kInf ? -1 : d;
    }
  }
  rollback_dist(snap);

  solve_time(l, mid);

  const int snap2 = static_cast<int>(dist_stack.size());
  add_sources(l, mid);
  solve_time(mid, r);
  rollback_dist(snap2);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  xs.resize(n);
  ys.resize(n);
  adj.assign(n, {});
  for (int i = 0; i < n; ++i) {
    cin >> xs[i] >> ys[i];
    cell_id[pack_cell(xs[i], ys[i])] = i;
  }

  for (int i = 0; i < n; ++i) {
    for (int dir = 0; dir < 4; ++dir) {
      const int j = city_at(xs[i] + kDx[dir], ys[i] + kDy[dir]);
      if (j != -1) {
        adj[i].push_back(j);
      }
    }
  }

  cin >> q;
  queries.resize(q);
  answers.assign(q, -1);
  for (int i = 0; i < q; ++i) {
    int t, x, y;
    cin >> t >> x >> y;
    queries[i].type = t;
    queries[i].city = city_at(x, y);
  }

  dist.assign(n, kInf);
  solve_time(0, q);

  for (int i = 0; i < q; ++i) {
    if (queries[i].type == 2) {
      cout << answers[i] << '\n';
    }
  }
  return 0;
}

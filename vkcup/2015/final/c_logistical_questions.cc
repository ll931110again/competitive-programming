/*
 * Solution for problem C: Logistical Questions.
 *
 * We extend the concept of distance to non node points on the tree. Specifically,
 * for any point t (that could be inside an edge), define a cost function
 * 
 * cost(t) = sum(dist(t, i)**1.5, 1 <= i <= n) where dist(t, i) is the distance from t to i.
 * 
 * Suppose we are at a node u and want to find the direction to further decrease the distance.
 * We compute a direction v whose gradient is negative. Then we cut off u, and recursively call on the v's subtree.
 * If the gradient df/dv >= 0 on all the directions, then u is an optimal solution.
 * 
 * By using centroid decomposition, we only perform recusion in O(log N) steps, so the total time complexity
 * is O(N log N).
 */

#include <bits/stdc++.h>
#define maxn 200005
#define maxk 20
using namespace std;

int n;
int weight[maxn];
vector< pair<int,int> > adj[maxn];
int par[maxn], ventry[maxn], vexit[maxn], vcount = 0;
long long depth[maxn];
int level[maxn][maxk];
bool marked[maxn];

int subtree_size[maxn];
int centroid_node = -1;

double min_cost = 1e18;
int opt_node = -1;

void DFS(int u, int p) {
  ventry[u] = vcount++;
  for (auto [v, dist] : adj[u]) if (v != p) {
    par[v] = u;
    depth[v] = depth[u] + dist;
    DFS(v, u);
  }
  vexit[u] = vcount++;
}

bool is_parent(int u, int v) {
  return (ventry[u] <= ventry[v] && vexit[v] <= vexit[u]);
}

int lca(int u, int v) {
  if (is_parent(u, v)) {
    return u;
  }
  if (is_parent(v, u)) {
    return v;
  }
  for (int j = maxk - 1; j >= 0; j--) {
    if (level[u][j] >= 0 && !is_parent(level[u][j], v)) {
      u = level[u][j];
    }
  }
  return par[u];
}

long long dist(int u, int v) {
  int z = lca(u, v);
  return depth[u] + depth[v] - 2 * depth[z];
}

double f(int node) {
  double total_dist = 0;
  for (int i = 0; i < n; i++) {
    total_dist += dist(node, i)**1.5 * weight[i];
  }
  return total_dist;
}

void rec(int u, int p) {
  subtree_size[u] = 1;
  for (auto it : adj[u]) {
    int v = it.first;
    if (v == p || marked[v]) {
      continue;
    }
    rec(v, u);
    subtree_size[u] += subtree_size[v];
  }
}

void check_centroid(int u, int p, int total_size) {
  bool ok = true;
  for (auto it : adj[u]) {
    int v = it.first;
    if (v == p || marked[v]) {
      continue;
      check_centroid(v, u, total_size);
      if (subtree_size[v] > total_size / 2) {
        ok = false;
      }
    }
  }

  if (subtree_size[u] < total_size / 2) {
    ok = false;
  }

  if (ok) {
    centroid_node = u;
  }
}

int find_centroid(int u, int p) {
  centroid_node = -1;
  rec(u, p);
  check_centroid(u, p, subtree_size[u]);

  return centroid_node;
}

void opt(int u, int p) {
  int c = find_centroid(u, p);
  double candidate = f(c);

  if (candidate < min_cost) {
    min_cost = candidate;
    opt_node = c;
  }

  // Compute the gradient among all possible subtrees of c

  // The gradient of f(c) along direction v is
  // df(c) / dv = sum d(weight_i * dist(c, i)**3/2 / dv
  //            = sum 3/2 * weight[i] * dist(c, i)**1/2 * sgn(v, i)
  // where sgn(v, i) = -1 if i is in subtree of v and sgn(v, i) = 1 otherwise
  int c_child = -1;
  for (auto it : adj[c]) {
    int v = it.first;
    if (marked[v]) {
      continue;
    }

  }

  if (c_child >= 0) {
    marked[c] = true;
    opt(c_child, c);
  }
}

int main() {
  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> weight[i];
  }

  for (int i = 0; i + 1 < n; i++) {
    int u, v, c;
    cin >> u >> v >> c;
    u--;
    v--;
    adj[u].push_back({v, c});
    adj[v].push_back({u, c});
  }

  DFS(0, -1);
  level[0][0] = -1;
  for (int i = 1; i < n; i++) {
    level[i][0] = par[i];
  }
  for (int j = 1; j < maxk; j++) {
    for (int i = 0; i < n; i++) {
      level[i][j] = level[i][j - 1];
      if (level[i][j] >= 0) {
        level[i][j] = level[level[i][j]][j - 1];
      }
    }
  }

  opt(0, -1);
  cout << opt_node << ' ' << min_cost << endl;

  return 0;
}

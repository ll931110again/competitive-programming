// Codeforces Spectral::Cup 2026 Round 1 — Building Tree
// https://codeforces.com/contest/2222/problem/F
//
// Sketch
// ------
// Path length is mex of the weights on the path; dis(u, v) is the minimum over
// u-v paths. A path has mex <= k iff it avoids weight k, so
//
//   dis(u, v) = min { k : u and v stay connected after deleting weight-k edges }.
//
// Connected pairs always have some k in [0, m]: a simple path has at most m
// edges and there are m + 1 possible weights.
//
// The new graph is an MST on the q vertices with cost(i, j) = dis(c_i, c_j).
// Same color joins at cost 0 (empty path), so it is enough to MST the distinct
// colors. Output -1 if they do not lie in one original component.
//
// Kruskal never needs the pairwise matrix. For each candidate cost k, colors in
// the same component of G - {weight k} form a clique of available edges of cost
// <= k. Merging current color-components inside each clique (a star is enough)
// is Kruskal; each successful union is charged k.
//
// Rebuilding connectivity for every k is too slow. Divide-and-conquer over a
// weight interval [L, R) instead, with a rollback DSU that always contains every
// original edge whose weight is outside [L, R):
//
//   solve(L, R):
//     mid = (L + R) / 2
//     add weights [mid, R), charge merges with L, recurse [L, mid), roll back
//     add weights [L, mid), charge merges with mid, recurse [mid, R), roll back
//
// Left-then-right visits costs in increasing order, so the color DSU is ordinary
// Kruskal and is never rolled back.
//
// Complexity: O(m log m log n) per test.

#include <bits/stdc++.h>
using namespace std;

namespace {

struct DisjointSet {
  explicit DisjointSet(int n) : parent_(n) {
    iota(parent_.begin(), parent_.end(), 0);
  }

  int size() const {
    return static_cast<int>(parent_.size());
  }

  int find(int x) {
    return parent_[x] == x ? x : parent_[x] = find(parent_[x]);
  }

  bool unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) {
      return false;
    }
    parent_[a] = b;
    return true;
  }

  bool connected() {
    if (parent_.empty()) {
      return true;
    }
    const int root = find(0);
    for (int i = 1; i < size(); i++) {
      if (find(i) != root) {
        return false;
      }
    }
    return true;
  }

private:
  vector<int> parent_;
};

// Union-by-size, no path compression. Each component stores a tag: the dense id
// of one selected color inside it, or -1 if it contains none.
struct RollbackDisjointSet {
  RollbackDisjointSet(int n, vector<int> tags) : parent_(n), size_(n, 1), tag_(std::move(tags)) {
    iota(parent_.begin(), parent_.end(), 0);
  }

  int find(int x) const {
    while (x != parent_[x]) {
      x = parent_[x];
    }
    return x;
  }

  int tag(int x) const {
    return tag_[find(x)];
  }

  void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) {
      return;
    }
    if (size_[a] > size_[b]) {
      swap(a, b);
    }
    history_.push_back({a, b, size_[b], tag_[b]});
    parent_[a] = b;
    size_[b] += size_[a];
    if (tag_[b] == -1) {
      tag_[b] = tag_[a];
    }
  }

  int checkpoint() const {
    return static_cast<int>(history_.size());
  }

  void rollback(int saved) {
    while (static_cast<int>(history_.size()) > saved) {
      const Change change = history_.back();
      history_.pop_back();
      parent_[change.child] = change.child;
      size_[change.parent] = change.parent_size;
      tag_[change.parent] = change.parent_tag;
    }
  }

private:
  struct Change {
    int child;
    int parent;
    int parent_size;
    int parent_tag;
  };

  vector<int> parent_;
  vector<int> size_;
  vector<int> tag_;
  vector<Change> history_;
};

struct ColorIndex {
  vector<int> vertex_tag;
  int count = 0;
};

ColorIndex index_colors(int vertex_count, const vector<int>& query_colors) {
  ColorIndex index;
  index.vertex_tag.assign(vertex_count, -1);
  for (int color : query_colors) {
    if (index.vertex_tag[color] == -1) {
      index.vertex_tag[color] = index.count++;
    }
  }
  return index;
}

struct Solver {
  Solver(int vertex_count,
         int m,
         const vector<pair<int, int>>& endpoints,
         const vector<int>& weights,
         const vector<int>& query_colors)
      : Solver(vertex_count, m, endpoints, weights, index_colors(vertex_count, query_colors)) {}

  int64_t minimum_spanning_cost() {
    if (colors_.size() <= 1) {
      return 0;
    }
    divide_and_conquer(0, edge_count_ + 1);
    return colors_.connected() ? cost_ : -1;
  }

private:
  int edge_count_;
  vector<vector<pair<int, int>>> edges_by_weight_;
  RollbackDisjointSet graph_;
  DisjointSet colors_;
  int64_t cost_ = 0;

  Solver(int vertex_count,
         int m,
         const vector<pair<int, int>>& endpoints,
         const vector<int>& weights,
         ColorIndex index)
      : edge_count_(m),
        edges_by_weight_(m + 1),
        graph_(vertex_count, std::move(index.vertex_tag)),
        colors_(index.count) {
    for (int i = 0; i < m; i++) {
      edges_by_weight_[weights[i]].push_back(endpoints[i]);
    }
  }

  void add_weights(int first, int last, int merge_cost) {
    for (int weight = first; weight < last; weight++) {
      for (auto [x, y] : edges_by_weight_[weight]) {
        const int a = graph_.find(x);
        const int b = graph_.find(y);
        if (a == b) {
          continue;
        }
        const int tag_a = graph_.tag(a);
        const int tag_b = graph_.tag(b);
        if (tag_a != -1 && tag_b != -1) {
          if (colors_.unite(tag_a, tag_b)) {
            cost_ += merge_cost;
          }
        }
        graph_.unite(a, b);
      }
    }
  }

  void divide_and_conquer(int low, int high) {
    if (high - low == 1) {
      return;
    }
    const int mid = (low + high) / 2;

    int saved = graph_.checkpoint();
    add_weights(mid, high, low);
    divide_and_conquer(low, mid);
    graph_.rollback(saved);

    saved = graph_.checkpoint();
    add_weights(low, mid, mid);
    divide_and_conquer(mid, high);
    graph_.rollback(saved);
  }
};

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int test_count;
  cin >> test_count;
  while (test_count--) {
    int vertex_count, edge_count, query_count;
    cin >> vertex_count >> edge_count >> query_count;

    vector<pair<int, int>> endpoints(edge_count);
    vector<int> weights(edge_count);
    for (int i = 0; i < edge_count; i++) {
      int u, v, w;
      cin >> u >> v >> w;
      endpoints[i] = {u - 1, v - 1};
      weights[i] = w;
    }

    vector<int> query_colors(query_count);
    for (int i = 0; i < query_count; i++) {
      cin >> query_colors[i];
      --query_colors[i];
    }

    Solver solver(vertex_count, edge_count, endpoints, weights, query_colors);
    cout << solver.minimum_spanning_cost() << '\n';
  }

  return 0;
}

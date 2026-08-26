// Codeforces Spectral::Cup 2026 Round 1 — Statistics on Tree
// https://codeforces.com/contest/2222/problem/G
//
// Sketch
// ------
// Value of (u, v) is the largest component after deleting the u-v path.
// Root at a centroid r. For every LCA other than r, the "outside" piece
// n - (used child subtree sizes) is always the largest, so we can count
// those pairs from child-subtree sizes at each vertex:
//   one endpoint at x, other in a child of size s  ->  ans[n - s] += s
//   endpoints in two children of sizes s, t        ->  ans[n - s - t] += s * t
// Pairs (v, v) all have value n.
//
// The only wrong pairs are those with LCA = r that cross the largest
// centroid branch L of size A. Subtract their naive n - A - b contributions,
// then recount them as max(f(v), g(w)) where f (resp. g) is the largest
// piece on the L side (resp. other side). Two sorted arrays plus a merge
// count all pairwise maxima in O(n).
//
// Complexity: O(n sqrt n + n log n) per test.

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Solver {
  explicit Solver(int n) : n_(n), adj_(n), parent_(n, -1), subtree_(n) {}

  void add_edge(int u, int v) {
    adj_[u].push_back(v);
    adj_[v].push_back(u);
  }

  vector<int64_t> count_values() {
    vector<int64_t> ans(n_ + 1);
    if (n_ == 1) {
      ans[1] = 1;
      return ans;
    }

    compute_subtrees(0, -1);
    const int root = find_centroid(0, -1);
    compute_subtrees(root, -1);

    ans[n_] = n_;
    vector<int> freq(n_ + 2);
    for (int v = 0; v < n_; v++) {
      vector<int> child_sizes;
      vector<int> distinct;
      for (int to : adj_[v]) {
        if (parent_[to] != v) {
          continue;
        }
        const int s = subtree_[to];
        child_sizes.push_back(s);
        ans[n_ - s] += s;
        if (freq[s] == 0) {
          distinct.push_back(s);
        }
        freq[s]++;
      }
      for (int a : distinct) {
        for (int b : distinct) {
          if (b == a) {
            break;
          }
          ans[n_ - a - b] += int64_t(freq[a]) * freq[b] * a * b;
        }
        ans[n_ - a - a] += int64_t(freq[a]) * (freq[a] - 1) / 2 * a * a;
      }
      for (int s : child_sizes) {
        freq[s]--;
      }
    }

    int largest = -1;
    for (int to : adj_[root]) {
      if (parent_[to] != root) {
        continue;
      }
      if (largest < 0 || subtree_[to] > subtree_[largest]) {
        largest = to;
      }
    }
    const int large_size = subtree_[largest];
    for (int to : adj_[root]) {
      if (parent_[to] == root && to != largest) {
        ans[n_ - large_size - subtree_[to]] -= int64_t(large_size) * subtree_[to];
      }
    }

    vector<int> large_side;
    collect(largest, root, -1, large_side);
    vector<int> other_side;
    for (int to : adj_[root]) {
      if (parent_[to] == root && to != largest) {
        collect(to, root, n_ - large_size - subtree_[to], other_side);
      }
    }

    sort(large_side.begin(), large_side.end());
    sort(other_side.begin(), other_side.end());
    int i = 0;
    int j = 0;
    const int ns = static_cast<int>(other_side.size());
    const int nt = static_cast<int>(large_side.size());
    while (i < ns || j < nt) {
      if (j == nt) {
        ans[other_side[i++]] += j;
      } else if (i == ns) {
        ans[large_side[j++]] += i;
      } else if (other_side[i] < large_side[j]) {
        ans[other_side[i++]] += j;
      } else {
        ans[large_side[j++]] += i;
      }
    }
    return ans;
  }

private:
  int n_;
  vector<vector<int>> adj_;
  vector<int> parent_;
  vector<int> subtree_;

  void compute_subtrees(int v, int p) {
    parent_[v] = p;
    subtree_[v] = 1;
    for (int to : adj_[v]) {
      if (to == p) {
        continue;
      }
      compute_subtrees(to, v);
      subtree_[v] += subtree_[to];
    }
  }

  int find_centroid(int v, int p) {
    for (int to : adj_[v]) {
      if (to != p && subtree_[to] * 2 > n_) {
        return find_centroid(to, v);
      }
    }
    return v;
  }

  void collect(int v, int p, int best, vector<int>& values) {
    values.push_back(max(best, subtree_[v]));
    for (int to : adj_[v]) {
      if (to == p) {
        continue;
      }
      collect(to, v, max(best, subtree_[v] - subtree_[to]), values);
    }
  }
};

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int test_count;
  cin >> test_count;
  while (test_count--) {
    int n;
    cin >> n;
    Solver solver(n);
    for (int i = 0; i < n - 1; i++) {
      int u, v;
      cin >> u >> v;
      solver.add_edge(u - 1, v - 1);
    }
    const vector<int64_t> ans = solver.count_values();
    for (int i = 1; i <= n; i++) {
      cout << ans[i] << (i == n ? '\n' : ' ');
    }
  }
  return 0;
}

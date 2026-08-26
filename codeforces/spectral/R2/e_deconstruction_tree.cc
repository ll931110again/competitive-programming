// Codeforces Spectral::Cup 2026 Round 2 — Deconstruction Tree
// https://codeforces.com/contest/2229/problem/E
//
// Sketch
// ------
// Root the tree at n (1-based n, 0-based n-1). Node n is never removed and is
// always inserted into S. The maximum leaf only grows, so the sequence of
// newly inserted values is increasing.
//
// dp[x] = number of ways to reach a state whose current max(S) equals x.
// x can be inserted on top of max(S) = i iff
//   max(subtree(x) \ {x}) < i < x
// because the rest of x's subtree must already have been peeled off by a
// strictly larger outside leaf, and x itself must beat the current maximum.
// Those i form a range, so prefix sums of dp give every transition in O(1).
//
// Transitions that finish by inserting n are special: x can be the last value
// before n iff every node outside x's component among the children of n is
// smaller than x (otherwise a larger leftover leaf would have been inserted
// first). The initial state is the unique maximum original leaf.
//
// Complexity: O(n) per test.

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int kMod = 998244353;

struct Solver {
  explicit Solver(int n) : n_(n), adj_(n), max_descendant_(n, -1) {}

  void add_edge(int u, int v) {
    adj_[u].push_back(v);
    adj_[v].push_back(u);
  }

  int count_sets() {
    if (static_cast<int>(adj_[n_ - 1].size()) == 1) {
      return 1;
    }

    fill_max_descendants(n_ - 1, -1);
    const vector<char> can_finish = finishing_nodes();

    int start = -1;
    for (int v = 0; v < n_; v++) {
      if (static_cast<int>(adj_[v].size()) == 1) {
        start = v;
      }
    }

    vector<int> dp(n_), prefix(n_);
    dp[start] = 1;
    prefix[start] = 1;
    for (int i = start + 1; i < n_ - 1; i++) {
      const int low = max_descendant_[i] + 1;
      if (low < i) {
        dp[i] = prefix[i - 1];
        if (low > 0) {
          dp[i] -= prefix[low - 1];
          if (dp[i] < 0) {
            dp[i] += kMod;
          }
        }
      }
      prefix[i] = dp[i] + prefix[i - 1];
      if (prefix[i] >= kMod) {
        prefix[i] -= kMod;
      }
    }

    int ans = 0;
    for (int i = 0; i < n_; i++) {
      if (can_finish[i]) {
        ans += dp[i];
        if (ans >= kMod) {
          ans -= kMod;
        }
      }
    }
    return ans;
  }

private:
  void fill_max_descendants(int v, int parent) {
    for (int to : adj_[v]) {
      if (to == parent) {
        continue;
      }
      fill_max_descendants(to, v);
      max_descendant_[v] = max({max_descendant_[v], to, max_descendant_[to]});
    }
  }

  void collect_component(int v,
                         int parent,
                         set<int, greater<int>>& remaining,
                         vector<int>& component) const {
    remaining.erase(v);
    component.push_back(v);
    for (int to : adj_[v]) {
      if (to != parent) {
        collect_component(to, v, remaining, component);
      }
    }
  }

  vector<char> finishing_nodes() const {
    set<int, greater<int>> remaining;
    for (int v = 0; v < n_ - 1; v++) {
      remaining.insert(v);
    }

    vector<char> ok(n_);
    vector<int> component;
    for (int child : adj_[n_ - 1]) {
      collect_component(child, n_ - 1, remaining, component);
      const int outside_max = *remaining.begin();
      for (int v : component) {
        if (v > outside_max) {
          ok[v] = 1;
        }
      }
      for (int v : component) {
        remaining.insert(v);
      }
      component.clear();
    }
    ok[n_ - 1] = 1;
    return ok;
  }

  int n_;
  vector<vector<int>> adj_;
  vector<int> max_descendant_;
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
    cout << solver.count_sets() << '\n';
  }
  return 0;
}

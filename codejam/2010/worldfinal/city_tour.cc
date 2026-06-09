#include <bits/stdc++.h>
using namespace std;

namespace {

inline long long edge_key(int a, int b) {
  if (a > b)
    swap(a, b);
  return (static_cast<long long>(a) << 32) ^ static_cast<unsigned int>(b);
}

struct Solver {
  int n = 0;
  unordered_map<long long, vector<int>> children; // edge -> nodes added on that edge
  unordered_map<long long, int> memo;             // edge -> best downward cycle length
  int best_cycle = 0;

  int best_edge(int x, int y) {
    if (x > y)
      swap(x, y);
    long long key = edge_key(x, y);
    auto it = memo.find(key);
    if (it != memo.end())
      return it->second;

    int max_len = 2;
    int second_max_len = -1;

    auto ch_it = children.find(key);
    if (ch_it != children.end()) {
      for (int z : ch_it->second) {
        // Critical: the recurrence only uses nodes with z > max(x,y)=y,
        // otherwise the recursion graph can become cyclic.
        if (z <= y)
          continue;
        int len = best_edge(x, z) + best_edge(y, z) - 1;
        if (len > max_len) {
          second_max_len = max_len;
          max_len = len;
        } else if (len > second_max_len) {
          second_max_len = len;
        }
      }
    }

    if (max_len >= 3)
      best_cycle = max(best_cycle, max_len);
    if (second_max_len >= 3)
      best_cycle = max(best_cycle, max_len + second_max_len - 2);

    memo.emplace(key, max_len);
    return max_len;
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    Solver s;
    cin >> s.n;
    s.children.reserve(4 * s.n);
    s.memo.reserve(4 * s.n);

    for (int v = 4; v <= s.n; v++) {
      int a, b;
      cin >> a >> b;
      s.children[edge_key(a, b)].push_back(v);
    }

    // Seed only the one valid "above" relation from the initial triangle:
    // edge (1,2) has a third vertex 3 with 3 > max(1,2).
    s.children[edge_key(1, 2)].push_back(3);

    // Start recursion from the initial triangle edges.
    s.best_edge(1, 2);
    s.best_edge(1, 3);
    s.best_edge(2, 3);

    // There is always at least a 3-cycle (the original triangle).
    s.best_cycle = max(s.best_cycle, 3);

    cout << "Case #" << tc << ": " << s.best_cycle << "\n";
  }
  return 0;
}

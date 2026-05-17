#include <algorithm>
#include <map>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

class IndependentOfOR {
  vector<int> A;
  map<pair<int, vector<int>>, int> memo;

  static bool canAdd(const vector<int>& cur, int x) {
    unordered_set<int> nxt;
    nxt.reserve(cur.size() * 2 + 3);
    for (int v : cur)
      nxt.insert(v);
    for (int v : cur) {
      int t = v | x;
      if (nxt.count(t))
        return false;
      nxt.insert(t);
    }
    return (int)nxt.size() == 2 * (int)cur.size();
  }

  int dfs(int idx, vector<int> cur) {
    sort(cur.begin(), cur.end());
    cur.erase(unique(cur.begin(), cur.end()), cur.end());
    auto key = make_pair(idx, cur);
    auto it = memo.find(key);
    if (it != memo.end())
      return it->second;

    if (idx == (int)A.size()) {
      memo[key] = 0;
      return 0;
    }

    int best = dfs(idx + 1, cur);
    int x = A[idx];
    if (canAdd(cur, x)) {
      vector<int> nxt = cur;
      for (int v : cur)
        nxt.push_back(v | x);
      best = max(best, x + dfs(idx + 1, nxt));
    }
    memo[key] = best;
    return best;
  }

public:
  int maxSum(vector<int> A_) {
    A = A_;
    sort(A.begin(), A.end(), greater<int>());
    memo.clear();
    return dfs(0, {0});
  }
};

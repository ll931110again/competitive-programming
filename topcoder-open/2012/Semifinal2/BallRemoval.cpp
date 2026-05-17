#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class BallRemoval {
  string L;
  map<vector<pair<int, int>>, uint64_t> memo;

  static vector<int> flatten(const vector<pair<int, int>>& ivs) {
    vector<int> pos;
    for (auto [a, b] : ivs)
      for (int i = a; i <= b; ++i)
        pos.push_back(i);
    return pos;
  }

  static vector<pair<int, int>> removeTwo(vector<pair<int, int>> ivs, int i, int j) {
    vector<int> have;
    for (auto [a, b] : ivs)
      for (int x = a; x <= b; ++x)
        have.push_back(x);
    vector<int> rest;
    for (int x : have)
      if (x != i && x != j)
        rest.push_back(x);
    if (rest.empty())
      return {};
    vector<pair<int, int>> out;
    int s = rest[0], e = rest[0];
    for (size_t t = 1; t < rest.size(); ++t) {
      int x = rest[t];
      if (x == e + 1)
        e = x;
      else {
        out.push_back({s, e});
        s = e = x;
      }
    }
    out.push_back({s, e});
    return out;
  }

  uint64_t dfs(vector<pair<int, int>> ivs) {
    vector<int> pos = flatten(ivs);
    if (pos.size() == 1)
      return 1ULL << pos[0];
    auto it = memo.find(ivs);
    if (it != memo.end())
      return it->second;
    uint64_t ans = 0;
    for (size_t t = 1; t + 1 < pos.size(); ++t) {
      int k = pos[t];
      int i, j;
      if (L[k] == '<') {
        i = pos[t - 1];
        j = k;
      } else {
        i = k;
        j = pos[t + 1];
      }
      ans |= dfs(removeTwo(ivs, i, j));
    }
    memo[ivs] = ans;
    return ans;
  }

public:
  string canLeave(string label) {
    L = label;
    memo.clear();
    const int n = (int)label.size();
    uint64_t m = dfs({{0, n - 1}});
    string out(n, '.');
    for (int i = 0; i < n; ++i)
      if (m >> i & 1ULL)
        out[i] = 'o';
    return out;
  }
};

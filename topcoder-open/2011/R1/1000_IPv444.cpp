#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class IPv444 {
  struct Node {
    map<int, int> child;
    int star = -1;
    i64 starCap = 0;
  };

  vector<Node> trie;

  int newNode() {
    trie.push_back(Node());
    return (int)trie.size() - 1;
  }

  int goStar(int u) {
    if (trie[u].star == -1) {
      trie[u].star = newNode();
      trie[u].starCap = 1000;
    }
    return trie[u].star;
  }

  int goDigit(int u, int d) {
    auto it = trie[u].child.find(d);
    if (it == trie[u].child.end()) {
      int v = newNode();
      trie[u].child[d] = v;
      return v;
    }
    return it->second;
  }

  vector<int> parse(const string& req) {
    vector<int> parts;
    int i = 0;
    while (i < (int)req.size()) {
      if (req[i] == '*') {
        parts.push_back(-1);
        i += 2;
        continue;
      }
      int j = i;
      while (j < (int)req.size() && req[j] != '.')
        j++;
      parts.push_back(stoi(req.substr(i, j - i)));
      i = j + 1;
    }
    return parts;
  }

  i64 assign(int u, const vector<int>& parts, int depth, i64 take, i64 price) {
    if (take <= 0)
      return 0;
    if (depth == 4)
      return take * price;

    if (parts[depth] == -1) {
      if (depth == 3) {
        i64 used = min(take, trie[u].starCap);
        trie[u].starCap -= used;
        return used * price;
      }
      int v = goStar(u);
      i64 used = min(take, trie[u].starCap);
      trie[u].starCap -= used;
      return assign(v, parts, depth + 1, used, price);
    }

    int d = parts[depth];
    int v = goDigit(u, d);
    if (depth == 3 && trie[u].star != -1)
      trie[u].starCap = max(0LL, trie[u].starCap - take);
    return assign(v, parts, depth + 1, take, price);
  }

public:
  i64 getMaximumMoney(vector<string> request, vector<int> price) {
    int m = request.size();
    vector<int> order(m);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) { return price[a] > price[b]; });

    trie.clear();
    trie.push_back(Node());

    i64 ans = 0;
    for (int idx : order) {
      vector<int> parts = parse(request[idx]);
      i64 ways = 1;
      for (int p : parts)
        if (p == -1)
          ways *= 1000;
      ans += assign(0, parts, 0, ways, price[idx]);
    }
    return ans;
  }
};

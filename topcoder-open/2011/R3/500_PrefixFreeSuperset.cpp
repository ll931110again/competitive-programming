#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class PrefixFreeSuperset {
  struct Node {
    int child[2];
    bool terminal;
    bool blocked;
    Node() {
      child[0] = child[1] = -1;
      terminal = blocked = false;
    }
  };

  vector<Node> trie;

  int newNode() {
    trie.push_back(Node());
    return (int)trie.size() - 1;
  }

  void insertWord(const string& s) {
    int v = 0;
    for (char ch : s) {
      int b = ch - '0';
      if (trie[v].child[b] == -1)
        trie[v].child[b] = newNode();
      v = trie[v].child[b];
    }
    trie[v].terminal = true;
  }

  void markBlocked(int v) {
    if (trie[v].blocked)
      return;
    trie[v].blocked = true;
    if (trie[v].child[0] != -1)
      markBlocked(trie[v].child[0]);
    if (trie[v].child[1] != -1)
      markBlocked(trie[v].child[1]);
  }

  void initBlocked() {
    function<void(int)> dfs = [&](int v) {
      if (trie[v].terminal) {
        markBlocked(v);
        return;
      }
      if (trie[v].child[0] != -1)
        dfs(trie[v].child[0]);
      if (trie[v].child[1] != -1)
        dfs(trie[v].child[1]);
    };
    dfs(0);
  }

  void collectAvailable(
      int v, int depth,
      priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>>& pq) {
    if (trie[v].blocked)
      return;
    if (!trie[v].terminal)
      pq.push({depth, v});
    if (trie[v].child[0] != -1)
      collectAvailable(trie[v].child[0], depth + 1, pq);
    if (trie[v].child[1] != -1)
      collectAvailable(trie[v].child[1], depth + 1, pq);
  }

public:
  i64 minSumLength(vector<string> cur, i64 k) {
    trie.clear();
    trie.push_back(Node());
    i64 base = 0;
    for (const string& s : cur) {
      base += (i64)s.size();
      insertWord(s);
    }
    initBlocked();

    i64 need = k - (i64)cur.size();
    if (need < 0)
      return -1;

    priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>> pq;
    collectAvailable(0, 0, pq);

    i64 extra = 0;
    while (need > 0) {
      if (pq.empty())
        return -1;
      auto top = pq.top();
      pq.pop();
      i64 depth = top.first;
      int v = top.second;
      if (trie[v].blocked || trie[v].terminal)
        continue;
      trie[v].terminal = true;
      markBlocked(v);
      extra += depth;
      need--;
      if (trie[v].child[0] != -1)
        collectAvailable(trie[v].child[0], depth + 1, pq);
      if (trie[v].child[1] != -1)
        collectAvailable(trie[v].child[1], depth + 1, pq);
    }

    i64 ans = base + extra;
    if (ans > 1000000000000000000LL)
      return -2;
    return ans;
  }
};

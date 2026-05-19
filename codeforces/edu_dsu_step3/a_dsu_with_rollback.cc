// ITMO Academy: DSU step 3 — A. DSU with rollback
// https://codeforces.com/edu/course/2/lesson/7/3/practice/contest/289392/problem/A
//
// union x y | persist | rollback — print component count only after union/rollback.

#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;

  vector<int> parent(n + 1);
  for (int i = 1; i <= n; ++i) {
    parent[i] = i;
  }

  auto find = [&](int x) {
    while (parent[x] != x) {
      x = parent[x];
    }
    return x;
  };

  vector<vector<int>> batches;
  vector<int> pending;
  int components = n;

  for (int i = 0; i < m; ++i) {
    string op;
    cin >> op;
    if (op == "union") {
      int x, y;
      cin >> x >> y;
      x = find(x);
      y = find(y);
      if (x != y) {
        parent[y] = x;
        --components;
        pending.push_back(y);
      }
    } else if (op == "persist") {
      batches.push_back(std::move(pending));
      pending.clear();
    } else {
      for (int v : pending) {
        parent[v] = v;
        ++components;
      }
      pending = batches.back();
      batches.pop_back();
      cout << components << '\n';
    }
    if (op == "union") {
      cout << components << '\n';
    }
  }
  return 0;
}

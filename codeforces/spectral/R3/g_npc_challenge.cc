// Codeforces Spectral::Cup 2026 Round 3 — NPC Challenge
// https://codeforces.com/contest/2245/problem/G
//
// Interactive tree reconstruction via parity queries (editorial solution).

#include <bits/stdc++.h>
using namespace std;

namespace {

string query(const vector<int>& nodes) {
  cout << "? " << nodes.size();
  for (int v : nodes) {
    cout << ' ' << v + 1;
  }
  cout << endl;
  string response;
  cin >> response;
  return response;
}

vector<int> work(const vector<int>& left, const vector<int>& right) {
  if (left.empty() || right.empty()) {
    return {};
  }
  vector<int> merged;
  merged.reserve(left.size() + right.size());
  for (int v : left) {
    merged.push_back(v);
  }
  for (int v : right) {
    merged.push_back(v);
  }
  const string response = query(merged);
  vector<int> kept;
  kept.reserve(right.size());
  for (int i = 0; i < static_cast<int>(right.size()); i++) {
    if (response[i + left.size()] == '0') {
      kept.push_back(right[i]);
    }
  }
  return kept;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int tests;
  cin >> tests;
  while (tests--) {
    int n;
    cin >> n;
    vector<vector<int>> graph(n);

    function<void(vector<int>, vector<int>)> find =
        [&](vector<int> left, vector<int> right) -> void {
      if (left.empty() || right.empty()) {
        return;
      }
      if (left.size() == 1) {
        for (int v : right) {
          graph[left[0]].push_back(v);
          graph[v].push_back(left[0]);
        }
        return;
      }
      const int mid = static_cast<int>(left.size()) / 2;
      vector<int> left1(left.begin(), left.begin() + mid);
      vector<int> left2(left.begin() + mid, left.end());
      const vector<int> matched1 = work(left1, right);
      set<int> matched1_set(matched1.begin(), matched1.end());
      vector<int> nb;
      vector<int> rest;
      for (int v : right) {
        if (matched1_set.count(v)) {
          nb.push_back(v);
        } else {
          rest.push_back(v);
        }
      }
      find(left1, matched1);
      for (int v : work(left2, nb)) {
        rest.push_back(v);
      }
      find(left2, rest);
    };

    function<void(vector<int>)> dfs = [&](vector<int> nodes) -> void {
      if (nodes.size() <= 1) {
        return;
      }
      const string response = query(nodes);
      vector<int> part0;
      vector<int> part1;
      for (int i = 0; i < static_cast<int>(nodes.size()); i++) {
        if (response[i] == '0') {
          part1.push_back(nodes[i]);
        } else {
          part0.push_back(nodes[i]);
        }
      }
      dfs(part1);
      vector<int> colors(n, -1);
      vector<vector<int>> sides(2);
      for (int v : part1) {
        if (colors[v] != -1) {
          continue;
        }
        queue<int> q;
        q.push(v);
        colors[v] = 0;
        while (!q.empty()) {
          const int u = q.front();
          q.pop();
          sides[colors[u]].push_back(u);
          for (int w : graph[u]) {
            if (colors[w] == -1) {
              colors[w] = colors[u] ^ 1;
              q.push(w);
            }
          }
        }
      }
      for (int side = 0; side < 2; side++) {
        find(sides[side], work(sides[side], part0));
      }
    };

    vector<int> all(n);
    iota(all.begin(), all.end(), 0);
    dfs(all);

    cout << '!' << endl;
    for (int u = 0; u < n; u++) {
      for (int v : graph[u]) {
        if (u < v) {
          cout << u + 1 << ' ' << v + 1 << endl;
        }
      }
    }
  }
  return 0;
}

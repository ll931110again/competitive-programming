#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <unordered_set>
#include <vector>
#define maxn 200005
using namespace std;

int T;
int n, Q, K;
long long a[maxn];

struct node {
  int cnt;
  int max_sum;
  int nxt[2];
  bool is_full = false;
  bool is_leaf = false;
};

vector<node> nodes;
unordered_set<int> unallocated;

void rec(int idx, long long x, int i, int delta) {
  if (i < K) {
    int branch = (x & (1LL << i)) ? 1 : 0;
    if (nodes[idx].nxt[branch] < 0) {
      if (unallocated.empty()) {
        nodes.push_back(
            {.cnt = 0, .max_sum = 0, .is_full = true, .is_leaf = true});
        nodes[nodes.size() - 1].nxt[0] = nodes[nodes.size() - 1].nxt[1] = -1;
        nodes[idx].nxt[branch] = nodes.size() - 1;
      } else {
        auto it = unallocated.begin();
        auto x = *it;

        nodes[x] = {.cnt = 0, .max_sum = 0, .is_full = true, .is_leaf = true};
        nodes[x].nxt[0] = nodes[x].nxt[1] = -1;
        nodes[idx].nxt[branch] = x;
        unallocated.erase(it);
      }
    }
    rec(nodes[idx].nxt[branch], x, i + 1, delta);
  }

  for (int branch = 0; branch < 2; branch++) {
    if (nodes[idx].nxt[branch] >= 0 && nodes[nodes[idx].nxt[branch]].cnt == 0) {
      unallocated.insert(nodes[idx].nxt[branch]);
      nodes[idx].nxt[branch] = -1;
    }
  }

  nodes[idx].cnt += delta;
  nodes[idx].is_leaf = true;
  for (int branch = 0; branch < 2; branch++) {
    if (nodes[idx].nxt[branch] >= 0 && nodes[nodes[idx].nxt[branch]].cnt > 0) {
      nodes[idx].is_leaf = false;
      break;
    }
  }

  nodes[idx].is_full = true;
  if (!nodes[idx].is_leaf) {
    for (int branch = 0; branch < 2; branch++) {
      if (nodes[idx].nxt[branch] < 0) {
        nodes[idx].is_full = false;
        break;
      }
      if (nodes[nodes[idx].nxt[branch]].cnt == 0) {
        nodes[idx].is_full = false;
        break;
      }
      if (!nodes[nodes[idx].nxt[branch]].is_full &&
          !nodes[nodes[idx].nxt[branch]].is_leaf) {
        nodes[idx].is_full = false;
        break;
      }
    }
  }

  nodes[idx].max_sum = 0;
  for (int branch = 0; branch < 2; branch++) {
    if (nodes[idx].nxt[branch] >= 0 && !nodes[nodes[idx].nxt[branch]].is_full &&
        !nodes[nodes[idx].nxt[branch]].is_leaf) {
      nodes[idx].max_sum =
          max(nodes[idx].max_sum, nodes[nodes[idx].nxt[branch]].max_sum);
    }
  }
  if (idx > 0) {
    nodes[idx].max_sum += nodes[idx].cnt;
  }
}

void add(long long x) { rec(0, x, 0, 1); }

void remove(long long x) { rec(0, x, 0, -1); }

void solve() {
  nodes.clear();
  unallocated.clear();
  nodes.push_back({.cnt = 0, .max_sum = 0, .is_full = false, .is_leaf = true});
  nodes[0].nxt[0] = nodes[0].nxt[1] = -1;

  for (int i = 1; i <= n; i++) {
    add(a[i]);
  }

  cout << nodes[0].max_sum << endl;
  while (Q--) {
    int pos;
    long long val;
    cin >> pos >> val;

    remove(a[pos]);
    add(val);
    a[pos] = val;

    cout << nodes[0].max_sum << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n >> Q >> K;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }
    solve();
  }

  return 0;
}

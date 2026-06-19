// CF 914E — Palindromes in a Tree
// https://codeforces.com/contest/914/problem/E
//
// Centroid decomposition: palindromic path <=> XOR mask has at most one bit set.
// Based on https://gist.github.com/luciocf/68f77b08dc3f9e2787b37d00fa23ed89

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 200010;

int valc[k_max_n];
int sz[k_max_n];
int freq[1 << 20];
i64 ans[k_max_n];
bool dead[k_max_n];

vector<int> adj[k_max_n];

void dfs_sz(int u, int p) {
  sz[u] = 1;
  for (int v : adj[u]) {
    if (v == p || dead[v]) {
      continue;
    }
    dfs_sz(v, u);
    sz[u] += sz[v];
  }
}

int centroid(int u, int p, int tot) {
  for (int v : adj[u]) {
    if (v != p && !dead[v] && sz[v] > tot / 2) {
      return centroid(v, u, tot);
    }
  }
  return u;
}

void upd(int u, int p, int mask, int delta) {
  freq[mask] += delta;
  for (int v : adj[u]) {
    if (v == p || dead[v]) {
      continue;
    }
    upd(v, u, mask ^ (1 << valc[v]), delta);
  }
}

i64 get_paths(int u, int p, int mask) {
  i64 res = freq[mask];
  for (int i = 0; i < 20; ++i) {
    res += freq[mask ^ (1 << i)];
  }
  for (int v : adj[u]) {
    if (v == p || dead[v]) {
      continue;
    }
    res += get_paths(v, u, mask ^ (1 << valc[v]));
  }
  ans[u] += res;
  return res;
}

void decompose(int entry) {
  dfs_sz(entry, 0);
  const int c = centroid(entry, 0, sz[entry]);
  dead[c] = true;

  freq[1 << valc[c]]++;
  for (int v : adj[c]) {
    if (dead[v]) {
      continue;
    }
    ans[c] += get_paths(v, c, 1 << valc[v]);
    upd(v, c, (1 << valc[c]) ^ (1 << valc[v]), 1);
  }
  upd(c, 0, 1 << valc[c], -1);

  for (int i = static_cast<int>(adj[c].size()) - 1; i >= 0; --i) {
    int v = adj[c][i];
    if (dead[v]) {
      continue;
    }
    get_paths(v, c, 1 << valc[v]);
    upd(v, c, (1 << valc[c]) ^ (1 << valc[v]), 1);
  }

  for (int v : adj[c]) {
    if (!dead[v]) {
      upd(v, c, (1 << valc[c]) ^ (1 << valc[v]), -1);
    }
  }
  for (int v : adj[c]) {
    if (!dead[v]) {
      decompose(v);
    }
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  for (int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  for (int i = 1; i <= n; ++i) {
    char c;
    cin >> c;
    valc[i] = c - 'a';
  }

  decompose(1);
  for (int i = 1; i <= n; ++i) {
    cout << ans[i] + 1 << (i == n ? '\n' : ' ');
  }
  return 0;
}

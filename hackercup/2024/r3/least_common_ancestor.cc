
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 3000005;
constexpr unsigned MOD = 998'244'353;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

int T, n;
int p[k_max_n];
string s[k_max_n];

int up[k_max_n], down[k_max_n];
set<string> universes;
map<string, int> indices;
vector<int> adj[k_max_n];

int occurences[k_max_n];
set<pair<int, int>> values;

int down_ref[k_max_n];
unordered_map<int, int> down_occurences[k_max_n];
set<pair<int, int>> down_values[k_max_n];

void update(pair<int, int> old_value, pair<int, int> new_value) {
  values.erase(old_value);

  if (new_value.first > 0) {
    values.insert(new_value);
  }
}

int find_min_index() {
  if (values.empty()) {
    return 0;
  }
  return values.begin()->second;
}

void recurse_up(int i) {
  up[i] = find_min_index();

  int ix = indices[s[i]];
  update({occurences[ix], ix}, {occurences[ix] + 1, ix});
  occurences[ix]++;

  for (auto v : adj[i]) {
    recurse_up(v);
  }

  update({occurences[ix], ix}, {occurences[ix] - 1, ix});
  occurences[ix]--;
}

int get(int u) {
  if (u != down_ref[u]) {
    down_ref[u] = get(down_ref[u]);
  }
  return down_ref[u];
}

void merge(int u, int v) {
  u = get(u);
  v = get(v);

  if (down_values[u].size() < down_values[v].size()) {
    swap(u, v);
  }

  down_ref[v] = u;
  for (auto it : down_occurences[v]) {
    int k = it.first, cnt = it.second;
    down_values[u].erase({down_occurences[u][k], k});
    down_occurences[u][k] += cnt;
    down_values[u].insert({down_occurences[u][k], k});
  }

  down_occurences[v].clear();
  down_values[v].clear();
}

void recurse_down(int i) {
  for (auto v : adj[i]) {
    recurse_down(v);
  }

  for (auto v : adj[i]) {
    merge(i, v);
  }

  int ref = get(i);
  if (down_values[ref].empty()) {
    down[i] = 0;
  } else {
    down[i] = down_values[ref].begin()->second;
  }

  // Add s[i]'s in
  int ix = indices[s[i]];
  down_values[ref].erase({down_occurences[ref][ix], ix});
  down_occurences[ref][ix]++;
  down_values[ref].insert({down_occurences[ref][ix], ix});
}

int solve() {
  universes.clear();
  indices.clear();

  for (int i = 1; i <= n; i++) {
    universes.insert(s[i]);
  }

  for (int i = 1; i <= n; i++) {
    down_values[i].clear();
    down_occurences[i].clear();
    down_ref[i] = i;
  }

  values.clear();

  int ix = 0;
  for (auto it : universes) {
    indices[it] = ++ix;
    occurences[ix] = 0;
  }

  recurse_up(1);
  recurse_down(1);

  long long ans = 0;
  int ux = universes.size();
  for (int i = 1; i <= n; i++) {
    ans = ans * (ux + 1) + up[i];
    ans %= MOD;
    ans = ans * (ux + 1) + down[i];
    ans %= MOD;
  }

  return ans;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  for (int it = 1; it <= T; it++) {
    cerr << it << endl;

    cin >> n;
    for (int i = 1; i <= n; i++) {
      adj[i].clear();
    }

    for (int i = 1; i <= n; i++) {
      cin >> p[i] >> s[i];
      if (p[i] >= 0) {
        adj[p[i]].push_back(i);
      }
    }

    cout << "Case #" << it << ": " << solve() << endl;
  }

  return 0;
}

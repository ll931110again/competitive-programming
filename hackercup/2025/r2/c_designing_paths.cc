
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 500005;
int T;
int n, k, m;
vector<vector<int>> trams;
unordered_map<int, int> positions[k_max_n];

priority_queue<pair<int, vector<int>>> q;
map<vector<int>, int> dist;

void put(vector<int> state, int d) {
  if (dist.find(state) == dist.end() || dist[state] > d) {
    dist[state] = d;
    q.push({-d, state});
  }
}

void update(int idx, int low, int high, int u, int v, int d) {
  if (high < u || v < low) {
    return;
  }
  if (u <= low && high <= v) {
    auto state = {1, idx, low, high};
    put(state, d);
    return;
  }

  int mid = (low + high) / 2;
  update(idx, low, mid, u, v, d);
  update(idx, mid + 1, high, u, v, d);
}

i64 solve() {
  dist.clear();
  while (!q.empty()) {
    q.pop();
  }

  dist[{0, 1}] = 0;
  q.push({0, {0, 1}});

  while (!q.empty()) {
    auto u = q.top().second;
    q.pop();

    if (u[0] == 0) {
      // regular node
      for (auto it : positions[u[1]]) {
        int tram_idx = it.first, offset = it.second;
        update(tram_idx, 0, trams[tram_idx].size() - 1, offset + 1, offset + k, 1 + dist[u]);
      }
    } else {
      // interval node
      int tram_idx = u[1], low = u[2], high = u[3];
      if (low == high) {
        auto state = {0, trams[tram_idx][low]};
        put(state, dist[u]);
      } else {
        int mid = (low + high) / 2;
        auto state = {1, tram_idx, low, mid};
        put(state, dist[u]);

        state = {1, tram_idx, mid + 1, high};
        put(state, dist[u]);
      }
    }
  }

  i64 ans = 0;
  for (int i = 1; i <= n; i++) {
    auto state = {0, i};
    int d = -1;
    if (dist.find(state) != dist.end()) {
      d = dist[state];
    }
    ans += 1LL * d * i;
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
    cin >> n >> k >> m;
    trams.clear();
    for (int i = 1; i <= n; i++) {
      positions[i].clear();
    }

    int cur_idx = 0;
    for (int i = 0; i < m; i++) {
      trams.push_back({});

      int len;
      cin >> len;
      for (int j = 0; j < len; j++) {
        int x;
        cin >> x;
        trams.back().push_back(x);
      }

      int nxt_idx = trams.size();
      for (int u = cur_idx; u < nxt_idx; u++) {
        for (int j = 0; j < trams[u].size(); j++) {
          positions[trams[u][j]][u] = j;
        }
      }

      cur_idx = nxt_idx;
    }

    cout << "Case #" << it << ": " << solve() << endl;
  }

  return 0;
}

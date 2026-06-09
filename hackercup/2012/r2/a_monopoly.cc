#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 30005;
int T, n, d;
pair<int, int> order[k_max_n], new_order[k_max_n];
int pos[k_max_n];
vector<int> bucket[k_max_n];

bool cmp(int x, int y) {
  return order[x] < order[y];
}

vector<int> refine(vector<int> mu) {
  vector<int> _mu;
  for (int i = 0; i < mu.size(); i++) {
    if (order[mu[i]].second > d)
      order[mu[i]].first = n + 8;
    _mu.push_back(mu[i]);
  }
  sort(_mu.begin(), _mu.end(), cmp);
  return _mu;
}

void merge(int mu, int mv) {
  if (bucket[mu].size() < bucket[mv].size())
    swap(mu, mv);
  for (int i = 0; i < bucket[mv].size(); i++) {
    bucket[mu].push_back(bucket[mv][i]);
    pos[bucket[mv][i]] = mu;
  }
  bucket[mv].clear();
  sort(bucket[mu].begin(), bucket[mu].end(), cmp);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("monopoly.in.2", "r", stdin);
  freopen("monopoly.out.2", "w", stdout);
  cin >> T;
  for (int it = 1; it <= T; it++) {
    cerr << it << '\n';
    cin >> n >> d;
    for (int i = 1; i <= n; i++) {
      bucket[i].clear();
      bucket[i].push_back(i);
      order[i] = make_pair(1, 0);
      pos[i] = i;
    }
    for (int i = 1; i < n; i++) {
      int u, v;
      cin >> u >> v;
      int mu = pos[u], mv = pos[v];
      int smu = order[bucket[mu][0]].first, smv = order[bucket[mv][0]].first;
      for (int j = 0; j < bucket[mu].size(); j++) {
        int x = bucket[mu][j];
        if (order[x].second >= d)
          new_order[x] = make_pair(n + 8, d + 3);
        else {
          new_order[x].second = order[x].second + 1;
          new_order[x].first = max(1 + smv, order[x].first);
        }
      }
      for (int j = 0; j < bucket[mv].size(); j++) {
        int x = bucket[mv][j];
        if (order[x].second >= d)
          new_order[x] = make_pair(n + 8, d + 3);
        else {
          new_order[x].second = order[x].second + 1;
          new_order[x].first = max(1 + smu, order[x].first);
        }
      }
      for (int j = 0; j < bucket[mu].size(); j++)
        order[bucket[mu][j]] = new_order[bucket[mu][j]];
      for (int j = 0; j < bucket[mv].size(); j++)
        order[bucket[mv][j]] = new_order[bucket[mv][j]];
      merge(mu, mv);
    }
    int ret = n;
    for (int i = 1; i <= n; i++)
      if (order[i].second <= d)
        ret = min(ret, order[i].first);
    cout << "Case #" << it << ": " << ret << '\n';
  }
}

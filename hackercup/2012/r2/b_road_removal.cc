#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 10005;
#define maxm 50005

int T, n, m, k;
int ma[maxm], mb[maxm];
int pre[k_max_n], rank[k_max_n], ret;
bool import[k_max_n];

int root(int x) {
  if (x != pre[x])
    pre[x] = root(pre[x]);
  return pre[x];
}

void link(int u, int v) {
  u = root(u);
  v = root(v);
  if (u == v)
    return;
  if (rank[u] > rank[v]) {
    import[u] |= import[v];
    pre[v] = u;
  } else {
    import[v] |= import[u];
    pre[u] = v;
  }
  if (rank[u] == rank[v])
    rank[v]++;
}

void check_link(int u, int v) {
  u = root(u);
  v = root(v);
  if (u == v && import[u]) {
    ret++;
    return;
  }
  link(u, v);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("road.in.2", "r", stdin);
  freopen("road.out.2", "w", stdout);
  cin >> T;
  for (int it = 1; it <= T; it++) {
    cin >> n >> m >> k;
    for (int i = 0; i < m; i++)
      cin >> ma[i] >> mb[i];
    for (int i = 0; i < n; i++)
      pre[i] = i, rank[i] = 0;
    for (int i = 0; i < n; i++)
      import[i] = (i < k);
    for (int i = 0; i < m; i++)
      if (ma[i] >= k && mb[i] >= k)
        link(ma[i], mb[i]);
    ret = 0;
    for (int i = 0; i < m; i++)
      if (ma[i] < k || mb[i] < k)
        check_link(ma[i], mb[i]);
    cout << "Case #" << it << ": " << ret << '\n';
  }
}

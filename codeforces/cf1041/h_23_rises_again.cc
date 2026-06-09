
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 40;
#define maxm 800

pair<int, int> edges[maxm];
int cnt[k_max_n];
int T, n, m;

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  random_device rd;
  mt19937 g(rd());

  cin >> T;
  while (T--) {
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
      cin >> edges[i].first >> edges[i].second;
    }

    int ret = 0;
    for (int iter = 0; iter < 3e5; iter++) {
      shuffle(edges, edges + m, g);
      int value = 0;

      memset(cnt, 0, sizeof cnt);
      for (int i = 0; i < m; i++) {
        int u = edges[i].first, v = edges[i].second;
        if (cnt[u] >= 2 || cnt[v] >= 2) {
          continue;
        }
        value++;
        cnt[u]++;
        cnt[v]++;
      }

      ret = max(ret, value);
    }
    cout << ret << endl;
  }

  return 0;
}

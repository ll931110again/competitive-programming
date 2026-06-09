#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
constexpr unsigned MOD = 1'000'000'007;
#include "../../lib/modint.h"
using Mint = ModInt<MOD>;

struct Event {
  int l, r;
  Mint p;
};

int T, n, k;
event E[k_max_n];
Mint ans[k_max_n];

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  /*
   * The probability that i is covered by exactly one segment is
      p_1 (1 - p_2)..(1 - p_n) + p_2 (1 - p_1).. (1 - p_n) + ..
      = (sum p_i / (1 - p_i)) * (1 - p_1) .. (1 - p_n)
   */

  cin >> n >> k;
  for (int i = 1; i <= k; i++) {
    int p, q;
    cin >> E[i].l >> E[i].r >> p >> q;
    E[i].p = p;
    E[i].p /= q;
  }

  vector<pair<int, int>> events;
  for (int i = 1; i <= k; i++) {
    events.push_back({E[i].l, i});
    events.push_back({E[i].r + 1, -i});
  }
  sort(events.begin(), events.end());

  Mint sum_value = 0, prod_value = 1;

  int it = 0;
  for (int i = 1; i <= n; i++) {
    while (it < events.size() && events[it].first <= i) {
      int pos = abs(events[it].second);
      if (events[it].second > 0) {
        sum_value += E[pos].p / (1 - E[pos].p);
        prod_value *= (1 - E[pos].p);
      } else {
        sum_value -= E[pos].p / (1 - E[pos].p);
        prod_value /= (1 - E[pos].p);
      }
      it++;
    }
    ans[i] = sum_value * prod_value;
  }

  for (int i = 1; i <= n; i++) {
    cout << ans[i];
    if (i < n) {
      cout << " ";
    } else {
      cout << endl;
    }
  }

  return 0;
}

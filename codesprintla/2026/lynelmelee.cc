/*
    E — Lynel Melee (user AC)
*/

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 1005;
int n, b;
int num_arrows[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> b;
  for (int i = 1; i <= n; i++) {
    cin >> num_arrows[i];
  }

  i64 cnt_small = 0;
  for (int i = 1; i < 5; i++) {
    cnt_small += num_arrows[i];
  }

  i64 total_large = 0;
  for (int i = 5; i <= n; i++) {
    total_large += i * num_arrows[i];
  }

  int bound = 2 * k_max_n - 1;

  bool stopped = false;
  for (int i = n; i >= 5; i--) {
    if (stopped) {
      break;
    }

    while (num_arrows[i] > 0) {
      if (b >= bound) {
        b -= i;
        total_large -= i;
        num_arrows[i]--;
      } else {
        stopped = true;
        break;
      }
    }
  }

  bool cur[2 * k_max_n], nxt[2 * k_max_n];
  memset(cur, false, sizeof cur);
  memset(nxt, false, sizeof nxt);
  cur[0] = true;

  for (int i = 5; i <= n; i++) {
    for (int j = 0; j < num_arrows[i]; j++) {
      for (int t = 0; t <= bound; t++) {
        nxt[t] = cur[t];
        if (t >= i) {
          nxt[t] |= cur[t - i];
        }
      }
      for (int t = 0; t <= bound; t++) {
        cur[t] = nxt[t];
      }
    }
  }

  i64 total_damage = 0;
  for (int t = 0; t <= bound && t <= total_large; t++)
    if (cur[t]) {
      // use smallest number of light arrows to exceed b - t damages
      i64 rem = b - t, cnt = 0;
      for (int j = 4; j > 0; j--) {
        if (rem < 0) {
          break;
        }
        int amount = min(rem, (i64)num_arrows[j] * j) / j;
        cnt += amount;
        rem -= amount * j;
        if (amount < num_arrows[j]) {
          cnt++;
          rem -= j;
        }
      }

      if (rem >= 0) {
        continue;
      }

      i64 damage = 0;
      if (t > b) {
        damage = (total_large - t) + 5 + cnt_small * 5;
      } else {
        if (cnt == 0) {
          damage = (total_large - t) + cnt_small * 5;
        } else {
          damage = (total_large - t) + (cnt_small - cnt) * 5 + 5;
        }
      }

      total_damage = max(total_damage, damage);
    }

  cout << total_damage << endl;
  return 0;
}

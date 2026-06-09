#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 105;
#define maxk 6

int n;
int a[k_max_n];
int occupy_cnt[maxk];

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  sort(a, a + n);
  int ans = 0;
  for (int i = n - 1; i >= 0; i--) {
    bool filled = false;
    for (int j = a[i]; j < maxk; j++)
      if (occupy_cnt[j]) {
        occupy_cnt[j]--;
        occupy_cnt[j - a[i]]++;
        filled = true;
        break;
      }
    if (!filled) {
      ans++;
      occupy_cnt[maxk - a[i]]++;
    }
  }

  cout << ans << endl;

  return 0;
}


#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
int T, n;
string s;
int cnt[3];

i64 solve() {
  memset(cnt, 0, sizeof cnt);
  cnt[0] = 1;

  i64 ans = 0;
  int alternating_len = 0;
  int smod = 0;

  for (int i = 0; i < n; i++) {
    if (i == 0 || s[i] != s[i - 1]) {
      alternating_len++;
    } else {
      alternating_len = 1;
    }

    if (s[i] == '1') {
      smod = (smod + 1) % 3;
    } else {
      smod = (smod + 2) % 3;
    }

    ans += (cnt[0] + cnt[1] + cnt[2] - cnt[smod]);

    // Exclude alternating strings of length 3, 5, 7, ..
    ans -= (alternating_len - 1) / 2;

    cnt[smod]++;
  }

  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> s;
    cout << solve() << endl;
  }
  return 0;
}

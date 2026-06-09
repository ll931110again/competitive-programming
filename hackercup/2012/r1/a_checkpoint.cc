#include <bits/stdc++.h>
using namespace std;

namespace {

map<int, int> mp;
int T, s;

int find_comb(int x) {
  if (x == 1)
    return 1;
  if (mp.count(x))
    return mp[x];
  for (int i = 1; i <= x; i++) {
    long long k = 1;
    for (int j = 1; j <= i; j++) {
      k = k * 1LL * (i + 1 - j) / j;
      if (k > x)
        break;
      if (k == x)
        return mp[x] = i;
    }
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("checkpoint.in.1", "r", stdin);
  freopen("checkpoint.out.1", "w", stdout);
  cin >> T;
  for (int it = 1; it <= T; it++) {
    cin >> s;
    int ret = 2 * s;
    for (int i = 1; i * i <= s; i++)
      if (s % i == 0)
        ret = min(ret, find_comb(i) + find_comb(s / i));
    cout << "Case #" << it << ": " << ret << '\n';
  }
}
